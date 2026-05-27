---
description: TU (translation unit) picking and dependency analysis for this Paperboy N64 decomp. Use whenever the user asks what to decompile next, what file to work on next, which TU has the highest impact, what's blocking what, when they want to mark a TU as done after finishing it, when they ask for an Australian animal name for an unnamed class, or when they want to know which symbols from a TU are referenced by other TUs. Triggers on "what's next", "what file should I work on next", "what should I do next", "which to do next", "high-leverage", "high-impact", "what should I decompile next", "X is done"/"X is decomped"/"mark X done" (where X is a vrom or class name), "give me an animal name"/"new animal name", "which externs from X are used", "tu_deps", "tu-deps", "next pick", "unblocks the most", "what TU next".
---

# TU Dependency Picking

Operational skill for steering decompilation order in this project. The dependency graph and "decompiled" state both live in `tools/tu_deps.py`.

## When to use this skill

- "What should I decompile next?" / "what's the next high-impact TU?" — run the leverage script (below) and recommend.
- "X is done" / "0xNNNN is decomped" — add it to `DECOMPILED_VROMS` in `tools/tu_deps.py`, then recompute and present the new top picks.
- "Give me an animal name" — find an unused Australian-animal class name (see naming section).
- "Which symbols from TU 0xNNNN are used elsewhere?" — run the cross-TU reference script.
- "Show the dep tree" / "what's blocked" — run `uv run tools/tu_deps.py [segment]`.

## Mental model

`tools/tu_deps.py` builds a TU-level dep graph from the linker map + readelf `.text` relocations. Three states:
- **green** — in `DECOMPILED_VROMS` (or `IGNORED_VROMS`, or has no `.text`). Treated as "done" for dep purposes.
- **yellow** — every dep is green. Ready to decompile right now.
- **uncoloured** — at least one dep is still uncoloured. Blocked.

"Leverage" of a yellow TU Y = how many *other* TUs become decompilable (transitively) if Y is finished, while no other currently-yellow TU is touched. That's the metric every "what next?" question really wants.

## State inspection

```bash
uv run tools/tu_deps.py            # default segment is .entry
uv run tools/tu_deps.py first       # other segment
```

Reads `build/ntsc/paperboy.map` + `build/ntsc/**/*.o`. If those are missing, ask the user to build first; do not run `configure.py` to "fix" it (see `decomp` skill — reconfiguring wipes asm).

## Leverage computation — the "what's next" script

Run this whenever the user asks what to do next, or after marking a TU done. Output covers all segments, sorted by transitive unblock count.

```bash
uv run --script - <<'PY'
# /// script
# requires-python = ">=3.11"
# dependencies = ["mapfile-parser"]
# ///
import sys
sys.path.insert(0, 'tools')
from tu_deps import load_tus, build_graph, DECOMPILED_VROMS, IGNORED_VROMS, short_name

tus, segments = load_tus()
deps, _ = build_graph(tus)

decomp = {k for k, tu in tus.items()
          if tu["vrom"] in DECOMPILED_VROMS or tu["vrom"] in IGNORED_VROMS
          or not tu["has_text"]}
key_to_seg = {k: name for name, keys in segments for k in keys}

def ready(state):
    return {k for k in tus if k not in state and deps[k] <= state}

base_yellow = ready(decomp)

results = []
for y in base_yellow:
    # cascade assuming y alone is decompiled, with all other yellows held back —
    # this measures y's pure leverage, not the cumulative effect of all yellows.
    locked = decomp | {y}
    state = set(locked)
    forbidden = base_yellow - {y}
    while True:
        r = {k for k in tus if k not in state and deps[k] <= state and k not in forbidden}
        if not r:
            break
        state |= r
    unblocked = state - locked
    direct = ready(decomp | {y}) - base_yellow
    results.append((len(unblocked), len(direct), y, unblocked))

results.sort(key=lambda r: (-r[0], -r[1], tus[r[2]]["vrom"]))

print(f"{'vrom':>10}  {'name':<22}  {'seg':<14}  {'trans':>5}  {'direct':>6}  exports")
print("-" * 100)
for trans, direct, k, _ in results:
    if trans == 0 and direct == 0:
        continue
    tu = tus[k]
    seg = key_to_seg.get(k, "?")
    exports = ", ".join(tu["exports"]) if tu["exports"] else ""
    print(f"  0x{tu['vrom']:06X}  {short_name(k):<22}  {seg:<14}  {trans:>5}  {direct:>6}  {exports}")

print("\nTop 8 — what each one unblocks:")
for trans, direct, k, unblocked in results[:8]:
    if trans == 0 and direct == 0:
        continue
    tu = tus[k]
    seg = key_to_seg.get(k, "?")
    print(f"\n  0x{tu['vrom']:06X} {short_name(k)}  [{seg}]  ({', '.join(tu['exports']) or '—'}):")
    for u in sorted(unblocked, key=lambda x: tus[x]["vrom"]):
        ut = tus[u]
        useg = key_to_seg.get(u, "?")
        exp = ", ".join(ut["exports"]) if ut["exports"] else ""
        print(f"    -> 0x{ut['vrom']:06X}  {short_name(u):<22}  [{useg}]  {exp}")
PY
```

### How to read the result

- `trans` is the headline number — bigger means more downstream TUs become reachable.
- `direct` is how many cascade in *one* step (the rest land via subsequent rounds).
- A TU with `trans == 0, direct == 0` is a yellow that nothing else depends on — finishing it doesn't open new work. Worth doing for completeness, but not the "high-leverage" answer.
- Cross-segment cascades are valuable signal: a `.first` TU that opens `.entry` work (or vice versa) is usually more strategic than a within-segment cluster of the same size, because it unblocks parts of the graph the user wasn't otherwise approaching.

### Presenting the recommendation

The pure count is necessary but not sufficient. Look at the "what each one unblocks" list and call out anything the user would care about:
- Does this TU gate a **named convergence point** (e.g., a render context, a controller system, a base class)? If yes, that's worth more than the raw count suggests.
- Does it sit on the path to a TU the user has mentioned wanting?
- If two TUs tie on count, pick the one with the better downstream surface — strategic > numeric.

Present as a short table (rank, vrom, segment, exports, trans, brief notes), then 2-3 sentences of recommendation. Don't dump the full top-8 prose unless the user asks; the table plus the top pick's rationale is usually enough.

## Marking a TU as done

When the user says "0xNNNN is done" or "Foo is decomped":

1. Find the vrom (the user may say a name like "Possum" — look it up in `symbol_addrs.txt` or in the script's earlier output).
2. Use `Edit` to insert the vrom into `DECOMPILED_VROMS` in `tools/tu_deps.py`. Preserve the existing sorted-by-address order — find the entry just below where the new one belongs and insert before it. Format: `    0x0NNNNN,` (six hex digits, zero-padded, comma).
3. Re-run the leverage script.
4. Present the new top picks.

Do **not** edit `symbol_addrs.txt` from this skill — naming is a separate workflow (see `decomp` skill).

## Cross-TU symbol usage

When the user asks "which symbols from TU 0xNNNN are used by other TUs?" — useful for understanding the public API of a TU, especially right before or right after decompiling it.

```bash
uv run --script - <<'PY'
# /// script
# requires-python = ">=3.11"
# dependencies = ["mapfile-parser"]
# ///
import sys
sys.path.insert(0, 'tools')
from collections import defaultdict
from tu_deps import load_tus, build_graph, short_name, demangle

VROM = 0xNNNN  # <-- set this

tus, segments = load_tus()
deps, sym_to_tu = build_graph(tus)
key_to_seg = {k: name for name, keys in segments for k in keys}
target_key = next(k for k, tu in tus.items() if tu["vrom"] == VROM)
own_syms = set(tus[target_key]["symbols"])

per_sym = defaultdict(lambda: defaultdict(int))
for key, tu in tus.items():
    if key == target_key:
        continue
    for sym in tu["extern_calls"]:
        if sym in own_syms:
            per_sym[sym][key] += 1

print(f"{len(per_sym)} externally-referenced symbol(s) from {short_name(target_key)}:\n")
for sym in sorted(per_sym, key=lambda s: (-len(per_sym[s]), s)):
    callers = per_sym[sym]
    disp, _ = demangle(sym)
    label = sym if disp == sym else f"{sym}  ({disp})"
    print(f"  {label}  — {len(callers)} TU(s)")
    for ck in sorted(callers, key=lambda k: tus[k]["vrom"]):
        seg = key_to_seg.get(ck, "?")
        print(f"      0x{tus[ck]['vrom']:06X}  {short_name(ck):<22} [{seg:<8}]  x{callers[ck]}")
PY
```

Edit the `VROM = 0xNNNN` line before running. Surface call-site count separately from unique-TU count — they answer different questions ("how widely used?" vs "how heavily used by this one caller?").

## Australian animal naming

Classes in this project are named after Australian animals. When the user asks for a fresh name:

1. Pull current names from `symbol_addrs.txt`:
   ```bash
   grep -oE '__[0-9]+[A-Z][a-zA-Z]+' symbol_addrs.txt | sed -E 's/__[0-9]+//' | sort -u
   ```
2. Cross-check against the demangled exports the script reports (some classes show up only as mangled symbols in `symbol_addrs.txt` — the `tu_deps.py` output already demangles).
3. Suggest something unused and recognisable. Bias toward iconic / well-known animals for foundational classes (lots of dependents), more obscure ones for leaf classes.

Past suggestions that have been used: Dugong, Koala. Reasonable unused candidates to draw from when stuck: Emu, Thylacine, Feathertail, Taipan, Cassowary (used), Bilby (used), Bandicoot (used)… check before suggesting.

Don't volunteer a name unless asked — naming is the user's call. When they do ask, offer one primary pick with a sentence on fit, plus 2-3 backups.

## Other segments and "ignored" TUs

The script defaults to looking across **all** segments — that's almost always what the user wants for ranking. If they specifically ask about one segment, run `uv run tools/tu_deps.py <segment>` for the tree view; the leverage script doesn't filter by segment but the output's `seg` column makes filtering visually trivial.

`IGNORED_VROMS` in `tools/tu_deps.py` is for TUs whose decomp status doesn't matter for dependency purposes (libc, libultra, libmus, cpp runtime). Their exports count as decompiled — don't move them into `DECOMPILED_VROMS`. If the user wants to ignore a new TU (e.g., a runtime library), add to `IGNORED_VROMS` instead.

## What this skill does NOT do

- It does not decompile anything. For that, hand off to the `decomp` or `decomp-file` skill.
- It does not edit `symbol_addrs.txt`. Renaming `func_NNNN` to a mangled class symbol is part of decomp, not picking.
- It does not regenerate `expected/build/` and never writes there.
- It does not run `configure.py`. Adding a vrom to `DECOMPILED_VROMS` is a comment-only edit — it changes the picking model, not the build.

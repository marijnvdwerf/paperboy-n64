---
description: Decompile a whole file (segment) in this Paperboy N64 project from top to bottom. Use when asked to decompile a .cpp/.c file end-to-end, "do the whole file", convert all of <file> from INCLUDE_ASM to C/C++, or migrate a segment. Complements the single-function `decomp` skill. Triggers on "decompile <file>", "do all of", "go through the file", "convert the file", "match the file", or when the task scope is the whole TU rather than a single function.
---

# Decompiling Whole Files

For one-off function work, use the `decomp` skill (`asm/nonmatchings/<seg>/<name>.s` → C, build, diff, iterate). This skill is for the *file*-level workflow: tens of functions, a class or two, cross-TU symbols, vtables, rodata, the works. The mistakes that hurt at this scale are different from the per-function ones, so the process is different.

The single most important rule: **the structure of the file (class shape, data members, who's a method vs. a free function) is determined by the data, not by guessing**. Inventory first, write later.

## Phases at a glance

| Phase | Goal | Output |
|------|------|--------|
| 0. Inventory | Understand what the file actually contains | A one-page plan |
| 1. Skeleton | Compile to a byte-identical `.o`, all bodies via `INCLUDE_ASM` | Structure verified |
| 2. Convert | Replace `INCLUDE_ASM` with real C/C++ bodies, one at a time | Most functions matching |
| 3. Revisit parked | Use accumulated context to unstick earlier blockers | Fewer (or zero) parked fns |
| 4. Cleanup | Types, casts, names — only after matching | Readable file |
| 5. Cross-TU | Propagate renames; full-rom checksum passes | Both NTSC and PAL match |

---

## Phase 0 — Inventory (read-only)

Before touching the source file, gather facts. **Every later choice depends on what you find here**; guessing the class shape costs a rewrite per wrong guess.

1. **List the asm dir**: `ls asm/nonmatchings/<segment>/`. Categorise each symbol:
   - `func_NNNN.s` — unmangled. Either a free helper *or* an as-yet-unnamed class method (the vtable will tell you which).
   - `<name>__<N><Class>...s` — cfront-mangled method. Confirms class name and gives you one method.
   - `_._<N><Class>.s` and `__<N><Class>.s` — dtor / ctor. **Confirms the class exists and gives you the class name.**
   - `D_NNNN.s` — rodata. The one in the ctor's `lui/addiu` pair is the vtable.

2. **Read the vtable rodata**. It's the class's schema. Each entry is `{s16 this_offset, s16 pad, void* fn}` = 8 bytes. Slot 0 is the cfront-null pad; real virtuals start at slot 1. The dtor's slot tells you how many virtuals are declared *before* `~Class()`. Once you know the vtable layout, you know:
   - Every virtual function in declaration order
   - Which `func_NNNN.s` are virtual members (those listed) vs. free helpers (those not)
   - Where the dtor goes in the declaration

3. **Read the ctor asm** (`__<N><Class>.s`). The stores to `this->+N` tell you the data members: `sb` = 1-byte, `sw` = 4-byte, an immediate value gives you the field's initial value. The `lui/addiu` pair sets the vptr — confirm it's at the offset you expect (typically after all data members of the class declaring the virtuals).

4. **Read the dtor asm** (`_._<N><Class>.s`). Reverse-confirms the data members and shows you which other helpers the class needs (the dtor body's `jal` targets are usually "reset" helpers).

5. **Check `include/structs.h`**. If a class with the same ctor/dtor name is already declared there, that's the existing source of truth. *Don't duplicate it locally* — extend or replace, but explicitly. *Why*: cross-TU users see this declaration; if you redeclare locally with a different layout the linker won't catch it but runtime will.

6. **Grep cross-TU references** for every symbol you might rename:
   ```bash
   grep -rn "func_NNNN\|<other symbols>" src/ include/
   ```
   *Why*: when you rename `func_8004556C` to `StructZZ::open`, every other TU that calls the old name breaks the link. Find these now, not after the linker yells.

7. **Check `symbol_addrs.txt`** for any pre-existing entries in this file's address range. Honor them. They often encode prior decisions (e.g. a method has already been mangled and the user expects it that way).

8. **Find a reference implementation if one exists** (e.g. similar N64 game's decomp). Treat as a *structural hint*, not source-of-truth. *Why*: different compiler (LEGO Racers uses IDO, we use SN cfront), different codebase. The class shape is usually right; the exact code shape rarely is.

**Inventory output**: a short note (in your head or in a TaskCreate) that lists: class name, data members with offsets, vtable slot → function map, free helpers, cross-TU callers, and any pre-existing symbol_addrs entries.

---

## Phase 1 — Skeleton (matching state, no bodies yet)

Goal: a `.cpp` with the correct class declarations and *every* function still going through `INCLUDE_ASM`. The `.o` should be byte-identical to expected.

1. **Write the class declaration(s)**. Data members from Phase 0 step 3, virtuals in vtable order (from step 2, with the dtor at its correct slot), non-virtual methods and free helpers identified from `__<N><Class>` mangling.

   *Why this first*: every function body you write will reference these types. Getting them right means later edits are local; getting them wrong means everything re-cascades.

2. **`INCLUDE_ASM` every function** using the *exact symbol* the asm file is named after. For mangled methods, that's the cfront name. For unmangled helpers, it's `func_NNNN`.

3. **Update `symbol_addrs.txt`** with any mangled entries the asm dir already contains (`__<N><Class>`, `_._<N><Class>`, `vfunc<N>__<N><Class>`, etc.). Run `uv run tools/format_symbol_addrs.py` to re-sort.

4. **`uv run python configure.py --clean`**. This re-runs splat and regenerates asm files for any new mangled names you've referenced. **Always invoke `configure.py` with `--clean`, never plain** (project rule, in CLAUDE.md).

5. **Build the single file**:
   ```bash
   ninja build/ntsc/src/<file>.o
   ```
   *Don't run plain `ninja`* — relinking the whole ROM and checksumming it is irrelevant during per-function work and slow. (CLAUDE.md rule.)

6. **Spot-check one function** with `uv run asm-differ -o <symbol>`. It should be byte-identical because everything still goes through `INCLUDE_ASM`. If it isn't, your class layout is wrong somewhere — fix that before moving on.

*Why this whole phase*: you've committed to the *structure* without committing to any *body*. If the structure is wrong you find out from the linker and asm-differ now, not after writing 15 function bodies that all silently assume the wrong layout.

---

## Phase 2 — Convert function-by-function

Now flip `INCLUDE_ASM` lines to real C/C++ bodies, one at a time, building and diffing after each.

### Source order is mandatory; attempt order is yours

**The C/C++ source must appear in address order**, because the linker emits `.text` in source order and asm-differ aligns by offset. If you write `func_C` above `func_A` in the source, every diff after that point shifts and asm-differ becomes useless. So: top to bottom in the file is fixed.

**But which `INCLUDE_ASM` you flip next is up to you, and easy-first is right**:
- Trivial getters/setters/passthroughs first. They're 1–4 instructions, almost impossible to get wrong, and each one *validates* a data-member offset or a vtable slot. A passing 3-line `return self->curFile;` confirms `curFile` is at the right offset and the class is the right size. Without that confirmation, every 200-line decomp you write later is built on assumption.
- Save the 200-line beasts for after you have a feel for the original programmer's style (loop shapes, variable naming, where they cache vs. re-read, branch direction preferences).

### The inner loop, per function

1. Read the full asm.
2. m2c draft for control flow scaffolding (with rodata if the function has a jump table or float consts):
   ```bash
   uv run m2c asm/nonmatchings/<seg>/<func>.s asm/data/<rodata>.s
   ```
3. Translate to C/C++. **Replace the `INCLUDE_ASM` line; don't add the body alongside.**
4. `ninja build/ntsc/src/<file>.o`
5. `uv run asm-differ -o <symbol>`
   - `<` and `>` markers in the side-by-side = real code adds/removes; you have work to do.
   - `r` markers = register-only diff with same code shape; same instructions, different register choice. Sometimes acceptable, sometimes a sign your variable ordering or type is slightly off.
   - `i` markers = symbol substitution (e.g. `D_80004898` vs `_vt.8StructZZ`). Code is identical, only the referenced symbol name differs. Usually fine.
6. Iterate.

### The budget rule

Set a per-function iteration budget (~5 attempts). If you're still diff'ing after that, **park the function**:

```cpp
#if 0
// <one-sentence note on what blocks: e.g. "1-instruction delay-slot reorder on chunkSize ternary">
<your decomp>
#else
INCLUDE_ASM("asm/nonmatchings/<seg>", <symbol>);
#endif
```

Then move on. *Why a budget*: the long-tail of micro-diffs (one delay slot, one reg-alloc choice) eats hours and produces nothing. The decomp you wrote isn't wasted — it's preserved next to the asm reference for the revisit phase.

*Why `#if 0` and not `NON_MATCHING` blanket*: explicit per-function gives you precise control and a place for the "what blocks" note. A file-wide `NON_MATCHING` macro hides which functions are actually working.

### Don't lose matching state

Every commit / stopping point should leave the file building and matching for whatever's already passed. If you're about to make a structural change (rename a method, change a struct member type), the prior matches will likely break. Re-verify after the change, don't assume.

---

## Phase 3 — Revisit parked functions

Once the easy/medium functions are in, you have:
- A class layout that's been corroborated by a dozen successful matches
- A feel for the original programmer's preferences (loop forms, init patterns, where they hoist locals)
- Sometimes, a method on the class you can now call from the parked body, which changes codegen

Retry each parked function. **Often the same C code that didn't match in Phase 2 matches now**, because some related class/type detail you got slightly wrong has since been corrected. If it still doesn't match, the per-function `decomp` skill's SN codegen tips section is the right reference — that's where the truly small/stubborn diffs live.

*Why revisit instead of pushing through*: trying to brute-force a 200-line function against asm without context-validated types is the most expensive way to spend iterations.

---

## Phase 4 — Cleanup pass (only after matching)

Now make the file readable. **One change at a time, rebuild + asm-differ after each** — every rename or retype risks regressing a match.

- **Casts**: `grep -nE "\((u?[0-9]+|s[0-9]+|void)\*?\)" src/<file>.cpp`. Each cast is a signal that two types disagree. Often you can delete a cast by changing the *underlying type* (e.g. `int* bytesRead` → `s32* bytesRead` on a virtual method, or removing `(u8*)` on a `delete[]` once the pointer is already the right type).
- **Numeric types**: 1-byte fields → `u8`, counts/capacities → `u32`, offsets/return-codes → `s32`. Project convention; also avoids spurious `(u32)` casts at compare sites.
- **Pointer math**: replace ad-hoc `(u8*)x + N` with proper field access or array indexing. If you see `(SomeStruct*)((u8*)p + 0x10)`, that's a sign your struct is missing a field at that offset.
- **Names inside bodies**: rename `func_NNNN` *callers* inside function bodies to semantic names where you have evidence. File-level symbols (top-level `extern "C" void func_NNNN(...)`) stay as-is until you have enough evidence to commit to a name across the whole codebase.

*Why this phase is last*: each rename risks breaking a match. Doing it after matching means each change is a one-line, one-rebuild check, not a multi-variable mystery.

---

## Phase 5 — Cross-TU propagation

If you renamed any externally-visible symbol (a method that was an `extern "C" func_NNNN` and is now `Class::method`):

1. **Update `include/structs.h`** with the new method declarations. Confirm data layout still agrees with the local one in your `.cpp`.
2. **Update each caller TU** from the grep list you made in Phase 0 step 6. `func_NNNN(obj, arg)` becomes `obj->method(arg)`.
3. **Full `ninja` build**:
   ```bash
   ninja
   sha1sum -c checksum.sha1
   ```
   Both NTSC and PAL must produce `build/<ver>/paperboy.z64: OK`. (PAL build uses `-DPAL=1`; PAL-only code lives in `#ifdef PAL`.)

*Why last*: until your TU itself matches, full link is noise. Once it does, this phase catches anything you forgot to update elsewhere.

---

## Anti-patterns (things I've watched go wrong)

- **Write the whole file in one go.** Every assumption baked in simultaneously; one wrong guess re-cascades through everything.
- **`NON_MATCHING` as a blanket macro.** Hides which functions are actually working. Use per-function `#if 0 / #else INCLUDE_ASM / #endif` instead.
- **Top-to-bottom *conversion* order**: top-to-bottom is the rule for *source order in the file*, not for which `INCLUDE_ASM` to flip next. Easy-first for flips is right.
- **Skip cross-TU grep before renaming.** The linker will tell you eventually, but it's a longer round-trip than `grep` would have been.
- **Guess cfront mangling.** Compile once, run `mips-linux-gnu-nm build/ntsc/src/<file>.o` to read the actual symbol names off the `.o`. Five seconds, no guessing.
- **Treat reference implementation as source-of-truth.** Different compiler emits different code. Reference is a *structural* hint at the class-shape level — that's all.
- **Eager parking without iteration**, OR **endless iteration without parking.** Both waste effort. Honour the budget.
- **Re-read with `sed`/`head` instead of `Read`.** Use the Read tool — file context matters and is cheap.
- **Run plain `ninja` during per-function work.** Slow and the checksum-fail noise is irrelevant. `ninja build/ntsc/src/<file>.o` is the right command until Phase 5.

---

## Paperboy-specific tooling

| Action | Command |
|--------|---------|
| Build single file | `ninja build/ntsc/src/<file>.o` |
| Build full rom | `ninja` (only in Phase 5) |
| Diff a function | `uv run asm-differ -o <symbol>` |
| Read symbol table from `.o` | `mips-linux-gnu-nm build/ntsc/src/<file>.o` |
| m2c draft | `uv run m2c asm/nonmatchings/<seg>/<func>.s asm/data/<rodata>.s` |
| Reconfigure (after YAML or new INCLUDE_ASM symbols) | `uv run python configure.py --clean` |
| Re-sort symbol_addrs.txt | `uv run tools/format_symbol_addrs.py` |
| Identify libultra | `bunx ultra64 identify baserom.z64 --base 0x80000400` |
| Checksum | `sha1sum -c checksum.sha1` |

**Critical files:**
- `paperboy.yaml` — splat segment config. Modify carefully; reconfigure with `--clean` after.
- `symbol_addrs.txt` — single list ordered by address. No blank lines. Entries: `name = 0xADDR; // type:func` (or no comment for non-funcs). Re-sort after edits.
- `include/structs.h` — cross-TU class declarations. Single-TU classes stay inline in the `.cpp`.
- `expected/build/` — user-owned reference build. **Never write to it.** `asm-differ` uses it as the TARGET side.
- `CLAUDE.md` — project-wide rules. Always check.

**File-editing rules (CLAUDE.md):**
- Use the `Edit` tool, not `sed`/`perl`/`awk` — they mangle whitespace.
- Don't run `ninja` plain during per-function work.
- Don't write to `expected/`.
- Don't run `configure.py` without `--clean`.

---

## When to ask the user, when to assume

**Ask up front, before Phase 1**, when:
- The class has both pre-existing `extern "C" func_NNNN` users *and* a clear `__<N><Class>` mangled ctor/dtor — they have a stylistic preference (free functions vs. C++ methods) that affects every line you'll write.
- A class already exists in `structs.h` with a different layout than what your inventory suggests — you need to know whether to extend, replace, or duplicate.
- The vtable rodata references symbols you haven't decided naming for (will they be `vfunc1`, `locate`, `func_NNNN`?). The naming convention affects mangling and symbol_addrs.txt.
- The reference implementation you found suggests a name that you're not sure they want adopted (their codebase, their preference).

**Don't ask, make a reasonable assumption and continue**, when:
- You're in the middle of Phase 2 (per-function conversion) and have a question about a single function. State the assumption in a comment, ship the function, move on. Easier to correct later than to context-switch.
- You're choosing between two C formulations that both might match. Just try one — asm-differ tells you in 5 seconds.

**Ask after the first draft is done**, when:
- You've parked some functions and want to know whether to keep iterating or accept the parked state.
- You've made naming choices that affect cross-TU callers and want sign-off before propagating.
- The cleanup phase has multiple plausible directions (e.g. multiple ways to name a method).

*Why this split*: asking interrupts flow and round-trips cost real time. Up-front questions prevent rewrites of work-not-yet-done, which is high leverage. Mid-flow questions interrupt work that's already moving, which is low leverage and often resolvable by the user just looking at the result.

---

## A note on data members vs. casts

When you find yourself adding a cast inside a function body, **suspect the type declaration first, not the function body**. The cast is usually compensating for a wrong field type or a wrong parameter type.

Concrete example: an asm read at `lh` (load halfword) suggests the field is `s16`. If your struct declares it as `s32`, every site that reads it will need `(s16)` casts. Fix the struct, not the casts.

Same rule for virtual function signatures: the signature in your `Class` declaration should be the *most accurate* possible type. If the virtual takes a `void*` everywhere, even passing it a `Foo*` adds a cast. Better signature, fewer casts everywhere.

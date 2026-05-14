---
description: Decompile a whole file (segment) in this Paperboy N64 project from top to bottom. Use when asked to decompile a .cpp/.c file end-to-end, "do the whole file", convert all of <file> from INCLUDE_ASM to C/C++, or migrate a segment. Complements the single-function `decomp` skill. Triggers on "decompile <file>", "do all of", "go through the file", "convert the file", "match the file", or when the task scope is the whole TU rather than a single function.
---

# Decompiling a Whole File

The goal: hand this skill a `.cpp` segment file full of `INCLUDE_ASM` lines, and walk away. By the time you finish, every function is either matched in C/C++ or parked in a `#ifdef NON_MATCHING / #else INCLUDE_ASM / #endif` block, the file builds cleanly into a `.o`, and the user comes back to a reviewable result.

The work is dominated by one inner loop. Most of this skill is about how to run that loop. Setup is brief; afterwards is "done".

---

## Three invariants

These hold for every function you touch. Internalize them before starting.

**1. The unit of work is one function. No batching, ever.** Read its asm, write its body, build, diff, iterate or park, then move to the next. Build and diff after every single function — the loop is the unit of progress, not the file. Concretely:
- Never write two function bodies before building.
- Never run `asm-differ` across multiple symbols in one command (no `for sym in a b c; do uv run asm-differ -o $sym; done`-style loops, no scripts that diff a set). One symbol at a time, every time.
- Never queue up changes across functions to verify in bulk.

Batching hides which change caused which diff and turns a tractable iteration into a guessing game.

**2. Source order is fixed; walk the file top to bottom.** asm-differ aligns functions by their position in the original map file. If you write a function out of order, every diff downstream shifts and lies. The first `INCLUDE_ASM` you replace is the topmost one, the second is the next one down, always.

**3. The asm filename dictates the symbol your C/C++ must compile to. The asm filenames are fixed.** asm-differ anchors on the symbol name, and `expected/` was built against the current names. Two cases:
- The `.s` file is `func_NNNN.s` → write `extern "C" ReturnType func_NNNN(...) { ... }`. The `extern "C"` suppresses C++ mangling so the compiled symbol stays `func_NNNN`.
- The `.s` file is cfront-mangled (`open__10JamArchive...s`, `__10JamArchive.s`, `_._10JamArchive.s`) → write a normal C++ member / ctor / dtor. The compiler mangles it back to the matching symbol.

Do **not** edit `symbol_addrs.txt` or run `configure.py --clean` to rename anything during this skill — *not even for an obvious ctor/dtor*. Reconfiguring regenerates asm filenames in the current tree while `expected/` keeps the old names, so asm-differ has nothing to anchor to and the diff breaks. cfront-style mangling is also fiddly enough that the name you'd pick is unlikely to match what cfront actually emits without iteration. Renaming `func_NNNN` to a mangled symbol is a separate later phase, not part of this skill. The asm filenames are whatever they are; you write C/C++ to match them.

---

## Scope

**In**: writing C/C++ for each function in this one `.cpp` segment, getting the `.o` to match asm-differ.

**Out**: cross-TU work, full ROM links, defining vtable data symbols (leave vtables as `extern`), preserving rodata symbol names from `.s` (inline the values as C literals — see loop step 3), renaming externally-visible symbols of other TUs, propagating types into `include/structs.h`, editing `symbol_addrs.txt`, running `configure.py`.

All types and struct declarations stay local to this `.cpp`. External functions and variables get `extern` (or `extern "C"`) declarations at the top of this file.

---

## Setup

A short read of the asm to understand the class shape, then one build to confirm the skeleton compiles.

### 1. Inventory the asm directory

`ls asm/nonmatchings/<segment>/`. The list, in alphanumeric order, is the file's full set of functions (alphanumeric = address order). This is your work queue.

The filenames tell you what symbol each function compiles to:
- `__<N><Class>.s` — ctor of `Class` (write as C++ ctor)
- `_._<N><Class>.s` — dtor (write as C++ dtor)
- `<method>__<N><Class>...s` — method (write as C++ member function)
- `func_NNNN.s` — write as `extern "C" func_NNNN(...)`, regardless of whether it's conceptually a free helper or a method that just hasn't been mangled. The filename is the constraint.

### 2. Identify the class shape

You need data-member offsets/types and the vtable layout so the C++ class declaration is right. Read asm to find them:

- **The ctor** (whatever its filename): a function that loads a rodata address via `lui/addiu` and stores it to `this->+N` (the vptr slot). It usually stores immediate constants to other `this->+M` offsets first. Those stores reveal the data members: `sb` = 1-byte field, `sw` = 4-byte field, the immediate value = the field's initial value. The vptr store comes last.
- **The vtable**: follow the ctor's `lui/addiu` into rodata. Each entry is `{s16 this_offset, s16 pad, void* fn}` = 8 bytes. Slot 0 is the cfront pad; real virtuals start at slot 1, in declaration order.
- **The dtor**: called from `delete` patterns; takes `this` as its only `a0`. Its slot in the vtable tells you where it sits in the declared order.

End of this step you should have: the class name (or a working name), data members with offsets and types, and the vtable in declaration order.

How much of this you express in C++ depends on the asm filenames you have to match:

- **All methods cfront-mangled** → write a full C++ class with `virtual` keywords matching the vtable, ctor/dtor as C++ ctor/dtor.
- **Some methods still `func_NNNN.s`** → write a plain struct (data members only). Define each function as `extern "C" ReturnType func_NNNN(Class* this, ...)` with the receiver as an explicit first arg. Don't try to express the vtable as C++ virtuals; the mixed-mangling case doesn't map cleanly, and renaming `func_NNNN` to a mangled symbol is out of scope here (see invariant #3).

### 3. (Optional) reference lookup

If you know of a similar class decompiled elsewhere (another game, another project), peek at it for structural hints: field names, virtual signatures. Use as a hint about shape, not as source for code — different compilers emit different code.

### 4. Write the skeleton

In the `.cpp`:
- **Top**: `extern "C"` declarations for every external function/variable the asm references (jal targets, data referenced via `lui/addiu`). For rodata symbols and vtables, `extern` is enough — you're not defining them.
- **Then**: local struct/class declarations. Data members in offset order. Virtuals declared with the C++ `virtual` keyword, in vtable order. Mangled methods declared as plain C++ members. (For virtuals whose asm filename is still `func_NNNN`, you'll define them as `extern "C"` outside the class and the vtable entry's referenced symbol matches — see invariant #3.)
- **Then**: one `INCLUDE_ASM("asm/nonmatchings/<seg>", <symbol>);` for every function in the asm directory, in source order. `<symbol>` is the asm filename without extension.

### 5. Confirm the skeleton

```bash
ninja build/ntsc/src/<file>.o
```

Spot-check one function with `uv run asm-differ -o <symbol>`. Every body is still asm, so every diff should be clean. If a diff shows up against an `INCLUDE_ASM` line, your class layout is wrong — fix the declaration before entering the loop.

---

## The loop

You're at the top of the file, looking at the first `INCLUDE_ASM` line. Walking top to bottom, for each `INCLUDE_ASM`:

```
read asm  →  draft body  →  replace INCLUDE_ASM  →  build  →  diff
                                                              ├─ matches?    → next function
                                                              ├─ retry (≤5)  → adjust C, build, diff again
                                                              └─ budget out  → park → next function
```

### Per-function steps

1. **Read the function's full asm.** Every instruction.

2. **(Optional) m2c draft** for control-flow scaffolding:
   ```bash
   uv run m2c asm/nonmatchings/<seg>/<func>.s
   ```
   Pass the rodata file too if the function has a jump table or float consts. m2c gives you a starting point, not a finished translation.

3. **Replace the `INCLUDE_ASM` line with the body.** Apply invariant #3's mangling rule:
   - `.s` is `func_NNNN.s` → `extern "C" ReturnType func_NNNN(...) { ... }`
   - `.s` is mangled → matching C++ member function / ctor / dtor (no `extern "C"`)

   Use C++ forms: `new`/`new[]`, `delete`/`delete[]`, member-function calls. Locally-declared types and casts stay inside this `.cpp`.

   **No `goto`/labels in the C source.** If you find yourself wanting to add `goto check;` or similar to mimic the compiler's emitted control flow shape, stop and park the function instead. Readable non-matching C beats goto-spaghetti that matches.

   **Rodata referenced from the function body — inline the literal, don't preserve the symbol.** If the asm references `D_NNNN` for a string, float constant, or similar value, look up its content (in the function's own `.s` via `.section .rdata`, or in a separate `asm/data/*.rodata.s` file) and write the literal in C. Example: an asm reference to `D_80000CE0` whose rodata is `"."` becomes `strcat(dst, ".")` in C, not `strcat(dst, D_80000CE0)`. The compiler re-emits the literal into rodata at the right address. Skip `INCLUDE_RODATA` entirely — it's not needed for this workflow.

4. **Build:**
   ```bash
   ninja build/ntsc/src/<file>.o
   ```
   Compile errors block; fix them. (Single-file build produces only the `.o`, so no link errors at this stage.)

5. **Diff:**
   ```bash
   uv run asm-differ -o <symbol>
   ```
   `<symbol>` is the asm filename without extension.
   - `<` / `>` markers — real code differences. Adjust the C, retry.
   - `r` markers — same instructions, different register allocation. Sometimes a sign that variable ordering or a type is slightly off; sometimes the compiler's register choices have no straightforward C-source lever. If obvious variants don't move it within budget, park.
   - `i` markers — symbol-name substitution (e.g. `D_80004898` vs `_vt.8JamArchive`). Usually fine.

   Read the markers, not the score. asm-differ's numeric score also counts label-name changes (e.g. `.L_func_800100C0_4C` vs `.L__7Class_4C` after a rename), which aren't real diffs. If the actual instructions match and only label/symbol references differ, treat it as a match regardless of the score.

6. **Iterate, budget ~5 attempts.** If still diffing after that, park:
   ```cpp
   #ifdef NON_MATCHING
   ReturnType func_or_method(...) {
       // your draft
   }
   #else
   INCLUDE_ASM("asm/nonmatchings/<seg>", <symbol>);
   #endif
   ```
   One short comment above the `#ifdef` describing what blocked, for the future re-attempt ("delay-slot diff on chunk-size ternary"; "reg-alloc differs on the third store"). Then move on.

7. **Move to the next `INCLUDE_ASM` line.** Leave everything above untouched.

### Why this discipline

- **Build-and-diff per function**: when a diff shows up, you know exactly which line caused it. Two bodies between builds means two suspects.
- **Top-to-bottom source order**: keeps asm-differ's offset alignment honest, so the diff for function 12 is about function 12, not about a reorder upstream.
- **The loop stays narrow**: type renames, signature tweaks, and helper extractions happen *inside* the iteration for the function that needs them, not as a separate "cleanup" between functions. If a type turns out wrong, the next function to use it surfaces that; fix it there, in its own loop iteration, and continue downward.

### Parking is checkpointing, not failure

A `#ifdef NON_MATCHING / #else INCLUDE_ASM / #endif` block still emits the asm in the build (because `NON_MATCHING` is undefined), so the `.o` continues to match. Your C draft is preserved next to the asm reference for a later re-attempt with more class/context info accumulated. The loop keeps moving; the file keeps matching. That's the deal.

---

## Done condition

Every function in `asm/nonmatchings/<seg>/` has either:
- a real C/C++ body in the `.cpp` (no `INCLUDE_ASM` line) that matches under asm-differ, or
- a `#ifdef NON_MATCHING / #else INCLUDE_ASM / #endif` block with a one-line note.

`ninja build/ntsc/src/<file>.o` succeeds. That's the deliverable; the user can review on return.

---

## Paperboy commands

| Action | Command |
|--------|---------|
| Build this file only | `ninja build/ntsc/src/<file>.o` |
| Diff a function | `uv run asm-differ -o <symbol>` |
| m2c draft | `uv run m2c asm/nonmatchings/<seg>/<func>.s [asm/data/<rodata>.s]` |
| Read .o symbols (for reference) | `mips-linux-gnu-nm build/ntsc/src/<file>.o` |

**Tool rules** (from project CLAUDE.md):
- Use the `Edit` tool, not `sed`/`awk`/`perl` (they mangle whitespace).
- `expected/` is the user's reference build, used as the TARGET side by asm-differ — read-only.
- Single-file `ninja build/ntsc/src/<file>.o` is the right build command for this workflow.
- Don't run `configure.py` during this workflow (see invariant #3).
- Don't edit `symbol_addrs.txt` during this workflow.

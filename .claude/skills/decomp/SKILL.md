---
description: Decompile functions in this Paperboy N64 project. Use when asked to decompile, diff, or match a function. Triggers on "decompile", "decomp", "diff", "match", "m2c", function names like "func_800XXXXX".
---

# Decompiling Functions

See `DECOMPILING.md` at the repo root for the long-form guide. This is the operational cheat sheet.

## The loop

1. Read `asm/nonmatchings/<segment>/<name>.s`.
2. Get a draft with m2c (always pass relevant rodata, even with no jump table — resolves float consts):
   ```bash
   uv run m2c asm/nonmatchings/<file>/<func>.s asm/data/<rodata>.s [more.rodata.s]
   ```
3. Replace the `INCLUDE_ASM(...)` line in the corresponding `src/*.{c,cpp}` with your C/C++.
4. `ninja` — this builds *and* runs `sha1sum -c checksum.sha1`. `build/ntsc/paperboy.z64: OK` is the only authoritative match signal. Both NTSC and PAL must pass.
5. If not OK, iterate with `uv run asm-differ -o <func_name>` (we do NOT use asm-differ's auto-build).

**asm-differ score is a heuristic.** Score 0 doesn't guarantee a ROM match (rodata/bss may differ); non-zero with only `i`-marked symbol substitutions usually does match. Trust the sha1sum.

To park a not-yet-matching draft without blocking, wrap it: `#if 0 ... your C ... #else INCLUDE_ASM(...); #endif` (or `#if 1` to keep the C active while iterating).

## Toolchain facts

- C: `cc1n64.exe` (`-lang-c -D_LANGUAGE_C`). C++: `cc1pln64.exe` (`-lang-c++ -D_LANGUAGE_C_PLUS_PLUS`). Both via `wibo`.
- Flags: `-O2 -G0`.
- PAL build is compiled with `-DPAL=1`; PAL-only code in `#ifdef PAL`; PAL-only symbols in `undefined_syms_pal.txt`, shared in `undefined_syms.txt`.
- C functions called from asm need `extern "C"` in C++ TUs to avoid mangling.

## C++ patterns (critical)

We model the original as C++ classes — don't flatten to struct-of-function-pointer.

**Vtable position.** SN places the vptr **after** all data members of the class declaring the virtuals. For data both before and after the vptr, use inheritance: parent holds early data + at least one virtual (forces vtable at its tail); derived class adds later data + more virtuals.

**Dtor slot = declaration order.** Virtual destructors do NOT auto-go to slot 0/1. If the rodata vtable has the dtor at slot 7, declare 6 other virtuals before `~Class()`.

**Dtor delete-mode arg in `$a1`.** `2` = run dtor, don't free (stack scope exit). `3` = run dtor + free (`delete obj`). Seeing `jal _._Class; li $a1, 2|3` is normal — just declare the dtor, the compiler handles it.

**Static devirtualization.** A virtual call on an object with statically-known dynamic type (e.g. stack local) compiles to a direct `jal`. On a pointer/ref, it's the usual `lw vptr; lh off; lw fn; jalr`. **Match the asm:** stack local for direct, pointer for indirect.

**Operator new/delete mappings** (already in `symbol_addrs.txt`):
```
__builtin_new        = 0x8004B414   // operator new
__builtin_delete     = 0x8004B3F0   // operator delete
__builtin_vec_new    = 0x80064320   // operator new[]   (project custom heap)
__builtin_vec_delete = 0x80064340   // operator delete[]
```
Use `new u8[N]` / `delete[] ptr` for the `vec` variants.

**Vtable entry layout:** 8 bytes each = `{s16 this_offset, s16 pad, void* fn}`. With 1-indexed `vfuncNN`, slot K is at byte offset `K*8`.

## Name mangling (SN cfront-style)

| Declaration | Mangled |
|---|---|
| `Class::method()` | `method__<N>Class` |
| `Class::method(Other*)` | `method__<N>ClassP<M>Other` |
| `Class::method(s32)` | `method__<N>Classl` |
| `~Class()` | `_._<N>Class` |
| `Class()` ctor | `__<N>Class` |
| Vtable | `_vt.<N>Class` |

`<N>` = strlen(class name). Primitives: `c` char, `i` int, `l` long/s32, `Pv` void*, `PCc` const char*. Return type NOT mangled. `T<n>` reuses arg `n`'s type (`lPvlT2` = `(long, void*, long, void*)`).

## Adding/renaming a class method — workflow

Splat scans `.c`/`.cpp` files for `INCLUDE_ASM` macros to decide which `.s` files to emit. **Update INCLUDE_ASM in source BEFORE reconfiguring**, or splat will delete the old `func_NNNN.s` and the build breaks.

1. Declare the method in the header (in `include/structs.h` if cross-TU, else inline in the .cpp).
2. Update the source: either provide a definition, or change the `INCLUDE_ASM(..., func_NNNN)` line to reference the new mangled name.
3. Add `<mangled> = 0xADDR; // type:func` to `symbol_addrs.txt`.
4. `uv run python configure.py --clean` to reconfigure.
5. `ninja`.

Reconfigure deliberately — it can wipe in-progress asm. Don't reconfigure casually.

## Link failures

`defined in discarded section` usually means a vtable references unresolved virtuals. Either map every declared virtual to a real address in `symbol_addrs.txt`, or remove the virtual decls until things resolve.

## Splitting TUs

A single asm range can be two TUs glued by the linker. Symptom: a literal (e.g. `""` crash msg) appears at two different addresses — the SN compiler would have deduplicated within a TU. To split:

1. Find an aligned address between the TUs.
2. Add `- [0xNNNN, cpp]` (and a matching `rodata` entry if needed) to `paperboy.yaml`.
3. Reconfigure. Move source for the new range into the new `.cpp`. Move shared types to `include/structs.h`.

## Header / style conventions

- `include/structs.h` for cross-TU class defs; single-TU classes stay inline.
- Field offset comments (`/* 0x18 */`) are mandatory — the offset is truth, not the name.
- Field names start `unkXX`; rename only with evidence. No opportunistic renaming.
- `clang-format` before commit.

## SN codegen tips (when "structurally right" doesn't match)

- **Instruction scheduling**: SN fills delay slots aggressively. Reorder C statements, hoist locals, swap `while`/`for`/`do-while`, combine NULL checks with `&&` vs split `if`s.
- **Don't cache struct fields in locals** across function calls or inside loops — caller-saved regs get clobbered and a local burns an s-register, growing the frame. Use `obj->child->method()` directly.
- **Stack local declaration order**: SN allocates locals so the LAST-declared gets the LOWEST offset. If `sp10` should be at `sp+0x10` and `sp40` at `sp+0x40`, declare `sp40` first, `sp10` last.
- **Switch case order**: emit order = source order; rodata jumptable is target order. Reorder cases to match.
- **Float arrays**: assign literals in sequential index order (0,1,2,...) — rodata is emitted in source order.
- **Rodata migration**: when decomping a function with a jump table or inline float consts, extend the `.rdata` subsegment in the yaml to cover them and adjust the next rodata start.
- **Branch-likely (`beqzl`/`bnel`)**: induced by where vars are initialized relative to the `if`/loop. Try moving init lines.
- **Virtual with mixed arg types across call sites**: declare param as `void*`.
- **`u32` for count/capacity fields**: avoids spurious `(u32)` casts in compares.

## Tools

- `uv run m2c asm/nonmatchings/<f>.s asm/data/<rodata>.s` — starting draft.
- `uv run asm-differ -o <func>` — diff after building. `i` = symbol substitution, `r` = register diff.
- `bunx ultra64 identify baserom.z64 --base 0x80000400` — identify libultra functions. Use **baserom**, not the built ROM. Game vram base is `0x80000400`. Known: `osRecvMesg = 0x8005CEB0`.

## Project glossary

- `libmus` (Software Creations audio): `MusInitialize`, `MusStop`, `MusAsk`, `musConfig` (0x44 bytes).
- Audio scheduler msgs: `OS_SC_RETRACE_MSG`=1, `OS_SC_PRE_NMI_MSG`=4, custom `0x63`.
- `D_80003EB8` = the empty `""` crash string; appearance at two addresses signals two TUs.
- `func_800079A8(msg, 0, 0, 0)` is the crash/assert helper.

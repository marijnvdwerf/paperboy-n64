---
description: Decompile functions in this Paperboy N64 project. Use when asked to decompile, diff, or match a function. Triggers on "decompile", "decomp", "diff", "match", "m2c", function names like "func_800XXXXX".
---

# Decompiling Functions

## Getting a starting point with m2c

To decompile a function, first generate a C approximation from the assembly. Always pass all relevant rodata files to resolve float constants and jump tables:

```bash
uv run m2c asm/nonmatchings/<file>/<func_name>.s asm/data/<rodata1>.s asm/data/<rodata2>.s
```

Example:
```bash
uv run m2c asm/nonmatchings/A49A0/func_800DCA68.s asm/data/7EC08.rodata.s asm/data/79F20.rodata.s
```

m2c output needs manual cleanup — fix types, struct access patterns, and variable names.

## Building and diffing

Always build before diffing. We don't use asm-differ's auto-build.

```bash
ninja && uv run asm-differ -o <func_name>
```

Example:
```bash
ninja && uv run asm-differ -o func_80029900
```

The diff shows TARGET (original ROM) vs CURRENT (your build) side by side.

## Workflow

1. Run m2c on the nonmatching asm to get a starting point
2. Put the decompiled C code in the source file, replacing the `INCLUDE_ASM` line
3. `ninja && uv run asm-differ -o <func_name>` to compare
4. Iterate until the diff shows a match (score 0)
5. Run `ninja` and verify `build/ntsc/paperboy.z64: OK` (checksum passes)

## Project-specific notes

- SN toolchain: C files use `cc1n64.exe`, C++ files use `cc1pln64.exe` (both via wibo)
- Compiler flags: `-G0 -O2`
- C files are compiled with `-lang-c -D_LANGUAGE_C`
- C++ files are compiled with `-lang-c++ -D_LANGUAGE_C_PLUS_PLUS`
- C++ extern functions called from asm need `extern "C"` to prevent name mangling
- C++ member functions need their mangled name in `symbol_addrs.txt` (e.g. `func_80007DE0__8StructXX = 0x80007DE0; // type:func`)
- PAL-specific code is guarded with `#ifdef PAL`
- PAL-only symbols go in `undefined_syms_pal.txt`, shared symbols in `undefined_syms.txt`
- After any YAML changes, always run `uv run python configure.py --clean` before building

## SN compiler behavior

### C++ vtables
- The SN compiler places the vtable pointer AFTER all data members (not at the start like GCC/MSVC)
- The compiler auto-generates a destructor at vtable slot 0, even if no destructor is declared
- Each vtable entry is 8 bytes: `{s16 this_offset, s16 pad, void *func_ptr}`
- With 1-indexed vfunc names (vfunc_01, vfunc_02, ...), vfunc_K is at vtable byte offset K*8. Example: vfunc_31 is at offset 0xF8 (31*8), vfunc_27 is at offset 0xD8 (27*8)
- Virtual function calls in asm look like: load vtable ptr from object, load entry offset+func, adjust `this` by offset, `jalr` the func ptr
- If a struct needs both a vtable at a known offset AND child class data after the vptr, use inheritance: declare virtuals in a base class, then add child data in the derived class. Without inheritance, all data goes before the single vptr, making it impossible to have data at offsets past the vptr.

### Switch statements
- The compiler may emit switch cases in a different order than written in source — reorder cases to match the jump table in rodata
- When decompiling a function with a switch/jump table, the rodata for the jump table must be migrated: add a `.rdata` subsegment in the YAML pointing to the C/C++ file, and remove/replace the old rodata entries
- m2c needs the rodata file passed as a second argument to handle jump tables: `uv run m2c asm/nonmatchings/<file>/<func>.s asm/data/<rodata_file>.s`

### Register allocation and local variables
- Don't store struct field accesses in local variables if the value is only used across function calls. The compiler will reload from the struct after each call (caller-saved regs are clobbered). A local variable forces the compiler to allocate an extra s-register, increasing the stack frame size.
- Example: use `sceneChild->child2->vfunc_18(...)` instead of `GameObjChild2* child2 = sceneChild->child2; child2->vfunc_18(...);`
- When a virtual function accepts different argument types across call sites (pointer in one, integer in another), declare the parameter as `void*`
- Avoid caching struct fields in locals inside loops — the compiler reloads them naturally and the local wastes an s-register. Use `data->children[i]` directly instead of `s32 base = data->childrenOffset; ... base + offset`.
- `u32` types for count/capacity fields avoid unnecessary `(u32)` casts in comparisons

### Stack variable declaration order
- The SN compiler allocates stack locals from the bottom of the frame for the LAST declared variable. First declared = highest stack offset, last declared = lowest offset.
- If asm-differ shows stack offsets are wrong (e.g. sp+0x10 vs sp+0x40), reorder the local array declarations at the top of the function.
- Declare variables in descending stack-offset order: `sp10` last, `sp40` first.

### Loop variables and branch-likely (beqzl)
- Declaring a loop counter inside the `for` init (e.g. `for (u32 i = 0; ...)`) vs before the loop body can affect whether the compiler generates `beqzl` (branch-likely) or `beqz` (regular branch)
- If the target uses `beqzl` and your code generates `beqz`, try moving variable initializations to different positions relative to the if-block

### Float literal arrays
- When assigning float literals to a local array, use sequential index order (0, 1, 2, 3). The SN compiler emits rodata in source order of the literals, and sequential stores match the target better.
- Always pass the rodata file to m2c as a second argument, even when there's no jump table — it resolves float constants to their actual values: `uv run m2c asm/nonmatchings/<file>/<func>.s asm/data/<rodata_file>.s`
- When decompiling a function with inline float constants, the `.rdata` subsegment boundary in the YAML must be extended to cover them, and the following `rodata` subsegment start address must be adjusted

### Instruction scheduling
- The SN compiler aggressively fills branch delay slots — it may reorder stores, address computations, or `move` instructions into delay slots
- Reordering C statements (e.g. moving `var = 0` before or after a function call) can affect which instruction lands in a delay slot
- The compiler CSEs (common subexpression eliminates) `&global` into a register with `lui+addiu`, then uses `move` to pass it. The original code may have recomputed the address each time with `addiu reg, saved_hi, %lo(...)` — this is a known codegen difference that's hard to control from C source

### Identifying library functions
- Use `bunx ultra64 identify baserom.z64 --base 0x80000400` to identify libultra functions in the ROM
- Always use the **baserom**, not the built ROM (which may not match yet)
- The game's base vram is `0x80000400` — pass `--base 0x80000400` for correct addresses
- Known: `osRecvMesg = 0x8005CEB0`

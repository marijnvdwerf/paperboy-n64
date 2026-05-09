---
description: Decompile functions in this Paperboy N64 project. Use when asked to decompile, diff, or match a function. Triggers on "decompile", "decomp", "diff", "match", "m2c", function names like "func_800XXXXX".
---

# Decompiling Functions

## Getting a starting point with m2c

To decompile a function, first generate a C approximation from the assembly:

```bash
uv run m2c asm/nonmatchings/<file>/<func_name>.s
```

Example:
```bash
uv run m2c asm/nonmatchings/2A500/func_80029900.s
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
- So declared virtual functions start at slot 1. If you see a vtable access at byte offset N, the entry index is N/8, and the declared virtual function index is (N/8 - 1) accounting for the destructor
- Virtual function calls in asm look like: load vtable ptr from object, load entry offset+func, adjust `this` by offset, `jalr` the func ptr

### Switch statements
- The compiler may emit switch cases in a different order than written in source — reorder cases to match the jump table in rodata
- When decompiling a function with a switch/jump table, the rodata for the jump table must be migrated: add a `.rdata` subsegment in the YAML pointing to the C/C++ file, and remove/replace the old rodata entries
- m2c needs the rodata file passed as a second argument to handle jump tables: `uv run m2c asm/nonmatchings/<file>/<func>.s asm/data/<rodata_file>.s`

### Instruction scheduling
- The SN compiler aggressively fills branch delay slots — it may reorder stores, address computations, or `move` instructions into delay slots
- Reordering C statements (e.g. moving `var = 0` before or after a function call) can affect which instruction lands in a delay slot
- The compiler CSEs (common subexpression eliminates) `&global` into a register with `lui+addiu`, then uses `move` to pass it. The original code may have recomputed the address each time with `addiu reg, saved_hi, %lo(...)` — this is a known codegen difference that's hard to control from C source

### Identifying library functions
- Use `bunx ultra64 identify baserom.z64 --base 0x80000400` to identify libultra functions in the ROM
- Always use the **baserom**, not the built ROM (which may not match yet)
- The game's base vram is `0x80000400` — pass `--base 0x80000400` for correct addresses
- Known: `osRecvMesg = 0x8005CEB0`

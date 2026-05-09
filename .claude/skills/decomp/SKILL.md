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

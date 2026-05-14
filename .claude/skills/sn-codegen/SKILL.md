---
description: SN compiler (a GCC fork) codegen quirks observed in this Paperboy N64 decomp. Use when an asm-differ run shows a mismatch that isn't a structural issue (wrong type, missing field, off-by-one offset) — these are the small compiler choices the C source has to anticipate to match. Referenced from the `decomp` and `decomp-file` skills. Triggers on "register allocation diff", "reg-alloc", "`r` markers", "memcpy inlined", "why is memcpy/strcpy inlined", "builtin declaration warning", "conflicts with built-in declaration", or any question about why two structurally-equivalent C formulations produce different MIPS.
---

# SN compiler codegen quirks

A running catalogue of small SN-compiler (cc1n64 / cc1pln64 — a GCC fork) behaviours that affect asm-differ matching. When you hit a mismatch that *isn't* explained by structure (wrong field offset, wrong type, missing local), check here for a known pattern.

(Note: the C++ ABI — name mangling, vtable layout — is cfront-style and is documented in `decomp-file`. This skill is about *codegen* behaviour from SN's GCC fork: inlining decisions, register allocation, builtin handling.)

Each tip names: **symptom** (what asm-differ shows, or what the asm looks like), **cause** (the cfront behaviour), **fix** (the source-level lever, or "no lever, park").

---

## memcpy / strcpy inlined unexpectedly

**Symptom**: a small copy in the asm is a sequence of `lwl/lwr/swl/swr` (or `lb/sb` for tiny sizes) instead of `jal memcpy`. Or vice versa — your C produces an inlined copy but the target wants a real call.

**Cause**: the compiler decides whether to inline `memcpy(dst, src, N)` based on the static type of `src` at the call site. `const char*` (or any concrete byte-pointer-ish type) → inlined. `const void*` → real `jal memcpy`.

**Fix**: match the type of the source operand to what the asm wants.
- Asm has `jal memcpy` → declare the source as `const void*` (or cast at the call site)
- Asm has inline `lwl/lwr/...` → declare the source as `const char*`

Same pattern likely applies to `strcpy` / `memmove` / `strncpy` — verify when you encounter one.

---

## "Conflicts with built-in declaration" on libc decls

**Symptom**: `extern "C" int memcmp(const void*, const void*, u32);` (or similar with `u32`/`u16`/etc.) produces a compile warning, and codegen can shift subtly around the call.

**Cause**: `u32` is `unsigned long` (per `ultratypes.h`). GCC's built-in declarations for `memcmp` / `memcpy` / `strncmp` / `strlen` / etc. use `unsigned int`. Same width on MIPS but different types, so the compiler refuses to merge them with its builtins.

**Fix**: declare libc builtins with plain `unsigned` (or `unsigned int`) for size parameters, not `u32`.

```cpp
extern "C" int memcmp(const void* a, const void* b, unsigned n);   // good
extern "C" int memcmp(const void* a, const void* b, u32 n);        // warns, shifts codegen
```

---

## Register allocation sensitive to declaration order

**Symptom**: asm-differ shows `r` markers — same instruction sequence as the target, but the compiler picked different registers (`a1`↔`a2`, `s2`↔`s3`, etc.).

**Cause**: the SN compiler's register allocator is sensitive to *where in the source* a variable is declared, not just to its reads and writes. Declarations interleaved with control flow behave differently from declarations clustered at the head of the function.

**Fix**: try variations in source-level placement, building and diffing after each:
- Declare a local at its first use rather than at the top of the function
- Interleave a declaration between two `if` blocks rather than placing it after both
- Reorder unrelated declarations that share a register class

If no variation flips the allocation in your favour within the budget, **park**. Some `r` diffs have no source-level lever; chasing them past the budget is wasted time.

---

*Add new tips as they're discovered. Each tip names its symptom, its cause, and its fix.*

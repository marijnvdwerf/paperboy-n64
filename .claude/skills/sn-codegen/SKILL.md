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

## `bnel`-with-useful-delay-slot reusing a parameter register

**Symptom**: target has `bnel ..., .L_loop` whose delay slot is `move <param_reg>, <some_reg>` (e.g. `move a0, v0`), and the loop body downstream reads from that param register as if it were a different variable. Your C, structurally equivalent, produces `bne ... + nop` instead, and the loop variable lives in a fresh register (`a2`/`a3`/...) — diff shows `r` markers throughout.

**Cause**: the compiler emitted `bnel` because the branch target needed the param register loaded with a different value (e.g. the loop's "current" pointer). Branch-likely with a useful delay-slot move only happens when the value being moved is the *same register the loop body already expects*. If your C keeps the loop variable in a separate local, the compiler allocates it to a fresh register, the move-into-param-slot isn't needed, and you get plain `bne + nop`.

**Fix**: reassign the parameter itself instead of introducing a new local. The compiler then keeps the loop variable in the parameter's register slot, and the branch likely + delay slot move appears.

```cpp
// produces bne + nop, loop var in a2
void f(Joey* parent, Joey* child) {
    Joey* cur = parent->child;
    if (cur == child) { /* first-child path */ return; }
    /* loop using cur */
}

// produces bnel + `move a0, v0` in delay, loop var in a0
void f(Joey* parent, Joey* child) {
    Joey* head = parent->child;
    if (head == child) { /* first-child path */ return; }
    parent = head;          // reuse the a0 slot
    /* loop using parent */
}
```

For C++ methods where `this` is the parameter you'd like to reuse, copy `this` into a local of the same name and reassign the local — `this` itself is not assignable, but a `Joey* parent = this;` then `parent = head;` produces the same codegen.

---

*Add new tips as they're discovered. Each tip names its symptom, its cause, and its fix.*

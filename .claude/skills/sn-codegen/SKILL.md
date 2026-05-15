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

## Signed vs unsigned loop counter → `slti` vs `sltiu`

**Symptom**: loop-bound check at the end of a counted loop emits `slti` (signed) or `sltiu` (unsigned), and you have the opposite of target.

**Cause**: the compiler picks the comparison instruction directly from the loop variable's declared signedness. `s32 i; i < N` → `slti`. `u32 i; i < N` → `sltiu`. A cast at the comparison site (`(u32)i < N`) also flips it, but the cleanest match is to use the right type at declaration.

**Fix**: match the type of the loop counter to the asm. If target shows `sltiu`, declare `u32 i`. If `slti`, declare `s32 i`. This often cascades: with the right comparison, the surrounding branch pattern (`bnez` vs `bnezl`) also flips into place.

---

## `arr[i++] = X` triggers register-incremental address reuse

**Symptom**: target emits an indexed byte/word store (`sb`, `sw`, …) at `base+i` and then immediately uses `base+(i+1)` as the destination of a following call — keeping `i` in a register and incrementing it with `addiu a1, a1, 1; addu a1, base, a1`. Your code recomputes the second address as a single `addiu a1, base, i_plus_1`, and the diff shifts.

**Cause**: `arr[i] = X; f(&arr[i + 1], ...)` makes the compiler treat `i + 1` as a fresh expression and fold it into one `addiu` against `base`. `arr[i++] = X; f(&arr[i], ...)` makes the compiler reuse the register that holds `i`, bumping it in place with `addiu i, i, 1`, then computing `base + i` once.

**Fix**: use post-increment when the asm shows an "increment-then-index" pattern:

```cpp
// recomputes the address from scratch
info->name[sz] = '.';
func(&info->name[sz + 1], ...);

// keeps sz in a register, bumps it, reuses
info->name[sz++] = '.';
func(&info->name[sz], ...);
```

---

## Split nested member access into intermediate locals to control scheduling

**Symptom**: at a call site whose arg list mixes several `self->stream->X` accesses, the instructions are present in both versions but in a different order — `r` markers on register choices, `|` markers on schedule slots. Common around 4+ arg calls that need stack arg setup.

**Cause**: when args come from nested member accesses, the compiler treats each access as a separate dependency chain and reorders them based on register-pressure heuristics. The scheduler is sensitive to surrounding code (especially what was clobbered by a previous call), and the inline form gives it the most freedom — usually picking a different register (`v1` instead of `v0`) for the inner pointer, which cascades into a different schedule.

**Fix**: introduce named intermediate locals matching the target's load order. The locals' declaration order pins the load order in the emitted asm, and the named locals end up in registers the scheduler then respects:

```cpp
// nested access — scheduler chooses its own load order and registers
func(self->stream->unk4, self->buf, self->stream->unkA, self->stream->unkC, ...);

// explicit locals — loads happen in declaration order, registers stabilize
Stream* s = self->stream;
char* buf = self->buf;
void* unk4 = s->unk4;
u16 unkA  = s->unkA;
s32 unkC  = s->unkC;
func(unk4, buf, unkA, unkC, ...);
```

Doing this also tends to flip the post-call reload's register choice (`lw v0, …` instead of `lw v1, …`), because the fresh local "claims" the natural temp slot.

---

## Duplicate local for callee-saved value preservation

**Symptom**: target keeps a *duplicate* of the same value in two callee-saved registers across a span of calls (`move s3, s0` in some delay slot, then later uses `s3` instead of `s0` for an arg setup even though both hold the same pointer). Your version uses one register everywhere, the frame is one register smaller, and the diff shows a missing `move` plus a cascade of shifted save offsets.

**Cause**: when the C source has the *same* value reachable through two separately-named locals, the compiler allocates each to its own register and preserves both across calls — even when one is a trivial copy of the other. Reach the same value through a single local and you get one register.

**Fix**: introduce an aliasing local with a different name. The compiler will allocate it to its own callee-saved register and use it wherever that name is referenced in the source:

```cpp
// one local → one register, no extra save
void* x = self->stream->unk4;
if (f(x, …)) g(x, …);
if (cond) h(x, …);

// two locals → two callee-saved registers, extra save+move appear
void* x = self->stream->unk4;
void* y = x;
if (f(x, …)) g(x, …);
if (cond) h(y, …);
```

---

## Mismatched return type produces ghost `move v0, zero` at function end

**Symptom**: body matches but the epilogue has an extra `move v0, zero` (or similar `v0` setup) the target doesn't have. Often appears when porting a method that "looks like it returns" but the target asm just falls through to `jr ra` without touching `v0`.

**Cause**: function declared with a non-void return type (e.g. `s32`) but the C body has no explicit `return` statement at every path. The compiler emits an implicit zero-return at the fallthrough. The original source had the function declared `void`, so no `v0` is touched.

**Fix**: change the function's return type to `void` if the target asm doesn't set `v0` before return. For virtual methods, the *base class declaration* must also be `void` — otherwise the override's signature mismatches and the compiler errors. Update the pure-virtual declaration in the base class and any sibling overrides.

---

*Add new tips as they're discovered. Each tip names its symptom, its cause, and its fix.*

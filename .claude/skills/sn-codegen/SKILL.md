---
description: SN compiler (a GCC fork) codegen quirks observed in this Paperboy N64 decomp. Use when an asm-differ run shows a mismatch that isn't a structural issue (wrong type, missing field, off-by-one offset) — these are the small compiler choices the C source has to anticipate to match. Referenced from the `decomp` and `decomp-file` skills. Triggers on "register allocation diff", "reg-alloc", "`r` markers", "memcpy inlined", "why is memcpy/strcpy inlined", "builtin declaration warning", "conflicts with built-in declaration", or any question about why two structurally-equivalent C formulations produce different MIPS.
---

# SN compiler codegen quirks

A running catalogue of small SN-compiler (cc1n64 / cc1pln64 — a GCC fork) behaviours that affect asm-differ matching. When you hit a mismatch that *isn't* explained by structure (wrong field offset, wrong type, missing local), check here for a known pattern.

(Note: the C++ ABI — name mangling, vtable layout — is cfront-style and is documented in `decomp-file`. This skill is about *codegen* behaviour from SN's GCC fork: inlining decisions, register allocation, builtin handling.)

Each tip names: **symptom** (what asm-differ shows, or what the asm looks like), **cause** (the cfront behaviour), **fix** (the source-level lever, or "no lever, park").

---

## memcpy / strcpy inlined unexpectedly

**Symptom**: a small copy in the asm is a sequence of `lwl/lwr/swl/swr`, `lw/sw`, or `lb/sb` instead of `jal memcpy`. Or vice versa — your C produces an inlined copy but the target wants a real call.

**Cause**: the compiler decides whether and how to inline `memcpy(dst, src, N)` based on the static type of `src` at the call site:
- `const void*` → real `jal memcpy`
- `const char*` (or unknown-alignment byte pointer) → inline `lwl/lwr/swl/swr` (unaligned word ops)
- `const u32*` (or any aligned word pointer) → inline `lw/sw` (aligned), with a loop-unrolled body for larger sizes

**Fix**: match the type of the source operand to what the asm wants.
- Asm has `jal memcpy` → declare the source as `const void*` (or cast at the call site)
- Asm has inline `lwl/lwr/...` → declare the source as `const char*`
- Asm has inline `lw/sw` (aligned, possibly with a 4-word unrolled loop) → declare the source as `const u32*`

This was the lever in `func_80026450` (`const char*` for a 4-byte unaligned copy), `func_800269D0` and `func_80026A64` (`const u32*` for 60-byte and 204-byte aligned copies with unrolled 4-word loops).

Same pattern likely applies to `strcpy` / `memmove` / `strncpy` — verify when you encounter one.

---

## Thunk function: passing args through without setup

**Symptom**: a small function ends in a virtual call (or another `jal`), and the asm has `nop` in the jalr delay slot — *no* `move a1, ...` or argument setup before the call. Your C, declared with just `self`, produces a similar shape but the compiler aggressively packs the this-adjust into the jalr delay slot instead of leaving `nop`. Sometimes register choices shift too.

**Cause**: the source function is a thunk — it has its own extra arg (or several) that it passes through to the call unchanged. The compiler doesn't emit a `move` because the value is already in the correct argument register from the caller. The "wasted" `nop` in the delay slot is because the compiler has nothing left to schedule there — all the work is already done.

**Fix**: give the function the same arg list as the callee and pass the args through:

```cpp
// produces aggressive delay-slot packing, score never reaches 0
extern "C" void func_80026318(RenderContext* self) {
    self->vfunc23();
}

// matches: a1 carries through, nop fills the jalr delay slot
extern "C" void func_80026318(RenderContext* self, UNK, UNK arg) {
    self->vfunc23(arg);
}
```

The "extra" args don't even need to be referenced — leaving them anonymous (just typed) is enough to make cfront reserve the registers and the call schedules cleanly. Used in `func_80026318`, `func_800264A0`, `func_8002699C`.

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

## Reference receiver can preserve branch-time `this` coalescing

**Symptom**: target loads a member-call receiver into one arg register, preserves another branch-live value, then uses the branch delay slot to move the receiver into `a0`:

```asm
lw    a2, 4(v1)
move  a3, v1
beqz  v0, .L_else
move  a0, a2
```

Your structurally-equivalent C++ with a pointer receiver is two instructions shorter — it loads the receiver straight into `a0`, drops the preserving move, and keeps the same behavior.

**Cause**: for C++ member calls, `T* p = ...; p->method(...)` and `T& r = *...; r.method(...)` are not register-allocation equivalent under this compiler. The reference form can keep the object identity live separately from the eventual `this` argument, which gives cc1pln64 a reason to coalesce into `a0` across the branch instead of materializing `a0` immediately.

**Fix**: when two branch arms call methods on the same object and the target has this "extra" pre-branch receiver move pattern, try a reference local for the receiver:

```cpp
// shorter: compiler may load directly into a0
N64ControllerSystem* ncs = stone->unk4;
r = ncs->pfsFindFile(...);

// target form in func_80048E80: preserves a separate receiver identity
N64ControllerSystem& ncs = *stone->unk4;
r = ncs.pfsFindFile(...);
```

This was the key lever in `func_80048E80`: the pointer form kept collapsing away two target instructions; the reference form restored the target's branch coalescing exactly.

---

## Assignment expressions inside call arguments can pin argument setup order

**Symptom**: the right values reach a call, but the target loads register arguments early and in-order while your version sinks one field load until just before the `jal`, or moves stack-argument setup ahead of a field load. Plain named locals still leave a scheduling diff.

**Cause**: with direct member expressions, cc1pln64 has latitude to reorder independent argument trees. A block-local temp assigned *inside the argument list* creates a useful dependency at the exact call site while preserving the original argument position, which can force the compiler back into the target's load order.

**Fix**: as a late-stage scheduling lever, try declaration-only temps plus assignment expressions in the call:

```cpp
u16 companyCode;
s32 gameCode;
r = ncs.pfsAllocateFile(&o->pfs,
                        (companyCode = stone->companyCode),
                        (gameCode = stone->gameCode),
                        ...);
```

In `func_80048E80`, ordinary direct arguments and ordinary pre-initialized locals both produced the right values with the wrong schedule. The assignment-expression form made the two PFS call setups match exactly.

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

## Real narrow local type can replace repeated casts without changing codegen

**Symptom**: target computes a value once, keeps it live across calls, then later emits a truncating use such as `andi reg, reg, 0xffff`. Your C matches only when an `s32` local is wrapped in repeated casts like `(u16)count`, even though the source clearly represents a 16-bit quantity.

**Cause**: the compiler is happy to keep a `u16` local in a full-width register across the function and still emit the same truncation at the narrow use site. The narrow declaration records the real storage semantics; it does not necessarily force extra masking at each assignment.

**Fix**: when the value is semantically narrow, try declaring the local with that narrow type first and remove the casts at use sites. In `func_800459A0`, changing `s32 dataCount` plus repeated `(u16)dataCount` casts into a plain `u16 dataCount` preserved the same object output.

```cpp
// noisy but sometimes used as a temporary matching crutch
s32 count;
read(..., ((u16)count) * 2);
for (i = 0; i < (u16)count; i++) { ... }

// often equivalent codegen, and closer to the real type
u16 count;
read(..., count * 2);
for (i = 0; i < count; i++) { ... }
```

---

## Reuse one function-scope loop temp when the target keeps one register story

**Symptom**: two separated loops are structurally similar, but your version allocates different temps or shifts saved-register usage between them. The target appears to carry one counter or scratch value through the whole function instead.

**Cause**: separate block-local declarations give the allocator separate lifetimes to work with. A single function-scope declaration encourages the compiler to keep reusing one register identity across both loops, which can stabilize the surrounding saved-register layout and scheduling.

**Fix**: if the target uses the same register role across multiple loops, try one function-scope `i` or scratch temp reused by both loops before inventing aliases or casts. This helped `func_800459A0` line up after two same-shaped byte-swapping loops were made to share one `i` and one `value`.

```cpp
// distinct locals can produce distinct allocator decisions
for (s32 i = 0; i < a; i++) { ... }
...
for (s32 i = 0; i < b; i++) { ... }

// one visible lifetime can keep the register choice stable
s32 i;
u16 value;
for (i = 0; i < a; i++) { value = ...; }
...
for (i = 0; i < b; i++) { value = ...; }
```

---

## Mismatched return type produces ghost `move v0, zero` at function end

**Symptom**: body matches but the epilogue has an extra `move v0, zero` (or similar `v0` setup) the target doesn't have. Often appears when porting a method that "looks like it returns" but the target asm just falls through to `jr ra` without touching `v0`.

**Cause**: function declared with a non-void return type (e.g. `s32`) but the C body has no explicit `return` statement at every path. The compiler emits an implicit zero-return at the fallthrough. The original source had the function declared `void`, so no `v0` is touched.

**Fix**: change the function's return type to `void` if the target asm doesn't set `v0` before return. For virtual methods, the *base class declaration* must also be `void` — otherwise the override's signature mismatches and the compiler errors. Update the pure-virtual declaration in the base class and any sibling overrides.

---

## Loop back-edge: `j` to top vs `bnez` skipping the top check

**Symptom**: a walking loop (linked list, array iter) matches almost everything except the back-edge instruction. The target emits an unconditional `j` to the loop's null-check at the top; your version emits a conditional `bnez` (or `bne`) that skips the null check entirely, jumping to the body. Diff shows one `|` marker on the back-edge instruction, everything else aligned.

```asm
; target
loop_top:
  beqz  curr, end
  ; ... body, advance ...
  lw    curr, next(curr)
  j     loop_top         ; unconditional back to null check
  nop
end:

; your version
loop_top:
  beqz  curr, end
  ; ... body, advance ...
  lw    curr, next(curr)
  bnez  curr, body_top   ; skip the null check
  nop
end:
```

**Cause**: when the loop increment is a chain like `curr = curr->next`, the compiler proves the loaded value reaches the null check unchanged and folds the test — `lw curr, ...; bnez curr, body_top` replaces `lw curr, ...; j top; ... beqz curr, end`. Functionally equivalent, one fewer instruction.

**Fix**: break the data-flow chain between the load and the next iteration's null check by introducing an intermediate local for the loaded value:

```cpp
// folds: compiler combines lw with next iter's beqz, emits bnez back-edge
while (curr != NULL) {
    // ... body ...
    prev = curr;
    curr = curr->next;
}

// doesn't fold: lw lands in `next`, then the assignment to curr is a separate move,
// preventing the combiner from reusing the load — emits j back-edge
while (curr != NULL) {
    // ... body ...
    ListNode* next = curr->next;
    prev = curr;
    curr = next;
}
```

This was the key lever in `func_80026560` (linked-list unlink). Same pattern likely applies to any chained-pointer walk where the asm shows an unconditional `j` back-edge.

Related: a similar back-edge mismatch in `func_800264FC` (search loop) was fixed by a different lever — giving the null-exit path a side-effect (`result = 0; break;`) so the compiler couldn't merge it with the loop's natural exit. The general principle is the same: prevent the compiler from coalescing the load with the null check by introducing a step it can't see through.

---

## "Defaults" if-else placed inline vs out-of-line

**Symptom**: a small `defaults` branch (e.g. `fv1 = 0.0f; fv0 = 0.5f;`) and a larger compute branch share a single tail (a function call using `fv1`/`fv0`). The target emits the defaults code OUT-OF-LINE (physically after the compute branch) and reaches it via a likely branch with a useful delay slot (e.g. `bc1tl ...; mov.s fv1, fs0`). Your version emits defaults INLINE between the if-test and the compute branch, falling through via a regular branch and an explicit `j` over the inline block to the tail.

**Cause**: cfront's basic-block layout heuristic for `if (X) defaults; else compute;` and `if (X) compute; else defaults;` is not symmetric. When the *first* arm is the smaller "defaults" arm, cfront emits it inline; when it's the larger "compute" arm, cfront emits defaults out-of-line after the compute body and uses likely-branch + delay-slot work to reach it.

**Fix**: write the condition with the inverted polarity so the compute branch is the if-arm and defaults are the else-arm:

```cpp
// inline defaults (bad)
if (a == 0.0f && b == 0.0f) {
    fv1 = 0.0f; fv0 = 0.5f;
} else {
    /* compute fv1, fv0 */
}

// out-of-line defaults (good)
if (a != 0.0f || b != 0.0f) {
    /* compute fv1, fv0 */
} else {
    fv1 = 0.0f; fv0 = 0.5f;
}
```

This was the key lever for `func_80025E7C`: a single inversion dropped the score by an order of magnitude.

---

## Persistent zero across calls → `mtc1 zero, fs0` early vs late

**Symptom**: target loads zero into a callee-saved float reg (`mtc1 zero, fs0`) lazily — right before the first comparison that needs it. Your version loads zero into the same callee-saved reg eagerly — at the top of the function, well before the first user. Diff shows a stray `mtc1 zero, fs0` that's correct in shape but in the wrong position; everything downstream shifts.

**Cause**: declaring an explicit `f32 zero = 0.0f` (or `f32 fv1 = 0.0f`) at the *top* of the function makes cfront emit the load there. cfront then folds all later comparisons with `0.0f` into uses of that saved register.

**Fix**: don't declare a `zero` (or pre-initialized) variable at the top. Use `0.0f` literals at each comparison and assignment site. cfront's liveness analysis spots the multiple uses and still allocates a saved register, but it places the `mtc1 zero, ...` lazily, at the first use.

This worked in tandem with the if-else inversion above to take `func_80025E7C` from 1378 → 10.

---

## Inline indexing expression chooses different register than the named-temp form

**Symptom**: the last reg-alloc diff in an otherwise matching function is a `lui v0, %hi(LUT)` (yours) versus `lui v1, %hi(LUT)` (target) for an array base load. Subsequent uses (`addu v0, v0, v1`) all match — only the lui target register differs.

**Cause**: cfront picks the *first available* int temp when materializing an address. With the array index computed inline inside the subscript, `lui` is scheduled before the `mfc1 v0, fa0` that produces the index, so v0 is still free and gets picked. The compiler then has to `addiu v1, v0, ...` to move the address to v1 (the register downstream code expects).

**Fix**: lift the index into a named `s32` local computed first. That forces v0 to be allocated to the index value, leaving v1 as the natural choice for the address:

```cpp
// inline: lui v0, ...; addiu v1, v0, ...  (off by one r-marker)
fv1 = D_8006C5F0[((s32)((x + 1.0f) * 511.5f)) & 0x3FF] * k;

// named idx: lui v1, ...; addiu v1, v1, ...  (matches)
s32 idx = ((s32)((x + 1.0f) * 511.5f)) & 0x3FF;
fv1 = D_8006C5F0[idx] * k;
```

This was the final lever to push `func_80025E7C` from score 10 → 0.

---

## Zero-initialized tracking variable: narrow type folds to `$zero`, `int` stays in a register

**Symptom**: a variable initialized to `0` and reused later (loop counter, "previous" tracker, first-call argument) — the target sources *every* nearby zero from one callee-saved register (`move a1, s4` / `move s1, s4` / `move s0, s4`, where `s4` holds the zero-initialized variable). Your version emits `move a1, zero` / `move s1, zero` and chains the others off `$zero` instead. Pure `r` markers, often 2–3 of them, on `move`s that materialize `0`.

**Cause**: a *narrow* zero-initialized local (`u16 ch = 0;`) is constant-folded at its first use — reaching-definition analysis proves it's `0` there, so the compiler uses the free `$zero` register and doesn't bother sourcing from the variable's register. Widening the same local to `s32` (`int`) makes the compiler keep the materialized `0` live in its allocated register and reuse *that* register for other adjacent zero-initializations (the loop counter's `i = 0`, the strength-reduced index offset's `= 0`, the first call's argument). One register becomes the canonical zero source, exactly as the target does.

**Fix**: when a zero-init variable is also used as a `vfunc`/call argument or compared in a loop, and the target sources its neighbouring zeros from one register, try declaring it `s32` instead of `u16`/`u8` — even if the value it later holds is semantically a `u16` (e.g. a glyph/page code loaded via `lhu`). The wider type changes nothing about the loads/compares but flips the constant-0 sourcing.

```cpp
// u16: first use folds to $zero; i=0 and offset=0 chain off $zero → 3 r-markers
u16 ch = 0;
SurfaceBase* src = self->vfunc3(ch);       // move a1, zero
for (u32 i = 0; i < self->unk20; i++) {     // move i, zero
    if (self->unk24[i].unk4 != ch) { ch = self->unk24[i].unk4; src = self->vfunc3(ch); }
    ...
}

// s32: 0 stays live in s4, reused for the arg and both loop inits → score 0
s32 ch = 0;
SurfaceBase* src = self->vfunc3(ch);       // move a1, s4
for (u32 i = 0; i < self->unk20; i++) {     // move i, s4 ; move off, s4
    ...
}
```

This was the final lever in `func_80017DA8` (15 → 0). Note the matching virtual's base-class declaration must accept the wider type too (`vfunc3(s32)` not `vfunc3(u16)`), or the override signature mismatches.

---

## A spurious intermediate temp can block `for`-loop rotation

**Symptom**: a counted `for` loop with a call-heavy body compiles to a *top-tested* `while` (`sltu i,n; beqz` at the top, unconditional `j` back-edge at the bottom) instead of the target's *guard + bottom-test* `do-while` (`beqz n` guard, then `sltu i,n; bnez` at the loop end). The whole frame inflates — extra callee-saved registers, loop-invariant addresses hoisted that the target recomputes — and the score is huge (thousands) despite the body matching instruction-for-instruction.

**Cause**: SN/cfront only rotates a `for` loop into the guarded `do-while` form when register pressure is low enough. An *extra local that a human wouldn't have written* — e.g. a temp `u16 gx = glyph->unk2;` introduced to "help" CSE a repeated field load — raises pressure just enough that the rotation pass bails, and the compiler falls back to the top-test shape. The simpler, more faithful source rotates; the cluttered one doesn't.

**Fix**: remove invented temps and write the loop body the way a human would. In particular, don't manually CSE a repeated field access into a temp — the compiler does it. Reuse an already-stored value through its destination instead of re-reading the field:

```cpp
// blocks rotation (gx is not in the original), AND re-reads unk2
u16 gx = self->unk24[i].unk2;
rect.x = gx;
rect.w = gx + self->unk24[i].unkA;

// rotates correctly; rect.x's value is reused, unk2 loaded once
rect.x = self->unk24[i].unk2;
rect.w = rect.x + self->unk24[i].unkA;   // right = left + width
```

In `func_80017DA8`, removing the `gx` temp dropped the score 1986 → 135 (enabling rotation) and switching `rect.w` to reuse `rect.x` took it 135 → 15. General rule for this codebase: prefer the `for` loop a human would write over an explicit `if (n) do {} while (i<n)`; the compiler reaches the guarded `do-while` itself **once the body is clean**. If your `for` is top-testing, suspect an extra local before suspecting the loop form.

---

## Named local can BLOCK unwanted loop rotation (the reverse case)

**Symptom**: a `for` loop with a call-heavy body compiles to a *guard + bottom-test* rotated form (`beqz count` pre-check, `sltu; bnez` at the bottom, `bnez` for continue) instead of the target's *top-test* form (`sltu; beqz` at the top, `bnezl` + delay-slot `i++` for continue, `j` back-edge). The pre-check adds 2 extra instructions, the loop condition moves from top to bottom, and `bnezl` (branch-likely with useful delay slot) degrades to `bnez`. Score ~900–1000.

**Cause**: the compiler rotates the loop into the guarded do-while form when register pressure is low enough. The target was compiled from source where an *extra live local* across the loop body raised pressure enough to prevent rotation. Without that local, the compiler has breathing room and rotates.

This is the **mirror image** of the "spurious intermediate temp blocks rotation" tip above. That tip describes removing a temp to *enable* rotation. This tip describes adding one to *block* it.

**Fix**: introduce a named local for a value that's otherwise used as a store-then-reload pattern. The local claims a register, raising pressure, and the compiler falls back to the top-test form. Crucially, you must still read from memory (not from the local) where the target does, so the store+reload pattern is preserved:

```cpp
// rotates (pre-check + bottom-test, bnez): pressure too low
for (i = 0; i < self->count; i++) {
    self->entries[i] = (Entry*)ctx->lookup(name);
    if (self->entries[i] != NULL) {  // check from memory
        continue;
    }
    // error path ...
}

// does NOT rotate (top-test, bnezl with i++ in delay slot): local raises pressure
for (i = 0; i < self->count; i++) {
    Entry* entry;                                         // ← extra local
    entry = (Entry*)ctx->lookup(name);
    self->entries[i] = entry;                             // store through local
    if (self->entries[i] != NULL) {                       // check from MEMORY, not local
        continue;
    }
    // error path ...
}
```

The local `entry` occupies a register across the function-call boundary (`ctx->lookup`), tipping the allocator past the rotation threshold. But the `if` still reads `self->entries[i]` from memory (not `entry`), so the target's `sw; lw; lw; bnezl` store+reload+check pattern is preserved.

**Why `self->entries[i]` and not `entry`**: if you write `if (entry != NULL)`, the compiler uses the register directly (no reload), which collapses 4 instructions into 1 and the structural diff gets worse even though the score might improve on the rotation axis.

**Interaction with `for` vs `while`**: once rotation is blocked, a `for` loop with `continue` generates `bnezl; addiu i,i,1` (branch-likely with the for-increment folded into the delay slot). A `while` with explicit `i++; continue;` also generates `bnezl` but the while form is independently prone to rotation. If the `while` rotates, `bnezl` survives but the condition moves to the bottom. The `for` + named-local combo was the only formulation that produced both the top-test layout AND the `bnezl` delay-slot trick.

This was the key lever in `func_80021380` (skink.cpp): score went 990 → 0.

---

*Add new tips as they're discovered. Each tip names its symptom, its cause, and its fix.*

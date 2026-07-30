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

**Situational, not universal**: this lever is specific to address materialization racing an `mfc1`-produced index. It does *not* flip every inline-vs-named reg-alloc diff. In `func_80020860`, lifting a struct-copy's subscript (`this->offsets[track->offsetStart]` → `s32 start = track->offsetStart; ... [start]`) produced byte-identical asm — the register choice there was driven by the return structure (see "Shared trailing `return`" below), not the index expression. Try it, but if the asm doesn't move, the lever is elsewhere.

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

## Shared trailing `return <const>` vs per-path `return` — dying-register reuse + epilogue sharing

**Symptom**: a small early-out block (e.g. a `count == 1` special case) precedes a larger compute block, and both ultimately return the *same* constant. The target reuses the **dying compare registers** inside the early-out block — e.g. the `li v0, 1` that tested `== 1` becomes the address accumulator (`sllv v0, v1, v0`, destroying the `1`), with the just-tested value reused as data — and then reaches a **shared** `li v0, 1; jr` at the function tail. Your structurally-equivalent C, with an explicit `return 1` *inside* the early-out block, instead **preserves** the constant in `v0` (allocates the block's scratch onto a fresh register like `a0` to avoid clobbering `v0`) and jumps *past* the shared `li`, straight to `jr`. Diff shows a cluster of `r` markers in the early-out block plus a misplaced/duplicated `li v0, 1` at the tail.

**Cause** (inferred, but the lever is solid): when the early-out block has its own `return <const>`, the compiler notices the return register already holds that constant (left over from the preceding `if (x == 1)` compare) and keeps it live, allocating the block's arithmetic *around* `v0` — so it can bypass the epilogue. When the early-out block instead **falls through** to a single trailing `return <const>` shared with the compute block, it has no reason to preserve `v0`, so it freely reuses the just-tested/dying registers (`v0`, `v1`) for its own address math and re-materialises the constant once at the shared tail.

**Fix**: give the special case and the main body **one shared trailing `return`** via `if/else` + fall-through, instead of a per-path `return`. A different constant (e.g. the `count == 0 → return 0` path) can stay a separate early return; only the paths that return the *same* constant need to merge.

```cpp
// per-path return: count==1 preserves v0=1, allocates scratch to a0, bypasses the shared li
// (cluster of r-markers in the body)
if (track->offsetCount == 1) {
    *out = this->offsets[track->offsetStart];
    return 1;
}
/* main body ... */
return 1;

// shared trailing return: count==1 reuses v0/v1 and shares the epilogue li (matches)
if (track->offsetCount == 1) {
    *out = this->offsets[track->offsetStart];
} else {
    /* main body ... */
}
return 1;
```

**Corollary — don't express the shared return with a `result` accumulator.** A named `s32 result = 0; ... result = 1; return result;` lands in `a0` (named locals tend to here), costs a `move v0, a0` at the tail, *and* its liveness across the loop raises pressure enough to block loop rotation (an extra `sltu` guard appears). A bare trailing `return 1` / `return 0` puts the value straight in `v0`. This was the final lever in `func_80020860` (140 → 0).

---

## Cache-in-local suppresses a field reload the target wants; `for`-bound hoists, mid-body break re-reads

**Symptom**: a counted loop over a struct-field bound (`track->offsetCount`). The target loads the bound **fresh** into a register in the loop preheader (`lhu a0, 0x12(a2)` right before the loop, reused by both the entry `beqz` guard and the bottom `sltu`), *separately* from an earlier load of the same field used by preceding `== 0` / `== 1` checks. Your version is one instruction short (no preheader reload) and/or puts the early-check value in the wrong register — or, if you wrote the bound as a mid-body break, re-reads the field *every iteration*.

**Cause**: two interacting behaviours.
- A **named local** holding a struct field is kept live and reused across basic blocks (and tends to land in `a0`); the loop reuses that register instead of reloading. **Direct member reads** in separate blocks each get their own load — so the early-check read and the loop's read become two independent loads in two registers, matching a target that reloads. (In `func_80020860`, caching `u32 count` put it in `a0` and reused it for the bound — no reload; reading `track->offsetCount` directly put the early checks in `v1` and produced the target's separate preheader reload in `a0`.)
- A **`for (j = 0; j < obj->field; j++)`** condition gets the invariant field hoisted into a single preheader load (reused top and bottom). A **mid-body `if (j >= obj->field) break;`** is an ordinary per-iteration conditional and reloads the field each pass.

**Fix**: to match a target that reloads the bound fresh for the loop, *don't* cache the count in a local — read the member directly in the early checks, and write the loop as a `for` over the member (not a `>=` break):

```cpp
// caches count: reused for the bound (no preheader reload), early value forced to a0
u32 count = track->offsetCount;
if (count == 0) return 0;
if (count == 1) { ... }
for (j = 0; j < count; j++) { ... }

// direct member reads: early checks in v1, fresh preheader reload (a0) for the bound
if (track->offsetCount == 0) return 0;
if (track->offsetCount == 1) { ... }
for (j = 0; j < track->offsetCount; j++) { ... }   // NOT `if (j >= track->offsetCount) break;`
```

(Historical note: the missing reload here is **not** a cross-block CSE that needs a memory-store "kill" to defeat — there is no aliasing store involved. It's purely named-local-keeps-alive vs direct-member-reloads. Don't go hunting for phantom stores.)

---

## Inline an invariant member access to push its hoisted load *after* the loop guard

**Symptom**: an invariant value used in the loop body (`track->offsetTimeBase`) is loaded once, but on the wrong side of the loop's entry guard. The target loads it in the preheader, *after* the `beqz` guard; your version, with `s32 timeBase = track->offsetTimeBase;` declared *before* the loop, loads it *before* the guard — everything shifts by one slot.

**Cause**: a local assigned before the loop is emitted before the loop's entry test. An invariant member access written *inside* the loop body is hoisted by the compiler into the preheader — which is *after* the entry guard. Same field, same single load, different placement.

**Fix**: when the target's invariant load sits after the loop guard, inline the member access into the loop body rather than pre-loading it into a local:

```cpp
// loads timeBase before the for-guard
s32 timeBase = track->offsetTimeBase;
for (j = 0; j < track->offsetCount; j++)
    if (time < (f32)this->times[timeBase + j]) break;

// hoists the timeBase load into the preheader, after the guard (matches)
for (j = 0; j < track->offsetCount; j++)
    if (time < (f32)this->times[track->offsetTimeBase + j]) break;
```

The array base (`this->times`) hoists the same way; in `func_80020860` both landed in the preheader after the guard once `offsetTimeBase` was inlined.

---

## `?:` vs `if`-assignment controls `%hi(global)` CSE range

**Symptom**: a global referenced several times across a function. Target caches `lui %hi(global)` in a callee-saved reg for the early references but re-materializes a fresh `lui v0/v1` for a later reference (after intervening branches/calls). Your version merges *all* references into one callee-saved hi-cache, burning an extra s-register — frame grows by 8, and r-diffs cascade through the whole function.

**Cause**: CSE of the `%hi` constant follows the pre-jump-optimization CFG. A conditional written as declare-then-overwrite (`char* buf = a; if (cond) buf = b;`) produces a CFG that lets CSE carry the cached hi across the join into the later reference. The same logic written as a conditional expression (`char* buf = cond ? a : b;`) produces a join CSE won't cross — the later reference gets a fresh `lui` in a caller-saved temp, and the hi-cache pseudo's live range (hence the s-register count) shrinks to match.

**Fix**: when the target shows a fresh `lui` for a later global reference and your build reuses a long-lived s-reg instead, rewrite a preceding two-step conditional assignment as a `?:` initializer (or vice versa if mismatched in the other direction).

```cpp
// merges all %hi refs into one s5-cached pseudo (mismatch)
char* buf = self->pathBuf;
if (totalLen > 0x40) buf = new char[totalLen];

// hi-cache dies early; later strcat ref re-does lui v0 (matches)
char* buf = (totalLen <= 0x40) ? self->pathBuf : new char[totalLen];
```

This was the single lever in `func_8001393C` (albatross.cpp): score → 0 in one change. Note an inline-function wrapper around the global reference does NOT break the CSE — only the CFG shape does.

---

## Statement duplicated into both branch arms is often ONE source statement after the join

**Symptom**: both arms of an `if`/`else` end with the same instruction (e.g. `addu s6,s6,s0` — once in a `j` delay slot, once at the join label). Writing the statement in both source arms produces the same instructions but flips a callee-saved register pair elsewhere in the function (pure `r` markers on two unrelated variables, e.g. s5↔s6), because the duplicated source statement inflates the variable's pre-allocation ref count and changes its allocation priority.

**Fix**: write the statement ONCE, after the `if`/`else`. The compiler duplicates it into the branch delay slot itself:

```cpp
// two refs: fileOff's allocno priority rises, steals the wrong s-reg
if (compSize < len) { ...; fileOff += compSize; }
else                { ...; fileOff += compSize; }

// one ref: allocation matches; compiler still emits the copy in the delay slot
if (compSize < len) { ... } else { ... }
fileOff += compSize;
```

This fixed the s5/s6 swap in `func_80012CE8` (120 → 0). When you see an unexplained s-reg pair swap, audit the source for statements you duplicated across arms that the original likely wrote once.

---

## Reusing one variable for a pre-loop value AND the loop counter rotates the s-register frame

**Symptom**: a value computed before a loop (e.g. `totalRows = unk44 * scaleY`, used only in flip math) and the loop counter itself are the same source variable. The merged live range spans from the early computation through the whole loop, grabs a low callee-saved register (s3), and every other loop variable's s-register rotates by one — dozens of pure `r` markers.

**Fix**: split into two disjoint variables: a `s32 totalRows` for the pre-loop math, and a fresh `u32 row = 0` counter for the loop. The allocator gives each its own (shorter) range and the target's register assignment falls out — it may even coalesce both into the same register (m2c shows this as `temp_sN`/`var_sN` pairs). Was the 639 → 350 lever in `func_80012954`.

```cpp
// one variable: long live range, steals s3, rotates the frame
row = self->unk44 * scaleY;
if (!flip) { dst += (row - 1) * step; step = -step; }
row = 0;
while (row < self->unk44) { ...; row++; }

// two variables: ranges disjoint, allocation matches
s32 totalRows = self->unk44 * scaleY;
if (!flip) { dst += (totalRows - 1) * step; step = -step; }
u32 row = 0;
while (row < self->unk44) { ...; row++; }
```

Related smaller levers from the same function: a horizontal-scale dispatch on bit depth is a `switch (bpp)` with fall-through cases (`case 15: case 16:`), not nested `if`s — the switch gives the target's unsigned `sltiu` dispatch and body order (declare the switch value `u32`, not `u16`); and `if ((s32)copies > 0) { do{...}while(...); } src--;` (single decrement after the `if`) produces the target's branch-likely with the decrement duplicated into the delay slot.

---

## Apparently dead stack stores may come from an inlined output-parameter helper

**Symptom**: the target stores two computed values into stack locals, but one appears never to be reloaded. Writing direct assignments lets DSE remove or reorder a store and shifts every later stack slot, producing a large structural/stack diff. An `asm("m")` memory operand can force the stores, but project policy bans that crutch.

**Cause**: the values may have been produced by a small `static inline` helper taking output pointers. Before inlining/DSE, both stores are externally observable through pointer arguments, so the compiler preserves them and allocates real stack homes. The helper can also explain why the same paired stores appear at multiple call sites.

**Fix**: look for repeated pairs of related assignments and reconstruct a natural output-parameter helper before concluding that the store is inherently dead:

```cpp
static inline void getDimensions(Grid* grid, u32 row, u32 col, s32* width, s32* height) {
    *width = grid->widths[row];
    *height = grid->heights[col];
}

getDimensions(grid, row, col, &rowWidth, &colHeight);
```

In `func_80012F8C`, this pure-C helper restored the target's `rowWidth`/`colH` stores at `sp+0x40`/`sp+0x44`, corrected the uniform stack-slot shift, and dropped the score from 1744 to 475; later register/scheduling cleanups reached 0. Only park or consider an asm constraint after ruling out a real producer API such as an inline helper. Truly parsed-but-unused fields (such as BMP header metadata) may still be genuine DSE cases.

---

## Whole-function callee-saved register rotation (self in the wrong s-reg) is usually unleverable

**Symptom**: the diff is dominated by dozens–hundreds of `r` markers that are all the *same* uniform rotation of the entire s-register file (e.g. `this`/`self` in s1 where the target has s3, and every other callee-saved reg shifted by the same constant offset: s4→s3, s5→s4, s6→s7 …). Often the frame is one saved register larger/smaller too. Zero logic differences.

**Cause**: the SN/GCC global allocator colored the allocnos in a slightly different priority order, so the receiver param grabbed a different hard register and everything else cascaded. The order is a function of the whole live-range/frequency graph, not any single source construct.

**Fix**: no reliable *register-level* lever. Declaration reorder, copying the param to a local (`T* self2 = self`), fp-value caching, and narrow/wide type changes were all tried on `func_80011880` without moving `self` off s1. BUT: before parking, hunt for an upstream *structural* seed — uniform rotations are often phase shifts caused by one RTL-shape difference earlier in the function (an early-return label vs a shared tail, a pseudo born one expression too late). Two rotations previously judged unleverable in albatross.cpp later fell to the "shared tail" and "derive-pointer-from-field" tips below. Only if no structural seed can be found should you park.

---

## `flag = (c1 || c2);` value form vs branchy flag assignments

**Symptom**: an assert/dispatch flag computed from two conditions. Target materializes the flag in an argument register (`move a0,zero` in a delay slot, `li a0,1`, `beqz a0`) and shares ONE register (t2) for repeated loads of the same base pointer across both conditions. Your branchy form (`s32 f = 0; if (c1) f = 1; else if (c2) f = 1;`) produces the same instruction shapes but different register sourcing for the repeated base-pointer loads, cascading r-diffs.

**Fix**: write the flag as a logical-OR value expression:

```cpp
// branchy: same shape, wrong register story
s32 oob = 0;
if (a->x < self->y) oob = 1;
else if (a->z < self->w) oob = 1;

// value form: both self loads share one CSE pseudo, flag lands in a0 (matches)
s32 oob = (a->x < self->y || a->z < self->w);
```

This took `func_80012384` from 575 → 380 in one change. Related: where the target's shared base-pointer register is sourced (`move tX,a0` from the still-live incoming arg vs `lw tX,sp-slot`) depends on whether an aliasing store (e.g. a local's inline-ctor zero stores through a pointer) sits between the parm spill and the first load in RTL order — the local's DECLARATION POSITION moves those stores. Declaring the ctor-bearing local at the top blocked the direct-a0 fold and restored the shared-t2 shape.

---

## Shared-tail wrapper `if` vs early return — reload inheritance across the branch

**Symptom**: an early-out check whose bail path performs the same cleanup call as the function end (e.g. both call `dstSurface->vfunc3()`). Structure matches, but shortly after the check your build reloads a value from its stack slot (`lw t0,0x40(sp)`) where the target keeps using the register that just stored it (`sw t2,0x40(sp)` … `mult v0,t2`) — one extra load plus a caller-saved register rotation downstream until the next call resyncs them.

**Cause**: `if (!cond) { cleanup(); return; }` places the then-block inline in the RTL stream; the label that terminates it resets reload inheritance, so the register copy of the spilled value is forgotten. `if (cond) { whole body } cleanup();` has no label on the fallthrough path — the branch jumps forward over the entire body to the single shared tail — so reload inheritance carries the register through. Final instruction layout is IDENTICAL either way (jump optimization moves the bail path out of line); only the register story differs.

**Fix**: when the bail path and the function end share a call, write the body as one wrapper `if` with the shared statement after it:

```cpp
// early return: label kills inheritance, step reloads from stack
if (this->unkED4 != 0x80) { dstSurface->vfunc3(); return; }
...body using step...
dstSurface->vfunc3();

// shared tail: step stays in its register across the branch (matches)
if (this->unkED4 == 0x80) {
    ...body using step...
}
dstSurface->vfunc3();
```

This was the final lever in `func_80012384` (180 → 0). Related to the "Shared trailing `return <const>`" tip — same principle: merge duplicated tails in source; the compiler duplicates/moves them itself.

---

## Derive the pointer from the field, declare the count variable after — pseudo birth order

**Symptom**: a scale/copy loop uses three short-lived caller-saved temps (width counter, inner counter, destination pointer) and the diff is a pure 3-cycle rotation of a0/a1/a2 (e.g. target {width=a0, copies=a1, dst=a2}, yours {width=a2, copies=a0, dst=a1}). Instruction sequence identical; every hoist/scope/type variation is neutral or much worse.

**Cause**: global allocation order among equal-priority pseudos follows creation order. `s32 w = this->unk40; src = base + w;` births `w` in its own statement — later than the target, which birthed the merged load inside the pointer expression. The one-slot difference in creation order rotates the whole assignment cycle.

**Fix**: reference the field directly in the pointer computation and declare the counter afterward — CSE still merges the two field loads into a single register (no extra load emitted), but the pseudo is born inside the pointer expression and wins the first free register:

```cpp
// rotated: w born as its own statement
s32 srcWidth = this->unk40;
u8* src = (u8*)(dstPtr + srcWidth) - 1;

// matches: load born inside the src expression; srcWidth aliases it via CSE
u8* dst = (u8*)(dstPtr + dstSurface->unk26) - 1;
u8* src = (u8*)(dstPtr + this->unk40) - 1;
s32 srcWidth = this->unk40;
```

This was the final lever in `func_80012954` (350 → 0).

---

## Debugging register allocation directly with -dg / -dl dumps

**Symptom**: a whole-function callee-saved rotation (or any stubborn `r`-marker cluster) resists source levers and you're guessing at allocator behavior.

**Fix**: run the compiler by hand with dump flags — the RTL dumps are pure gold:

```sh
mips-linux-gnu-cpp <same flags as build.ninja> -o /tmp/x.i src/<file>.cpp
wibo tools/bin/cc1pln64.exe -quiet -G0 -O2 -dg /tmp/x.i -o /tmp/x.s   # -> /tmp/x.i.greg
wibo tools/bin/cc1pln64.exe -quiet -G0 -O2 -dl /tmp/x.i -o /tmp/x.s   # -> /tmp/x.i.lreg
```

`.greg` shows `;; N regs to allocate: <pseudo list in ALLOCATION ORDER>`, per-pseudo conflict lists, and `;; Register dispositions: 80 in 18 ...` (hard reg 16=s0 … 22=s6). `.lreg` shows per-pseudo stats: `Register 80 used 86 times across 1300 insns; crosses 30 calls`. Allocation priority ≈ `floor_log2(refs) * refs * size / live_length`; `find_reg` then hands each pseudo the lowest-numbered non-conflicting hard reg (callee-saved only if it crosses a call). With these dumps you can *compute* why a pseudo landed in the wrong s-reg instead of guessing. `-dc` (combine) explains opcode-level surprises (e.g. `plus`→`ior`).

---

## Loop counter in a callee-saved reg: the counter doubles as a pre-call value

**Symptom**: a copy-loop counter sits in a callee-saved reg (`move s1,zero` in a delay slot) in the target but in `a1`/`v1` in yours — and fixing it by hand (init before the call) leaves an extra early `move sX,zero` plus asserts sourcing zero from that reg.

**Cause**: a pseudo only gets a callee-saved reg if its live range crosses a call. The original often achieves this *invisibly* by reusing ONE variable for a pre-call value and the loop counter: e.g. `i = unk68 * 3;` (buffer size, passed to `new[]`/`readAt`) then after the calls `i = 0;` and count with it. The single pseudo crosses the calls (callee-saved), and the re-init lands naturally in the loop-guard delay slot.

**Fix**: when a target loop counter is callee-saved, look for a value computed before the calls that dies right where the counter is born — same register, back-to-back ranges is the tell (`addu s1,…` for the size, later `move s1,zero`). Merge them into one variable. In func_80011880 this single change snapped the ENTIRE callee-saved map (self, bits, fileOffset, &dummy, palBuf, i) into the target's assignment.

Related: one function-scope counter shared by several exclusive-path loops (instead of per-block locals) keeps one pseudo, and reusing a dead earlier variable (`fileOffset = biSize + 0xE;` instead of a fresh `dibOffset`) lands the value in that variable's register.

---

## Little-endian field parse: absolute sp reads mean buffer-index source, pointer-relative means walking-p source

**Symptom**: byte-assembled header fields read via fixed sp offsets (`lbu v0,0x2e(sp)`) in the target but via `4(s0)`-style pointer offsets in yours — and the pointer bump (`addiu s0,sp,0x32`) sits EARLIER in the target than your source order allows.

**Cause**: fields read through the walking pointer `p` keep s0-relative addressing and pin the `p += N` after them. Fields read via the buffer directly (`L.hdr[6]`, `L.dibHdr[12]`) are sp-absolute and let the compiler hoist `p += N` above them.

**Fix**: match per field: pointer-relative reads → `p[k]`; sp-absolute reads → `buf[k]` indices, with `p += N` placed between per the emitted `addiu s0` position.

---

## Byte-pair sum: `or` vs `addu` is HImode vs SImode — use a function-scope u32, never u16

**Symptom**: a little-endian 16-bit field parse `v = b[0] + (b[1] << 8)` emits `or` where the target has `addu` (or vice versa), and/or the sum schedules late into the wrong temp.

**Cause**: two independent axes that earlier looked like one trade-off:
- Opcode: a `u16` variable makes the plus an HImode set; combine proves the operands disjoint and canonicalizes plus→ior (`or`). Any u32/s32 (SImode) destination keeps `addu`. There is no u16 formulation that yields `addu`.
- Schedule: a *block-local* `u32` births the pseudo late and the sum sinks (no in-block consumer); a *function-scope* `u32` (declared with the other top locals, like an existing `bitsPerPixel`) keeps the early schedule and the target temp register.

**Fix**: declare the variable function-scope `u32` and write the plain `lo + (hi << 8)` sum. This gives `addu` AND the early schedule — resolving what the earlier "u16 flips scheduling but yields or" tip recorded as an open problem (func_80011880: 841 → 258 in one change).

---

## Hi-byte temps steer the scheduler's load placement in sparse blocks

**Symptom**: a small BB parses two byte-pair fields around an interleaved store (`sw zero`); everything is semantically right but the four `lbu`s and the `sw` come out permuted vs target (target hoists both hi-byte loads to the top, defers one lo-byte into a spare temp like `a0`). Statement order and `+` operand order permutations plateau a few points short.

**Cause**: the backward list scheduler gives "launch" bonuses by RTL adjacency; `lo + (hi << 8)` emits the lo load first, so the hi loads can never reach the top slots. The `-dS` dump (see below) shows the exact `launching X before Y` / `blocking insn N` decisions.

**Fix**: pre-load the hi bytes into named temps ABOVE the sums; each temp's load is emitted at the temp's declaration point, and the sums still fold into the same lbu/sll/addu triples:

```cpp
u32 planesHi = L.dibHdr[9];
u32 bitsHi = L.dibHdr[11];
self->unk68 = 0;
planes = L.dibHdr[8] + (planesHi << 8);
bitsPerPixel = L.dibHdr[10] + (bitsHi << 8);
```

This was the 258 → 0 sequence in func_80011880 (230 after the temps, 75 after both, then statement-order of the `= 0` store). Position the plain store between/before the temps per the target's `sw` slot.

---

## Debugging the instruction scheduler with -dS

**Symptom**: instructions all present with right registers but permuted within a BB, and source-order permutations don't converge.

**Fix**: `wibo tools/bin/cc1pln64.exe -quiet -G0 -O2 -dS file.i -o file.s` writes `file.i.sched`: per-BB priorities and a full backward ready-list trace (`ready list at T-N`, `launching X before Y with no stalls`, `blocking insn N for 1 cycles`). T-1 is the LAST insn of the BB; program order is descending T. Correlate insn numbers with `-dc` (`.combine`) which prints full RTL bodies. The "launch" bonus (7f000001) follows RTL adjacency, which is why source emission order matters even when the DAG is identical.

---

## A branch that jumps into the middle of shared tail code may be a shared statement after the if/else

**Symptom**: one arm of a large if/else ends with `j <addr>` where `<addr>` is not the epilogue but a later dispatch (e.g. a switch) that your version emits inside the other arm only; diff shows the jump landing short of your epilogue target.

**Cause**: the statement (e.g. a `switch (bitsPerPixel)` setting pixel-format masks) sits AFTER the if/else in the original, shared by both paths, even though only one path obviously needs it.

**Fix**: move the statement out of the arm to function scope after the join (func_80011880: TGA path falls into the same bitsPerPixel switch as the BMP paths; 5 → 0).

---

## Two-value flag check: `||` with one const-in-a-local flips the result/val register swap

**Symptom**: `if (val == 2) result = 1; else if (val == 3) result = 1;` produces the exact target instruction sequence but with `val`↔`result` registers swapped (result grabs the lower reg, e.g. v1, while target has val=v1, result=a0). No declaration-order/type variation moves it. Rewriting as `val == 2 || val == 3` collapses to a branchless `addu -2; sltu` range check instead.

**Cause**: with the else-if form, `result` has 4 refs (init + two stores + use) over a short range → higher allocator priority than `val` (3 refs) → allocated first, takes the low register. The original was an `||` with ONE shared store (`result` drops to 3 refs, low priority, allocated after `val`), and the range-check conversion didn't fire because one comparison operand wasn't a literal constant at expand time.

**Fix**: use `||` with one comparand routed through a local; the local blocks the contiguous-range conversion at expand, then CSE rematerializes it as the same `li` temp so no extra register appears:

```cpp
// swapped registers (else-if) or branchless sltu (plain ||)
s32 two = 2;
s32 result = 0;
if (val == two || val == 3) { result = 1; }   // matches: beq/bne chain, result in a0
return result;
```

This was the lever in `func_80023BB0` (30 → 0). Also a general debugging aid used to find it: a scratch-TU harness that pipes variant bodies through `mips-linux-gnu-cpp` + `cc1pln64.exe` directly makes trying a dozen variants take seconds instead of full ninja builds.

---

## Dtor calls sub-object dtor with `li a1,2` + own conditional delete → member, not base

**Symptom**: target dtor stores the vtable, runs the body, then `jal _._5Skink / li a1,2`, then `andi s0,s0,1; beqz; jal __builtin_delete`. Your version with `struct Derived : public Base` instead passes the incoming flag straight through (`jal _._5Skink / move a1,s0`) and omits the conditional delete — 20 bytes short.

**Cause**: GCC 2.x delegates the delete-flag to a sole leftmost base's dtor when it can. A *member* object's dtor is always called with flag 2 (destroy, don't free), forcing the derived dtor to emit its own `if (flags & 1) __builtin_delete(this)`. Same field offsets either way when the sub-object is at offset 0.

**Fix**: model the sub-object as a data member at offset 0 (`Skink skink;`), not a base class. Accesses become `self->skink.field` / `self->skink.method()`. Bonus: the ctor codegen is identical (member ctor call), and the s0/s1 callee-saved assignment flips into place too. If the class's virtuals are still `func_NNNN` externs, add `#pragma interface` above the struct so the compiler doesn't emit its own vtable (keep `INCLUDE_RODATA(_vt.NClass)`); the dtor/ctor bodies still emit and still reference `_vt.NClass` externally. This was `_._8Platypus` + `__8Platypus` (both to 0).

## Loop-index vs count s-reg swap: duplicate the post-loop store across if/else arms

**Symptom**: two callee-saved locals are swapped (`cmdIdx` in s2 / `count` in s1, target wants the reverse), every instruction otherwise identical (pure `r` markers). The `.greg` allocation order has the wrong pseudo first; `.lreg` shows the two contenders with near-equal `floor_log2(refs)*refs/live` priority (e.g. count 12refs/41 = 0.878 vs cmdIdx 38refs/220 = 0.864, count wins by a hair).

**Cause**: the priority tie is decided by ONE ref. cmdIdx is stored once after the loop (`self->unk1C = cmdIdx;`) at a branch join — one weight-1 ref. The original wrote that store inside BOTH arms of the trailing `if/else`, inflating cmdIdx to 39 refs (5*39/220 = 0.886 > count 0.878) so cmdIdx sorts first and grabs the low reg. The compiler merges the duplicated store back to a single instruction at the join, so final asm is byte-identical.

**Fix**: duplicate the tail store into both arms of the last if/else:
```c
if (file->nextToken() != 6) {
    file->parseError(6);
    self->unk1C = cmdIdx;   // duplicated
} else {
    self->unk1C = cmdIdx;   // duplicated
}
```
This flipped func_80022BE4 (platypus) from 125 to 0. Levers that did NOT work: for-loop (compiled top-tested, skill-450), moving the store before the check (flips alloc but misplaces the store, score 70), a `result = cmdIdx` copy (copy-propagated away, no effect), reordering the case-6 ORs (extends count.live but breaks asm). The count.live window that flips it is [42,46]; +1 ref on cmdIdx is the only asm-invisible way in. Note the sibling of skill "Duplicate source statement across if/else arms": there the duplication was a bug to REMOVE; here it is the FIX — the discriminator is whether the target's join instruction is a store of the contested pseudo.

## Hoisted loop-invariant constants land in swapped callee-saved regs (share an AND-result var)

**Symptom**: two loop-invariant constants hoisted into callee-saved regs are swapped vs target (e.g. `lui s4,0x7000`/`lui s7,0x6000` where target wants `lui s7,0x7000`/`lui s4,0x6000`), every instruction otherwise identical. From `.greg`/`.lreg` the two const-temp pseudos have lopsided priority and the wrong one allocates first (takes the lower reg).

**Cause**: a masked-compare pattern like `if ((cmd & MASK) == 0) … else if ((cmd & MASK) == OTHER) …` compiles to TWO `and cmd,MASK` insns in the pre-alloc RTL (one per branch — the compiler failed to CSE them across a large loop body), which the final asm later merges to one `and`. But at allocation time the MASK constant pseudo has 2 and-refs (≈5 total) vs the OTHER constant's 1 (≈3), so MASK sorts first and grabs the lower callee-saved reg.

**Fix**: force the single shared AND by introducing an explicit result variable used in both comparisons:
```c
u32 kind = cmd & 0x70000000;
if (kind == 0) { … }
else if (kind == 0x60000000) break;
```
Now the MASK constant is referenced by ONE `and` (ref count drops), the OTHER constant (shorter live range) wins the priority sort and takes the lower reg. This fixed the s4/s7 swap in func_800232E0 (loop 2). NOTE: this is the AND-RESULT var, distinct from naming the mask *constants* (which was neutral). Loop 1 (smaller body) CSE'd on its own and never needed this.

## Caller-saved AND-result gets v0 (target v1) + shift mis-scheduled: hoist only the shift

**Symptom**: after fixing the const-temp swap above, a shared caller-saved compare temp (`cmd & MASK`) lands in v0 where target has v1, and a following `srl`/shift and a sibling `andi` are scheduled in the wrong order (the shift should sit in the branch delay slot but doesn't, because it would clobber the v0 temp).

**Cause**: the compare temp is live only on the `!=0`/break edge and never overlaps the `n = (cmd>>16)&0x3F` computation (which lives on the `==0` edge), so the allocator packs both into v0. The target keeps the compare temp in v1 because the shift executes *speculatively* (delay slot) and thus conflicts with it.

**Fix**: hoist ONLY the shift before the branch, then finish the field extraction inside:
```c
n = cmd >> 16;             // executes unconditionally, like the target's delay-slot srl
if (kind == 0) {
    n = (n & 0x3F) + 1;    // rest stays in the if-body
    …
```
This makes `n`'s shift live across the branch → it conflicts with the compare temp → compare temp moves to v1, and the shift naturally fills the bnez delay slot. Hoisting *more* (the full `(cmd>>16)&0x3F`, or `n` entirely) over-moves the shift above the `and` (score regressed) — hoist the shift alone. Took func_800232E0 from 60 to 0.

## Loop-invariant constant re-materialized each iteration (loop.c "savings 1 not desirable")

**Symptom**: the target hoists a big constant into a register ONCE before a loop (`lui a3,0x1000` before the loop, reused via `beq`/`sltu`), often caller-saving it around a call inside the loop; yours re-emits the `lui` inside the loop every iteration. All registers otherwise match. The delta is ~6 coupled insns (the missing hoist + its spill/reload + minor schedule shift).

**Cause**: the constant is a *switch-dispatch pivot* (e.g. `switch (cmd & 0x70000000)` with a `case 0x10000000:` — GCC's balanced compare tree uses the pivot in a `beq` and an `sltu`, 2 uses). The `-dL` .loop dump shows `Insn NNN: regno NNN (life 2), move-insn savings 1 not desirable`. loop.c's invariant motion moves a *const-load* movable only when `savings >= 2`, and savings for a const ≈ (uses − 1). With 2 uses savings is 1 → refused in a high-pressure outer loop (the same life-2/savings-1 const IS moved in a low-pressure inner loop, so it's pressure-gated). The retail source clearly had a THIRD reference to the constant that later passes deleted, so loop.c saw savings 2 and hoisted; the extra use is invisible in the final asm.

**Fix**: add a dead reference to the constant inside the loop that (a) is an **assignment to a named local** (a bare `(void)(expr)` gets killed by cse1 *before* loop.c and does nothing) and (b) uses the constant via an operation that forces a `lui` const-load which cse1 merges with the pivot (`+ 0x10000000` works; `& 0x10000000` did NOT — it got DCE'd too early):
```c
u32 masked = cmd & 0x70000000;
u32 unused = masked + 0x10000000;   /* 3rd use → loop.c savings 2 → hoist; DCE'd after */
(void)unused;
switch (masked) { case 0: …; case 0x10000000: …; case 0x60000000: … }
```
cse1 merges the `0x10000000` with the switch pivot (raising its ref count / lifetime to `life 4`), loop.c hoists it, then flow DCE deletes the dead `unused` store — final asm has the hoisted const with its 2 real uses. This took func_80023000 from 493 to 0. Verify with the `-dL` dump: the const's line should flip from "not desirable" to "moved to NNN". Note: keep the `switch` (an outer `if (masked == cmdType)` chain gets the register right but reintroduces `self->unk1C` caching — beql/move — in the `0x60000000` arm, because cse can follow an if-chain but not multiway switch dispatch).

## Indexed array-member writes (`self->arr[i]`) vs walking pointer: constant offset placement

**Symptom**: a loop writes a struct-array member (e.g. `self->palette[i].r/g/b/a` at base offset 0xA4). Target keeps the induction register starting at `self` (`move a2,s0`) with the member offset in each store (`sb v0,0xA4(a2)`); your walking-pointer version (`T* dst = self; dst = (T*)((u8*)dst + 4)`) folds the base offset into the pointer init (`addiu a1,s0,0xA4`) and stores at `0..3(a1)`.

**Cause**: with an explicit walking pointer whose only uses share a constant offset, loop.c shifts the offset into the biv's initial value. With `self->arr[i]` indexing, strength reduction builds the giv as `self + 4*i` and leaves the field offsets (0xA4..0xA7) inside the MEMs — matching the target.

**Fix**: replace the walking pointer with a counter-indexed access on the object: `self->palette[i].b = ...; i++;` with a `do {} while (i < n)` loop. The counter serves both as compare var and (via the reduced giv) as the address base. This was the main lever in func_80027110's four palette loops (2918 → 425).

## Per-case init of a var shared across switch cases → init hoisted into compare delay slots

**Symptom**: target has `move a0,s1` (init of a walking pointer) in the *delay slots* of the switch's compare `beq`s AND at some case heads — i.e. the init exists once per case. A single init before the `switch` emits the move exactly once (wrong); declaring the var inside each case gives per-case moves but the wrong register.

**Cause**: a variable *declared* before the switch (long live range, uses in all cases → high priority) gets an early register (a0); the per-case *assignments* give the compiler one init per case to hoist into the dispatch delay slots.

**Fix**: split declaration and init: `u8* p; switch (...) { case A: p = buf; ...` — declaration before the switch pins the register, per-case assignment produces the per-case moves. (func_80027110: `p` in a0 with delay-slot inits.)

**Related**: the loop *counter* in the same function needed the opposite — declared fresh in each case (`u32 i = 0;` inside the case) rather than shared, to land in a1 (shared declaration pushed it to a2). When a var's register is wrong, toggle shared-decl vs per-case-decl: it changes live range and hence allocation priority. (425 → 170.)

## Byte-field parse cluster: pre-load into locals to group load-load-load / store-store-store

**Symptom**: parsing consecutive header bytes into consecutive struct fields. Target emits `lbu v0; lbu v1; lbu a0; sw v0; sw v1; sw a0` (loads grouped, then stores); direct member assignments emit interleaved `lbu/sw` pairs. Similarly, the target pre-loads the *high byte* of a later 16-bit field before an intervening store (`lbu v1,hi; sw other; lbu v0,lo; sll; addu`).

**Cause**: each `self->f = hdr[k]` is an independent dependency chain; the scheduler only groups them if the loads are separate statements feeding named locals.

**Fix**: introduce locals in the target's load order, then store:
```cpp
u32 b0 = hdr[0]; u32 b1 = hdr[1]; u32 b2 = hdr[2];
self->idLength = b0; self->colorMapType = b1; self->imageType = b2;
```
and for the hoisted-high-byte pattern, pull just the high byte into a local *before* the intervening store: `u32 hi = hdr[6]; self->bitDepth = hdr[7]; self->len = hdr[5] + (hi << 8);` (func_80027110 header parse.)

## Declarations inside loop bodies block while/for rotation (old-C style is codegen-visible)

**Symptom**: a source while/for compiles top-tested (`beq` exit test at top, `j` back-edge) but the target shows the rotated form: entry guard test before the loop + bottom conditional back-edge. Or a for-loop whose rotated target duplicates the decrement/test (entry `addiu; beq t6` AND bottom `addiu; bne t5` with TWO distinct const registers).

**Cause**: `expand_end_loop` (stmt.c) rolls a leading exit test to the loop bottom, and `duplicate_loop_exit_test` (jump.c) then copies it before the loop as the guard — but the expand_end_loop scan **aborts at any `NOTE_INSN_BLOCK_BEG`**, which cc1pln64 emits for any compound in the loop body containing a *declaration* (at any depth — even a `u8 b` temp deep in an inner do-while). Bodies with braces but no decls are fine. The duplicated entry test's constants get remapped/CSE'd with function-wide literals (t6) while the in-loop original keeps its own pseudo (t5, `li` hoisted into the guard's delay slot).

**Fix**: declare ALL locals at function top, old-C style, and write the natural `while`/`for`; the compiler produces the guard + bottom-test + duplicated-decrement itself. Do not hand-write `count--; if (count != negOne) do {...} while (...)` when the target shows the duplicated-test register pattern — write `for (count--; count != -1; count--)`.

---

## Loop tail `bne back; j exit` vs `beq exit; j back` is decided by reorg branch prediction — fix the LOOP NESTING, not the branch

**Symptom**: a loop followed by more code (so its exit needs a `j`) ends `beq cond,exit; j top` where the target has `bne cond,top; j exit` (or vice versa). Every do-while/for(;;)/continue/break reformulation of the loop itself leaves the pair inverted.

**Cause**: `reorg.c relax_delay_slots` reverses a conditional jump followed by an unconditional jump whenever the conditional is predicted taken (`mostly_true_jump > 0`). A backward branch into a loop predicts 2 (always inverted); a forward exit branch predicts 0 (stable) — UNLESS the exit target label is followed by `NOTE_INSN_LOOP_VTOP` (the bottom test of a *rotated* enclosing loop), which predicts 1 and makes reorg invert the beq-form into the bne-back form. So the target shape is only reachable when the ENCLOSING loop is a compiler-rotated while/for (see previous tip) whose bottom test is the exit target.

**Fix**: if the back-edge orientation won't flip, stop iterating on the inner loop and make the enclosing loop rotate (all decls at function top, `while (x < self->field)` reloading the member per-iteration). This was func_800279D8: 205 → 0 came from rewriting the whole function old-C style, not from touching the inner loop.

**Debug flow**: compile the isolated function with `-da` and diff the per-pass dumps (`.jump/.loop/.greg/.jump2/.dbr`) to find WHICH pass diverges, then read that pass in gcc-2.7.2.2 (local copy in ~/Downloads) to find the gate, then design source to flip the gate. Store harness + findings under `docs/codegen-tests/<topic>/`.

---

## Walking store pointer may be a strength-reduction giv from `arr[counter]`

**Symptom**: target initializes a walking store pointer with `move t0,<base>` in the loop *preheader* (after the entry guard), bumps it with `addiu t0,t0,1` after each store, while a separate counter register also increments. Your explicit `u8* dstPtr = dst;` local produces the same walker but its init lands *before* the entry guard.

**Cause**: the original had no pointer at all — it wrote `dst[totalWritten]`; loop.c strength reduction creates the walking-pointer giv, placing its init in the preheader. The counter stays live because something else reads it (e.g. `savedPos = totalWritten`). A source-level pointer local is emitted at its declaration position, before the loop.

**Fix**: replace the walking pointer with counter-indexed stores (`dst[totalWritten] = ...; totalWritten++;`). Sibling of the "indexed array-member writes vs walking pointer" tip — check WHERE the walker's init instruction sits relative to the loop guard to tell which form the original used.

---

## Free asm-invisible refs can flip allocation order at a floor_log2 boundary

**Symptom**: a counter and a strength-reduction giv (or two similar pseudos) have swapped registers; the `.greg` allocation order shows the wrong one first, and priorities (`floor_log2(refs)*refs/live` from `.lreg`) are not close enough for small tweaks.

**Fix**: priorities jump at powers of two of the ref count. Add semantically-free references that fold away in the final asm: e.g. write a loop guard as `if (i < self->count)` right after `i = 0;` instead of `if (self->count != 0)` — cse folds it to the same `beqz`, but the counter gains one ref per site. In func_80027110, four such guards took the shared counter from 28 refs (log2=4) to 32 (log2=5), lifting it above the palette givs: i landed in a1, givs in a2 (180 → 20).

---

## Commutative-op operand order + register tie: local-alloc ties dst to op1 — unless op1 is multi-block

**Symptom**: `or dst,op1,op2` where yours has `or v1,v1,v0` (dst==op1) and target has `or v1,v0,v1` (dst==op2, three roles in two regs). Every rewrite of the expression (`a|b` vs `b|a`, compound assignment, temp splitting) just moves the tie to whichever operand is op1.

**Cause**: `local-alloc.c block_alloc` ties the output qty with the FIRST dying register operand. Source operand order is preserved in RTL, so `lo | (hi << 8)` gives op1=lo — and the tie drags lo into the dst register. `combine_regs` FAILS for pseudos not local to one basic block (`REG_BASIC_BLOCK == REG_BLOCK_GLOBAL`), and then the tie falls through to op2.

**Fix**: declare the op1 variable at a scope where it is used in MORE THAN ONE basic block — e.g. `u32 lo; u32 hi;` before a `switch` whose two case-loops both assign/use them. The pseudos become global-allocated, the local tie on op1 fails, dst ties op2, and the operand order + registers both match. This was the final lever in func_80027110 (20 → 0), combined with source order `lo | (hi << 8)`. General principle: old-C-style shared declarations are codegen-visible through BOTH loop rotation (see above) and local-alloc tying.

---

## Compiled switch jump table + INCLUDE_RODATA vtable: missing `.align 3` pad

**Symptom**: function diffs at score 0 but the ROM checksum fails; `cmp -l` shows a 4-byte shift in the vtable pointers. The compiled switch's jump table (N entries, N odd) ends 4-misaligned, and the following `INCLUDE_RODATA(_vt.NClass)` file (spimdisasm-generated) has no `.align` directive, so the vtable lands 4 bytes early.

**Fix**: add `.align 3` after `.section .rdata` in the `_vt.*.s` file (the original cc1plus emitted vtables 8-aligned; splat's jtbl dump carries an explicit trailing pad word instead). Note the file is regenerated by configure — re-check after any reconfigure.

---

*Add new tips as they're discovered. Each tip names its symptom, its cause, and its fix.*

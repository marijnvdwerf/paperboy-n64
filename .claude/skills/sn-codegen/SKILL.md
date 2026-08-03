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

Same pattern likely applies to `strcpy` / `memmove` / `strncpy` — verify when you encounter one.

---

## Thunk function: passing args through without setup

**Symptom**: a small function ends in a virtual call (or another `jal`), and the asm has `nop` in the jalr delay slot — *no* `move a1, ...` or argument setup before the call. Your C, declared with just `self`, produces a similar shape but the compiler aggressively packs the this-adjust into the jalr delay slot instead of leaving `nop`. Sometimes register choices shift too.

**Cause**: the source function is a thunk — it has its own extra arg (or several) that it passes through to the call unchanged. The compiler doesn't emit a `move` because the value is already in the correct argument register from the caller. The "wasted" `nop` in the delay slot is because the compiler has nothing left to schedule there — all the work is already done.

**Fix**: give the function the same arg list as the callee and pass the args through. The "extra" args don't even need to be referenced — leaving them anonymous (just typed) is enough to make cfront reserve the registers and the call schedules cleanly.

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

For C++ methods where `this` is the parameter you'd like to reuse, copy `this` into a local of the same name and reassign the local — `this` itself is not assignable, but a `T* parent = this;` then `parent = head;` produces the same codegen.

---

## Signed vs unsigned loop counter → `slti` vs `sltiu`

**Symptom**: loop-bound check at the end of a counted loop emits `slti` (signed) or `sltiu` (unsigned), and you have the opposite of target.

**Cause**: the compiler picks the comparison instruction directly from the loop variable's declared signedness. `s32 i; i < N` → `slti`. `u32 i; i < N` → `sltiu`. A cast at the comparison site (`(u32)i < N`) also flips it, but the cleanest match is to use the right type at declaration.

**Fix**: match the type of the loop counter to the asm. If target shows `sltiu`, declare `u32 i`. If `slti`, declare `s32 i`. This often cascades: with the right comparison, the surrounding branch pattern (`bnez` vs `bnezl`) also flips into place.

---

## `arr[i++] = X` triggers register-incremental address reuse

**Symptom**: target emits an indexed byte/word store at `base+i` and then immediately uses `base+(i+1)` as the destination of a following call — keeping `i` in a register and incrementing it with `addiu`. Your code recomputes the second address as a single `addiu` against `base`, and the diff shifts.

**Cause**: `arr[i] = X; f(&arr[i + 1], ...)` makes the compiler treat `i + 1` as a fresh expression and fold it into one `addiu` against `base`. `arr[i++] = X; f(&arr[i], ...)` makes the compiler reuse the register that holds `i`, bumping it in place.

**Fix**: use post-increment when the asm shows an "increment-then-index" pattern.

---

## Split nested member access into intermediate locals to control scheduling

**Symptom**: at a call site whose arg list mixes several `self->stream->X` accesses, the instructions are present in both versions but in a different order — `r` markers on register choices, `|` markers on schedule slots.

**Cause**: when args come from nested member accesses, the compiler treats each access as a separate dependency chain and reorders them based on register-pressure heuristics.

**Fix**: introduce named intermediate locals matching the target's load order. The locals' declaration order pins the load order in the emitted asm.

---

## Reference receiver can preserve branch-time `this` coalescing

**Symptom**: target loads a member-call receiver into one arg register, preserves another branch-live value, then uses the branch delay slot to move the receiver into `a0`. Your structurally-equivalent C++ with a pointer receiver is two instructions shorter.

**Cause**: for C++ member calls, `T* p = ...; p->method(...)` and `T& r = *...; r.method(...)` are not register-allocation equivalent under this compiler. The reference form can keep the object identity live separately from the eventual `this` argument.

**Fix**: when two branch arms call methods on the same object and the target has this "extra" pre-branch receiver move pattern, try a reference local for the receiver.

---

## Assignment expressions inside call arguments can pin argument setup order

**Symptom**: the right values reach a call, but the target loads register arguments early and in-order while your version sinks one field load until just before the `jal`. Plain named locals still leave a scheduling diff.

**Cause**: with direct member expressions, cc1pln64 has latitude to reorder independent argument trees. A block-local temp assigned *inside the argument list* creates a useful dependency at the exact call site.

**Fix**: as a late-stage scheduling lever, try declaration-only temps plus assignment expressions in the call.

---

## Duplicate local for callee-saved value preservation

**Symptom**: target keeps a *duplicate* of the same value in two callee-saved registers across a span of calls. Your version uses one register everywhere, the frame is one register smaller, and the diff shows a missing `move` plus a cascade of shifted save offsets.

**Cause**: when the C source has the *same* value reachable through two separately-named locals, the compiler allocates each to its own register and preserves both across calls.

**Fix**: introduce an aliasing local with a different name.

---

## Real narrow local type can replace repeated casts without changing codegen

**Symptom**: target computes a value once, keeps it live across calls, then later emits a truncating use such as `andi reg, reg, 0xffff`. Your C matches only when an `s32` local is wrapped in repeated casts like `(u16)count`.

**Cause**: the compiler is happy to keep a `u16` local in a full-width register and still emit the same truncation at the narrow use site. The narrow declaration records the real storage semantics.

**Fix**: when the value is semantically narrow, try declaring the local with that narrow type and remove the casts. Note the matching virtual's base-class declaration must accept the wider type too, or the override signature mismatches.

---

## Reuse one function-scope loop temp when the target keeps one register story

**Symptom**: two separated loops are structurally similar, but your version allocates different temps or shifts saved-register usage between them.

**Cause**: separate block-local declarations give the allocator separate lifetimes. A single function-scope declaration encourages the compiler to keep reusing one register identity across both loops.

**Fix**: if the target uses the same register role across multiple loops, try one function-scope `i` or scratch temp reused by both loops.

---

## Mismatched return type produces ghost `move v0, zero` at function end

**Symptom**: body matches but the epilogue has an extra `move v0, zero` the target doesn't have.

**Cause**: function declared with a non-void return type but the C body has no explicit `return`. The compiler emits an implicit zero-return. The original source had the function declared `void`.

**Fix**: change the function's return type to `void` if the target asm doesn't set `v0` before return.

---

## Loop back-edge: `j` to top vs `bnez` skipping the top check

**Symptom**: a walking loop matches almost everything except the back-edge instruction. The target emits an unconditional `j` to the loop's null-check at the top; your version emits a conditional `bnez` that skips the null check.

**Cause**: when the loop increment is a chain like `curr = curr->next`, the compiler proves the loaded value reaches the null check unchanged and folds the test.

**Fix**: break the data-flow chain between the load and the next iteration's null check by introducing an intermediate local for the loaded value.

Related: a similar back-edge mismatch in a search loop can be fixed by giving the null-exit path a side-effect (`result = 0; break;`) so the compiler can't merge it with the loop's natural exit. The general principle: prevent the compiler from coalescing the load with the null check by introducing a step it can't see through.

---

## "Defaults" if-else placed inline vs out-of-line

**Symptom**: a small `defaults` branch and a larger compute branch share a single tail. The target emits defaults OUT-OF-LINE via a likely branch; your version emits defaults INLINE.

**Cause**: cfront's basic-block layout heuristic for `if (X) defaults; else compute;` and `if (X) compute; else defaults;` is not symmetric. When the *first* arm is the smaller "defaults" arm, cfront emits it inline; when it's the larger "compute" arm, cfront emits defaults out-of-line.

**Fix**: write the condition with the inverted polarity so the compute branch is the if-arm and defaults are the else-arm.

---

## Persistent zero across calls → `mtc1 zero, fs0` early vs late

**Symptom**: target loads zero into a callee-saved float reg lazily — right before the first comparison. Your version loads it eagerly at the top of the function.

**Cause**: declaring an explicit `f32 zero = 0.0f` at the *top* makes cfront emit the load there. Using `0.0f` literals lets cfront place the `mtc1 zero, ...` lazily, at the first use.

**Fix**: don't declare a `zero` variable at the top. Use `0.0f` literals at each comparison and assignment site.

---

## Inline indexing expression chooses different register than the named-temp form

**Symptom**: the last reg-alloc diff is a `lui v0, %hi(LUT)` (yours) versus `lui v1, %hi(LUT)` (target) for an array base load.

**Cause**: cfront picks the *first available* int temp when materializing an address. With the array index computed inline inside the subscript, `lui` is scheduled before the `mfc1 v0, fa0` that produces the index, so v0 is still free and gets picked.

**Fix**: lift the index into a named `s32` local computed first. That forces v0 to be allocated to the index value, leaving v1 as the natural choice for the address.

**Situational, not universal**: this lever is specific to address materialization racing an `mfc1`-produced index. It does *not* flip every inline-vs-named reg-alloc diff.

---

## Zero-initialized tracking variable: narrow type folds to `$zero`, `int` stays in a register

**Symptom**: a variable initialized to `0` — the target sources *every* nearby zero from one callee-saved register. Your version chains them off `$zero` instead. Pure `r` markers on `move`s that materialize `0`.

**Cause**: a *narrow* zero-initialized local (`u16 ch = 0;`) is constant-folded at its first use — the compiler uses `$zero`. Widening to `s32` makes the compiler keep the materialized `0` live in its allocated register and reuse it.

**Fix**: when a zero-init variable is also used as a call argument or compared in a loop, and the target sources its neighbouring zeros from one register, try declaring it `s32` instead of `u16`/`u8`.

---

## A spurious intermediate temp can block `for`-loop rotation

**Symptom**: a counted `for` loop compiles to a *top-tested* `while` instead of the target's *guard + bottom-test* `do-while`. The whole frame inflates — extra callee-saved registers, loop-invariant addresses hoisted that the target recomputes — and the score is huge despite the body matching.

**Cause**: SN/cfront only rotates a `for` loop into the guarded `do-while` form when register pressure is low enough. An extra local raises pressure just enough that the rotation pass bails.

**Fix**: remove invented temps and write the loop body the way a human would. Don't manually CSE a repeated field access into a temp — the compiler does it. Reuse an already-stored value through its destination instead of re-reading the field. General rule: prefer the `for` loop a human would write over an explicit `if (n) do {} while (i<n)`; the compiler reaches the guarded `do-while` itself **once the body is clean**. If your `for` is top-testing, suspect an extra local before suspecting the loop form.

---

## Named local can BLOCK unwanted loop rotation (the reverse case)

**Symptom**: a `for` loop compiles to a *guard + bottom-test* rotated form instead of the target's *top-test* form.

**Cause**: the compiler rotates the loop when register pressure is low enough. The target source had an *extra live local* that raised pressure enough to prevent rotation.

This is the **mirror image** of the "spurious intermediate temp blocks rotation" tip above.

**Fix**: introduce a named local for a value that's otherwise used as a store-then-reload pattern. The local claims a register, raising pressure, and the compiler falls back to the top-test form. Crucially, you must still read from memory (not from the local) where the target does, so the store+reload pattern is preserved.

**Interaction with `for` vs `while`**: once rotation is blocked, a `for` loop with `continue` generates `bnezl; addiu i,i,1` (branch-likely with the for-increment folded into the delay slot). The `for` + named-local combo may be the only formulation that produces both the top-test layout AND the `bnezl` delay-slot trick.

---

## Shared trailing `return <const>` vs per-path `return` — dying-register reuse + epilogue sharing

**Symptom**: a small early-out block and a larger compute block both return the *same* constant. The target reuses dying compare registers inside the early-out block and shares a single `li v0, 1; jr` at the function tail. Your version with per-path `return` preserves `v0` throughout.

**Cause**: when the early-out block has its own `return <const>`, the compiler keeps the return register live. When it instead falls through to a shared trailing `return <const>`, it freely reuses the dying registers.

**Fix**: give the special case and the main body **one shared trailing `return`** via `if/else` + fall-through. Only paths that return the *same* constant need to merge.

**Corollary — don't express the shared return with a `result` accumulator.** A named `s32 result = 0; ... result = 1; return result;` lands in `a0`, costs a `move v0, a0` at the tail, *and* its liveness across the loop raises pressure enough to block loop rotation. A bare trailing `return 1` / `return 0` puts the value straight in `v0`.

---

## Cache-in-local suppresses a field reload the target wants; `for`-bound hoists, mid-body break re-reads

**Symptom**: a counted loop over a struct-field bound. The target loads the bound **fresh** in the loop preheader, separately from an earlier load of the same field. Your version reuses a cached local.

**Cause**: two interacting behaviours.
- A **named local** holding a struct field is kept live and reused across basic blocks. **Direct member reads** in separate blocks each get their own load.
- A **`for (j = 0; j < obj->field; j++)`** condition gets the invariant field hoisted into a single preheader load. A **mid-body `if (j >= obj->field) break;`** is an ordinary per-iteration conditional and reloads the field each pass.

**Fix**: to match a target that reloads the bound fresh for the loop, *don't* cache the count in a local — read the member directly, and write the loop as a `for` over the member (not a `>=` break).

---

## Inline an invariant member access to push its hoisted load *after* the loop guard

**Symptom**: an invariant value used in the loop body is loaded once, but on the wrong side of the loop's entry guard. The target loads it in the preheader, *after* the `beqz` guard; your version loads it *before*.

**Cause**: a local assigned before the loop is emitted before the loop's entry test. An invariant member access written *inside* the loop body is hoisted by the compiler into the preheader — which is *after* the entry guard.

**Fix**: when the target's invariant load sits after the loop guard, inline the member access into the loop body rather than pre-loading it into a local.

---

## `?:` vs `if`-assignment controls `%hi(global)` CSE range

**Symptom**: a global referenced several times across a function. Target caches `lui %hi(global)` in a callee-saved reg for early references but re-materializes a fresh `lui` for a later reference. Your version merges *all* references into one callee-saved hi-cache, burning an extra s-register.

**Cause**: CSE of the `%hi` constant follows the pre-jump-optimization CFG. A conditional written as declare-then-overwrite produces a CFG that lets CSE carry the cached hi across the join. A conditional expression (`?:`) produces a join CSE won't cross.

**Fix**: when the target shows a fresh `lui` for a later global reference and your build reuses a long-lived s-reg, rewrite a preceding two-step conditional assignment as a `?:` initializer (or vice versa). Note an inline-function wrapper around the global reference does NOT break the CSE — only the CFG shape does.

---

## Statement duplicated into both branch arms is often ONE source statement after the join

**Symptom**: both arms of an `if`/`else` end with the same instruction. Writing the statement in both source arms produces the same instructions but flips a callee-saved register pair elsewhere in the function, because the duplicated source statement inflates the variable's pre-allocation ref count.

**Fix**: write the statement ONCE, after the `if`/`else`. The compiler duplicates it into the branch delay slot itself. When you see an unexplained s-reg pair swap, audit the source for statements you duplicated across arms that the original likely wrote once.

---

## Reusing one variable for a pre-loop value AND the loop counter rotates the s-register frame

**Symptom**: a value computed before a loop and the loop counter itself are the same source variable. The merged live range grabs a low callee-saved register, and every other loop variable's s-register rotates by one.

**Fix**: split into two disjoint variables: one for the pre-loop math and a fresh counter for the loop. The allocator gives each its own (shorter) range and the target's register assignment falls out.

---

## Apparently dead stack stores may come from an inlined output-parameter helper

**Symptom**: the target stores two computed values into stack locals, but one appears never to be reloaded. Writing direct assignments lets DSE remove a store and shifts every later stack slot.

**Cause**: the values may have been produced by a small `static inline` helper taking output pointers. Before inlining/DSE, both stores are externally observable through pointer arguments, so the compiler preserves them.

**Fix**: look for repeated pairs of related assignments and reconstruct a natural output-parameter helper before concluding that the store is inherently dead.

---

## Whole-function callee-saved register rotation (self in the wrong s-reg) is usually unleverable

**Symptom**: the diff is dominated by dozens–hundreds of `r` markers that are all the *same* uniform rotation of the entire s-register file. Zero logic differences.

**Cause**: the SN/GCC global allocator colored the allocnos in a slightly different priority order.

**Fix**: no reliable *register-level* lever. BUT: before parking, hunt for an upstream *structural* seed — uniform rotations are often phase shifts caused by one RTL-shape difference earlier in the function (an early-return label vs a shared tail, a pseudo born one expression too late). Only if no structural seed can be found should you park.

---

## `flag = (c1 || c2);` value form vs branchy flag assignments

**Symptom**: an assert/dispatch flag computed from two conditions. Target shares ONE register for repeated loads of the same base pointer across both conditions. Your branchy form produces cascading r-diffs.

**Fix**: write the flag as a logical-OR value expression: `s32 oob = (a->x < self->y || a->z < self->w);`

---

## Shared-tail wrapper `if` vs early return — reload inheritance across the branch

**Symptom**: an early-out check whose bail path performs the same cleanup call as the function end. Your build reloads a value from its stack slot where the target keeps using the register.

**Cause**: `if (!cond) { cleanup(); return; }` places the then-block inline in the RTL stream; the label that terminates it resets reload inheritance. `if (cond) { whole body } cleanup();` has no label on the fallthrough path, so reload inheritance carries the register through.

**Fix**: when the bail path and the function end share a call, write the body as one wrapper `if` with the shared statement after it. Merge duplicated tails in source; the compiler duplicates/moves them itself.

---

## Derive the pointer from the field, declare the count variable after — pseudo birth order

**Symptom**: a scale/copy loop uses three short-lived caller-saved temps and the diff is a pure 3-cycle rotation of a0/a1/a2. Instruction sequence identical.

**Cause**: global allocation order among equal-priority pseudos follows creation order. A local born in its own statement is created later than one born inside a pointer expression.

**Fix**: reference the field directly in the pointer computation and declare the counter afterward — CSE still merges the two field loads into a single register, but the pseudo is born inside the pointer expression and wins the first free register.

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

**Symptom**: a copy-loop counter sits in a callee-saved reg in the target but in a caller-saved reg in yours.

**Cause**: a pseudo only gets a callee-saved reg if its live range crosses a call. The original often achieves this by reusing ONE variable for a pre-call value and the loop counter: e.g. `i = unk68 * 3;` (buffer size, passed to `new[]`) then after the calls `i = 0;` and count with it. The single pseudo crosses the calls (callee-saved).

**Fix**: when a target loop counter is callee-saved, look for a value computed before the calls that dies right where the counter is born — same register, back-to-back ranges is the tell. Merge them into one variable.

---

## Little-endian field parse: absolute sp reads mean buffer-index source, pointer-relative means walking-p source

**Symptom**: byte-assembled header fields read via fixed sp offsets in the target but via pointer offsets in yours.

**Cause**: fields read through a walking pointer keep pointer-relative addressing and pin `p += N` after them. Fields read via the buffer directly are sp-absolute and let the compiler hoist `p += N` above them.

**Fix**: match per field: pointer-relative reads → `p[k]`; sp-absolute reads → `buf[k]` indices, with `p += N` placed between per the emitted `addiu` position.

---

## Byte-pair sum: `or` vs `addu` is HImode vs SImode — use a function-scope u32, never u16

**Symptom**: a little-endian 16-bit field parse `v = b[0] + (b[1] << 8)` emits `or` where the target has `addu` (or vice versa).

**Cause**: two independent axes:
- Opcode: a `u16` variable makes the plus an HImode set; combine proves the operands disjoint and canonicalizes plus→ior (`or`). Any u32/s32 (SImode) destination keeps `addu`.
- Schedule: a *block-local* `u32` births the pseudo late and the sum sinks; a *function-scope* `u32` keeps the early schedule.

**Fix**: declare the variable function-scope `u32` and write the plain `lo + (hi << 8)` sum.

---

## Hi-byte temps steer the scheduler's load placement in sparse blocks

**Symptom**: a small BB parses two byte-pair fields around an interleaved store; the four `lbu`s and the `sw` come out permuted vs target.

**Cause**: the backward list scheduler gives "launch" bonuses by RTL adjacency; `lo + (hi << 8)` emits the lo load first, so the hi loads can never reach the top slots.

**Fix**: pre-load the hi bytes into named temps ABOVE the sums; each temp's load is emitted at the temp's declaration point.

---

## Debugging the instruction scheduler with -dS

**Symptom**: instructions all present with right registers but permuted within a BB, and source-order permutations don't converge.

**Fix**: `wibo tools/bin/cc1pln64.exe -quiet -G0 -O2 -dS file.i -o file.s` writes `file.i.sched`: per-BB priorities and a full backward ready-list trace. T-1 is the LAST insn of the BB; program order is descending T. Correlate insn numbers with `-dc` (`.combine`) which prints full RTL bodies. The "launch" bonus (7f000001) follows RTL adjacency, which is why source emission order matters even when the DAG is identical.

---

## A branch that jumps into the middle of shared tail code may be a shared statement after the if/else

**Symptom**: one arm of a large if/else ends with `j <addr>` where `<addr>` is not the epilogue but a later dispatch that your version emits inside the other arm only.

**Cause**: the statement sits AFTER the if/else in the original, shared by both paths, even though only one path obviously needs it.

**Fix**: move the statement out of the arm to function scope after the join.

---

## Two-value flag check: `||` with one const-in-a-local flips the result/val register swap

**Symptom**: `if (val == 2) result = 1; else if (val == 3) result = 1;` produces the right instructions with `val`↔`result` registers swapped. Rewriting as `val == 2 || val == 3` collapses to a branchless range check.

**Cause**: with the else-if form, `result` has more refs → higher priority → allocated first, takes the low register. An `||` with ONE shared store drops `result`'s ref count.

**Fix**: use `||` with one comparand routed through a local; the local blocks the contiguous-range conversion at expand, then CSE rematerializes it as the same `li` temp.

Also a general debugging aid: a scratch-TU harness that pipes variant bodies through `mips-linux-gnu-cpp` + `cc1pln64.exe` directly makes trying a dozen variants take seconds instead of full ninja builds.

---

## Dtor calls sub-object dtor with `li a1,2` + own conditional delete → member, not base

**Symptom**: target dtor calls sub-object dtor with `li a1,2`, then does its own `andi s0,s0,1; beqz; jal __builtin_delete`. Your version with inheritance passes the incoming flag straight through.

**Cause**: GCC 2.x delegates the delete-flag to a sole leftmost base's dtor when it can. A *member* object's dtor is always called with flag 2, forcing the derived dtor to emit its own conditional delete.

**Fix**: model the sub-object as a data member at offset 0, not a base class.

---

## Loop-index vs count s-reg swap: duplicate the post-loop store across if/else arms

**Symptom**: two callee-saved locals are swapped, every instruction otherwise identical. `.lreg` shows near-equal priority.

**Cause**: the priority tie is decided by ONE ref. The contested variable is stored once after the loop at a branch join — one ref. The original wrote that store inside BOTH arms of the trailing `if/else`, inflating the ref count so it sorts first. The compiler merges the duplicated store back to a single instruction.

**Fix**: duplicate the tail store into both arms of the last if/else. Note the sibling "Statement duplicated into both branch arms" tip: there the duplication was a bug to REMOVE; here it is the FIX — the discriminator is whether the target's join instruction is a store of the contested pseudo.

---

## Hoisted loop-invariant constants land in swapped callee-saved regs (share an AND-result var)

**Symptom**: two loop-invariant constants hoisted into callee-saved regs are swapped vs target.

**Cause**: a masked-compare pattern like `if ((cmd & MASK) == 0) … else if ((cmd & MASK) == OTHER) …` compiles to TWO `and cmd,MASK` insns in the pre-alloc RTL. MASK has more refs → higher priority → allocated first.

**Fix**: force the single shared AND by introducing an explicit result variable: `u32 kind = cmd & MASK; if (kind == 0) { … } else if (kind == OTHER) break;`. The MASK constant drops to ONE ref.

---

## Caller-saved AND-result gets v0 (target v1) + shift mis-scheduled: hoist only the shift

**Symptom**: a shared compare temp lands in v0 where target has v1, and a shift and sibling `andi` are scheduled in the wrong order.

**Cause**: the compare temp and the shift computation don't overlap in one branch, so the allocator packs both into v0. The target keeps the compare temp in v1 because the shift executes *speculatively* (delay slot) and thus conflicts with it.

**Fix**: hoist ONLY the shift before the branch, then finish the field extraction inside. This makes the shift live across the branch → it conflicts with the compare temp → compare temp moves to v1. Hoisting *more* (the full expression) over-moves the shift above the `and`.

---

## Loop-invariant constant re-materialized each iteration (loop.c "savings 1 not desirable")

**Symptom**: the target hoists a big constant into a register ONCE before a loop; yours re-emits the `lui` inside the loop every iteration.

**Cause**: the constant has only 2 uses. The `-dL` .loop dump shows `move-insn savings 1 not desirable`. loop.c's gate is `(threshold * savings * lifetime) >= insn_count`. The retail source had a THIRD reference that later passes deleted, so loop.c saw savings 2 and hoisted.

**Fix**: add a dead reference to the constant inside the loop that (a) is an **assignment to a named local** (a bare `(void)(expr)` gets killed by cse1 *before* loop.c) and (b) uses the constant via an operation that forces a `lui` const-load which cse1 merges with the pivot (`+ CONST` works; `& CONST` did NOT — it got DCE'd too early). Verify with the `-dL` dump: the const's line should flip from "not desirable" to "moved to NNN".

---

## Indexed array-member writes (`self->arr[i]`) vs walking pointer: constant offset placement

**Symptom**: a loop writes a struct-array member. Target keeps the induction register starting at `self` with the member offset in each store; your walking-pointer version folds the base offset into the pointer init.

**Cause**: with an explicit walking pointer, loop.c shifts the offset into the biv's initial value. With `self->arr[i]` indexing, strength reduction builds the giv as `self + 4*i` and leaves the field offsets inside the MEMs.

**Fix**: replace the walking pointer with a counter-indexed access on the object.

---

## Per-case init of a var shared across switch cases → init hoisted into compare delay slots

**Symptom**: target has `move a0,s1` (init of a walking pointer) in the *delay slots* of the switch's compare `beq`s AND at some case heads. A single init before the `switch` emits the move exactly once (wrong).

**Cause**: a variable *declared* before the switch (long live range) gets an early register; the per-case *assignments* give the compiler one init per case to hoist into the dispatch delay slots.

**Fix**: split declaration and init: `u8* p; switch (...) { case A: p = buf; ...` — declaration before the switch pins the register, per-case assignment produces the per-case moves.

**Related**: the loop *counter* may need the opposite — declared fresh in each case rather than shared, to land in a different register. When a var's register is wrong, toggle shared-decl vs per-case-decl.

---

## Byte-field parse cluster: pre-load into locals to group load-load-load / store-store-store

**Symptom**: parsing consecutive header bytes into consecutive struct fields. Target groups loads then stores; direct member assignments emit interleaved `lbu/sw` pairs.

**Cause**: each `self->f = hdr[k]` is an independent dependency chain; the scheduler only groups them if the loads are separate statements feeding named locals.

**Fix**: introduce locals in the target's load order, then store.

---

## Declarations inside loop bodies block while/for rotation (old-C style is codegen-visible)

**Symptom**: a source while/for compiles top-tested but the target shows the rotated form.

**Cause**: `expand_end_loop` (stmt.c) rolls a leading exit test to the loop bottom, but the scan **aborts at any `NOTE_INSN_BLOCK_BEG`**, which cc1pln64 emits for any compound in the loop body containing a *declaration* (at any depth). Bodies with braces but no decls are fine.

**Fix**: declare ALL locals at function top, old-C style, and write the natural `while`/`for`; the compiler produces the guard + bottom-test + duplicated-decrement itself.

---

## Loop tail `bne back; j exit` vs `beq exit; j back` is decided by reorg branch prediction — fix the LOOP NESTING, not the branch

**Symptom**: a loop followed by more code ends `beq cond,exit; j top` where the target has `bne cond,top; j exit`. Every loop reformulation leaves the pair inverted.

**Cause**: `reorg.c relax_delay_slots` reverses a conditional jump followed by an unconditional jump whenever the conditional is predicted taken. The target shape is only reachable when the ENCLOSING loop is a compiler-rotated while/for whose bottom test is the exit target.

**Fix**: if the back-edge orientation won't flip, stop iterating on the inner loop and make the enclosing loop rotate (all decls at function top, reloading the member per-iteration).

**Debug flow**: compile the isolated function with `-da` and diff the per-pass dumps (`.jump/.loop/.greg/.jump2/.dbr`) to find WHICH pass diverges, then read that pass in gcc-2.7.2.2 to find the gate.

---

## Walking store pointer may be a strength-reduction giv from `arr[counter]`

**Symptom**: target initializes a walking store pointer in the loop *preheader* (after the entry guard); your explicit pointer local produces the same walker but its init lands *before* the entry guard.

**Cause**: the original had no pointer at all — it wrote `dst[totalWritten]`; loop.c strength reduction creates the walking-pointer giv, placing its init in the preheader.

**Fix**: replace the walking pointer with counter-indexed stores. Check WHERE the walker's init instruction sits relative to the loop guard to tell which form the original used.

---

## Free asm-invisible refs can flip allocation order at a floor_log2 boundary

**Symptom**: a counter and a strength-reduction giv have swapped registers; priorities are not close enough for small tweaks.

**Fix**: priorities jump at powers of two of the ref count. Add semantically-free references that fold away in the final asm: e.g. write a loop guard as `if (i < self->count)` right after `i = 0;` instead of `if (self->count != 0)` — cse folds it to the same `beqz`, but the counter gains one ref per site.

---

## Commutative-op operand order + register tie: local-alloc ties dst to op1 — unless op1 is multi-block

**Symptom**: `or dst,op1,op2` where yours has `or v1,v1,v0` (dst==op1) and target has `or v1,v0,v1` (dst==op2).

**Cause**: `local-alloc.c block_alloc` ties the output qty with the FIRST dying register operand. Source operand order is preserved in RTL. `combine_regs` FAILS for pseudos not local to one basic block.

**Fix**: declare the op1 variable at a scope where it is used in MORE THAN ONE basic block. The pseudos become global-allocated, the local tie on op1 fails, dst ties op2. General principle: old-C-style shared declarations are codegen-visible through BOTH loop rotation and local-alloc tying.

---

## Compiled switch jump table + INCLUDE_RODATA vtable: missing `.align 3` pad

**Symptom**: function diffs at score 0 but the ROM checksum fails; `cmp -l` shows a 4-byte shift in the vtable pointers.

**Fix**: add `.align 3` after `.section .rdata` in the `_vt.*.s` file. The original cc1plus emitted vtables 8-aligned. Note the file is regenerated by configure — re-check after any reconfigure.

---

## Paired s16/u16 fields written via computed offset are usually an ARRAY field

**Symptom**: target writes `left`/`right`-style halfword pairs through a strength-reduced byte offset, and the final address `addu` has operands swapped.

**Cause**: with a real array member (`u16 children[2]`), loop.c substitutes the reduced giv IN PLACE inside the address tree, preserving `(plus base giv)` operand order. Pointer-arithmetic formulations rebuild the address with the giv first.

**Fix**: declare the pair as an array field (`u16 children[2]`) and index it.

---

## Reassigned pointer variable blocks loop.c address hoisting

**Symptom**: your build hoists `&GLOBAL` (lui+addiu) into a callee-saved reg; target re-materializes it fresh in each block.

**Cause**: plain direct global refs create per-EBB address pseudos that loop.c merges and hoists. A user variable **assigned at more than one site** has `n_times_set > 1` → not an invariant movable → never hoisted.

**Fix**: one pointer variable, re-assigned per use site. Related idiom: `x` component accessed via the absolute `%lo(sym)(hi)` form while `y`/`z` go through the pointer — this falls out naturally whenever an address pseudo is live.

---

## Stack Vec3f aggregate forces real stores; f32 locals don't

**Symptom**: target stores three computed floats to consecutive stack slots then reloads some. Separate `f32 dx, dy, dz` locals keep them in regs and the stores vanish.

**Fix**: use a `Vec3f delta;` aggregate — GCC 2.7 never SRAs aggregates, so every component assignment is a real stack store.

---

## Giv init placement: `stack[depth]` vs pointer copy

**Symptom**: a stack-cursor init (`move a2,s1`) lands before hoisted loop-invariant constants in yours, after them in target.

**Cause**: a source-level `stackp = stack;` is a pre-loop statement; an indexed `stack[depth]` access makes the cursor a strength-reduction giv whose init is emitted at the END of the preheader, after moved invariants.

**Fix**: index by the counter instead of keeping a walking pointer. Sibling of "Indexed array-member writes vs walking pointer".

---

## In-loop `Vec3f* op = &local;` adds one real preheader-hoistable insn

**Symptom**: an in-loop `&stack_local` used once per iteration: with the address taken at the call site it folds into a hard-reg arg set (not movable, stays in-loop); the target differs on whether a nearby savings-1 movable clears the desirability gate, because the loop's `insn_count` differs by one.

**Fix**: declaring `Vec3f* op = &offset;` INSIDE the loop keeps the address a multi-use pseudo (stores via `op->y/z` + call arg) — it survives cse, counts as a loop insn, gets hoisted as a movable, and can push `insn_count` past the threshold so borderline movables get refused.

---

## Consume computed values before storing to globals — named local defers the stores

**Symptom**: computed values (e.g. cross product components) are stored to globals, then a dot product or comparison reads them back. Target defers the global stores until after the comparison, with one store landing in a branch delay slot.

**Cause**: the compiler schedules global stores as soon as their values are ready. A named local capturing the comparison result forces the compiler to keep the values in registers for the local's computation, deferring the stores.

**Fix**: compute the derived value into a named `f32` local BEFORE emitting the global stores, then branch on the local.

---

## Dead second set of a pointer var: blocks loop.c hoisting, but NOT reload remat

**Symptom**: a `%hi(global)` address is hoisted into a callee-saved register in your build; target re-materializes it fresh each iteration.

**Cause**: with `n_times_set == 1`, loop.c treats the address pseudo as an invariant movable and hoists it. A dead second assignment on a cold path raises `n_times_set` to 2 at reg_scan time (before flow), making the pseudo non-invariant. Flow later deletes the dead set, so reload still sees a single-set REG_EQUIV.

**Fix**: add a dead assignment to the pointer on a path that's never reached in the hot loop (before a `return`, in an error branch). This blocks hoisting without affecting final codegen.

---

## Redundant walking pointer alongside struct pointer: let giv-combining build the walker

**Symptom**: source with both a struct pointer `e` AND a second pointer to an inner field produces two walking pointers plus a derived register; target has one struct pointer and one combined base.

**Cause**: offset-0 uses of the second pointer are plain biv uses — they keep the biv alive so it can't be eliminated.

**Fix**: delete the second pointer entirely; access all fields through the struct pointer. loop.c's giv-combining merges the field accesses onto one reduced base register.

---

## `do { } while(0)` wrapper increases loop depth for reg_scan ref weighting

**Symptom**: two callee-saved pseudos are swapped (pure `r` markers, identical instruction sequence). Priorities are close — one pseudo has slightly more weighted refs than the other. No source-level lever (declaration order, type changes, expression rewrites) flips the priority.

**Cause**: GCC 2.7.2's `reg_scan` weights each ref by `2^loop_depth`. A `do { ... } while(0)` block emits `NOTE_INSN_LOOP_BEG` / `NOTE_INSN_LOOP_END` notes that increment the loop depth for refs inside it. The `while(0)` condition is always false, so no backward branch is emitted and no actual loop exists in the asm — but the depth increment is already committed when `reg_scan` counts refs. This bumps the weighted ref count for pseudos referenced inside the block, flipping a tight priority race.

**Fix**: wrap the block containing the lower-priority pseudo's uses in `do { ... } while(0)`. The pseudo's refs inside the wrapper get double weight, raising its priority above the competitor's. No asm instructions are added or changed — the wrapper is a pure compiler-internal loop-depth annotation.

---

*Add new tips as they're discovered. Each tip names its symptom, its cause, and its fix.*

# Investigation: do-while back-edge inversion (func_800279D8, cormorant)

**Status: SOLVED — score 0, sha1 OK.**

## Problem

`func_800279D8` (TGA RLE decompressor) was stuck at score 205: everything
matched except the RLE repeat-loop back-edge:

```
TARGET                          OURS (205)
988: bne  a3,t5,958   (back)    988: beq  a3,t5,9d0   (exit)
990: j    9d0         (exit)    990: j    958         (back)
```

## Method

Standalone harness (`t.cpp` + variants, this directory) compiled with the
production pipeline, plus per-pass RTL dumps:

```sh
mips-linux-gnu-cpp <build.ninja flags> -o t.i t.cpp
wibo tools/bin/cc1pln64.exe -quiet -G0 -O2 -da t.i -o t.s
# -> t.i.{rtl,jump,cse,loop,cse2,flow,combine,sched,lreg,greg,jump2,sched2,dbr}
```

Cross-referenced against stock gcc-2.7.2.2 sources (reorg.c, jump.c, stmt.c,
loop.c) — cc1pln64's behavior matched stock in every respect we probed.

## Findings (the full causal chain)

1. **The inversion happens in `reorg.c relax_delay_slots` (the `.dbr` pass).**
   All mid-end passes preserve the front end's `bne back; j exit`; reorg then
   applies: *"If this is an unconditional jump and the previous insn is a
   conditional jump, try reversing the condition ... and swapping our
   targets"*, gated on `no_labels_between_p` and `0 < mostly_true_jump(cond)`.
   A backward NE branch into a loop predicts 2 (LOOP_BEG scan) → always
   inverted. So a plain guarded do-while can never keep the target shape.

2. **The stable target shape needs the opposite orientation to be predicted
   taken.** `mostly_true_jump` returns 1 when the branch target label is
   followed by `NOTE_INSN_LOOP_VTOP` — "jump to the test of a loop". VTOP is
   emitted **only** by `jump.c duplicate_loop_exit_test` — i.e. the enclosing
   loop must itself be a compiler-rotated while/for. Then reorg *inverts our
   beq-exit form into the target's bne-back form* (the exit target = the outer
   loop's bottom test = VTOP-marked).

3. **`expand_end_loop` (stmt.c) rotation gates.** The C++ front end emits
   while/for top-tested; at loop close it rolls a leading exit test to the
   bottom, but the scan **breaks at `NOTE_INSN_BLOCK_BEG`** — emitted for any
   loop-body compound containing **declarations**. Empirical matrix (mwE–mwI):
   body `if (m) s += ...` rotates; `{ ... }` without decls rotates; any decl in
   the body (even deep, e.g. `u8 b` in an inner do-while) blocks rotation.
   So the original was written **old-C style: every local declared at function
   top**, which is exactly what lets both the outer `while` and the inner
   repeat `for` rotate.

4. **The duplicated exit test explains the register story.** The repeat loop's
   `for (count--; count != -1; count--)`: rotation + `duplicate_loop_exit_test`
   produce an entry copy of the test (whose `-1` CSEs with the function's other
   `-1` literals → t6) while the in-loop original keeps its own const pseudo
   (→ t5, `li t5,-1` hoisted into the guard's delay slot). Writing `mask`/`-1`
   as **literals** (not named locals) lets loop.c hoist their `li`s into the
   loop preheader — *after* the entry guard, matching the target layout.

5. **`move t0,a2` in the preheader = strength-reduction giv init.** The
   original has **no walking dstPtr at all**: it writes `dst[totalWritten]`,
   and loop.c's strength reduction creates the walking pointer (init in the
   outer preheader, `addiu t0,t0,1` at each store) while keeping
   `totalWritten` (t1) live because `savedPos` snapshots it.

## Final source shape (now in src/cormorant.cpp, score 0)

```cpp
void Cormorant::decompressTgaRle(u8* src, u8* dst) {
    // ALL locals declared at function top, no decls in any loop body
    // outer:  while (pixelPos < self->unk40)          — rotates, reloads unk40
    // stores: dst[totalWritten] = ...                 — giv-reduced to walker
    // repeat: for (count--; count != -1; count--) { j = 0; if (bpp) do..while }
    // consts: literal ~0x80 / -1 everywhere           — hoisted li's in preheader
}
```

## Reusable lessons (also distilled into the sn-codegen skill)

- A `bne back; j exit` loop tail is only stable when the exit target is the
  VTOP-marked bottom test of a rotated enclosing loop. If you can't get it,
  your loop nesting/source forms are wrong — don't fight the branch itself.
- Declarations inside loop bodies (at any depth) suppress while/for rotation.
  Old-C-style hoisted declarations are a *codegen-visible* property of the
  original source.
- `p[counter] = x` vs `*p++ = x`: the former leaves a `move base` giv init in
  the preheader; check where the walker's init lands.
- Debug flow: `-da` RTL dumps to find the *pass* that diverges, then read that
  pass in gcc-2.7.2.2 to find the *gate*, then design source to flip the gate.

## Files

- `t.cpp` — baseline harness (guarded do-while, score-205 form)
- `v2–v4, wa–wk` — variants; `wk.cpp` is the first fully-rotated form
- `mwE–mwI` (mini) — rotation-gate bisection battery

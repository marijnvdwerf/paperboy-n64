# Observed emission model for old SN/GCC-style N64 C++

This note records behavior observed while matching `otter.o` in the Paperboy 64 project. Treat it as a field guide, not a language-lawyer specification.

The preservation generator used for these experiments lives beside this note in
[`otter-codegen/`](otter-codegen/):

- `source.cpp` holds the canonical matched bodies.
- `gen_otter.py` re-emits those same bodies while varying placement, declaration order, source order, and pragma mode.

Its main job is preservation: once the bodies match, structural experiments
should not require hand-editing them again.

## Target shape from `otter.o`

The original object wanted:

```text
.rdata:
  jump table for vfunc5
  jump table for vfunc6
  Otter vtable
  jump tables for the remaining switch-bearing bodies

.text:
  vfunc5
  vfunc6
  vfunc8
  vfunc9
  vfunc7
  func_80049980
  vfunc4
  func_80049B50
  func_80049B58
  vfunc3
  vfunc2
  func_80049CC4
  ~Otter
  Otter
  func_80049DA0
  func_80049DAC
  func_80049DB4
```

## Experiments that matter

| Source shape | Observed result |
| --- | --- |
| All bodies ordinary `.cpp` definitions | Vtable appears only after the ordinary switch-table region; too late for `otter.o`. |
| Entire class header-inline, empty `.cpp` include only | TU emits essentially nothing. |
| Entire class header-inline + `#pragma interface` / `#pragma implementation` | TU owns the class machinery, but vtable appears first in `.rdata`; wrong seam for `otter.o`. |
| Externally-used six bodies ordinary in `.cpp`, all virtuals header-inline | TU exists, but vtable lands after only one jump table; cross-TU use explains ownership, not target order. |
| Ordinary `vfunc5`/`vfunc6`, rest class-inline | Exact layout possible, but source can become implausibly header-heavy. |
| Ordinary `vfunc5`/`vfunc6`, rest `inline` definitions in `.cpp` | Exact layout possible with declaration-only header. Deferred inline copies emit later in reverse source order. |
| Tiny accessors + ctor/dtor class-inline; heavy bodies `inline` in `.cpp`; `vfunc5`/`vfunc6` ordinary | Exact layout possible and reads plausibly. |

## Practical rules learned

### 1. Ordinary definitions form the front region

Ordinary out-of-line switch-bearing methods emit their code and jump tables in source order before the deferred-inline region. In `otter.o`, keeping only `vfunc5` and `vfunc6` ordinary creates exactly the two pre-vtable jump tables.

### 2. Deferred inline bodies are a second region

`inline` definitions in the `.cpp` and in-class definitions can be emitted later, after the vtable, often in reverse source/declaration order. This is why a source order like:

```cpp
inline DB4()
inline DAC()
inline DA0()
inline Otter()
inline ~Otter()
...
inline vfunc8()
```

can produce emitted text ending with:

```text
...
~Otter
Otter
DA0
DAC
DB4
```

### 3. `.rdata` follows emitted bodies, not necessarily source reading order

Jump tables for deferred inline bodies land where those bodies are emitted, not where their source text appears. If the code body matches but `.rdata` is wrong, inspect emission class before rewriting logic.

### 4. TU ownership and source ownership are different questions

Another TU may need:

- class layout
- member declarations
- callable external symbols

without forcing every virtual body to be ordinary out-of-line code in the same implementation TU.

### 5. Pragmas are real, but not magic

Old G++ pragmas can make a chosen TU own header-defined inline bodies and the vtable:

```cpp
#pragma interface
#pragma implementation "foo.h"
```

That mechanism is useful when header ownership is historically plausible. In the `Otter` probe, it emitted a full object but put the vtable before the jump tables, so it did not explain the target layout.

## Candidate families worth testing

When matching another TU, search these first:

1. all ordinary definitions
2. first one or two virtuals ordinary, later bodies `inline` in `.cpp`
3. tiny accessors / ctor / dtor class-inline, heavy bodies `inline` in `.cpp`
4. header-inline implementation with pragma ownership

Rank matching candidates by:

1. exact object order
2. ordinary human source style
3. consistency with external call sites
4. consistency with nearby codebase conventions

## Pademelon case study

The Pademelon TU confirmed and extended the Otter findings with a larger class (31 functions, 10 virtual overrides). A Windows build of the same library provided plausible source ordering.

### Key observations

1. **Float-constant placement reveals the vtable seam.** The original `.rdata` had the vtable before any float literals. Since ordinary definitions emit their float constants at their own `.rdata` position (before the vtable), every float-using function had to be deferred. This was the primary constraint.

2. **Windows build order is a plausible proxy for source order.** The Windows port of the same library compiled functions in an order that, when used as the source order hypothesis, produced an exact N64 `.text` + `.rdata` match. This doesn't prove it's the true source order, but it's a useful starting point when available.

3. **The "reversed N64 order" illusion.** The N64 `.text` order looked roughly reversed compared to the Windows order. This wasn't because the SN compiler reverses things — it's because most functions were `inline`/deferred, and deferred functions emit in reverse source order. A small set of ordinary definitions at the top emitted forward; the large deferred tail emitted backward.

4. **Three placement classes in one TU.** The matching arrangement used:
   - 6 **ordinary** out-of-line definitions (heavy functions: lookAt variants, matrix product, mirror)
   - 11 **`inline`** definitions in the `.cpp` (virtual overrides + some helpers, written in Windows source order)
   - 14 **in-class** definitions (constructor, reset, and small accessor helpers not present in the Windows build)

5. **In-class and `inline`-in-cpp defer identically.** Both emit after the vtable in reverse order. The choice between them is about plausibility: trivial accessors and constructors feel natural in-class; larger methods with clear signatures feel natural as `inline` in the `.cpp`.

6. **Deferred float constants cluster together.** All four float literals (three `-1.0f`, one `1.0f`) landed contiguously after the vtable, each at the emission point of their respective deferred function. Matching the constant count and order was a useful secondary check beyond just `.text` order.

### Pademelon harness

The Pademelon generator lives at `tools/gen_padmelon.py`:

```bash
python3 tools/gen_padmelon.py --preset win_split    # matching arrangement
python3 tools/gen_padmelon.py --list-presets         # see alternatives
```

### Header-split refinement (2026-08-04)

The matched arrangement was later refactored so consumers (wallaroo.cpp,
E7450.cpp) share a real `include/pademelon.h` instead of local partial
re-declarations, with `pademelon.o` staying byte-identical. Solver:
`tools/pademelon_sim.py` (jabiru-style; enumerated 78 surviving `(k, m)`
cuts). Two rules fell out:

1. **Header-in-class bodies must form a contiguous SUFFIX of the deferred
   emission order.** In-class bodies enqueue first (at class-definition
   time, i.e. at the `#include`), so their reversal is the emission TAIL.
   A candidate header split is therefore fully described by two cuts of the
   N64 order: `k` (ordinary prefix) and `m` (header-in-class suffix start);
   the middle `k+1..m-1` must be `inline`-in-cpp definitions. In-class
   bodies are declared in reverse-N64 order (31→m) so the tail comes out
   ascending.

2. **Any member a consumer TU calls as a real jal must stay body-hidden**
   (declaration in header, `inline` definition in the .cpp). A header body
   is textually visible before the consumer's caller, so rule 6 inlines it
   there and breaks that TU. This includes the implicit base-ctor call in a
   derived class's constructor — a header-inline ctor would inline into
   `Derived::Derived()`. Conversely, members the original build DID inline
   at consumer call sites (Pademelon's 12 tail accessors never exist
   standalone on PC) are evidence those bodies lived in the header — and
   future TUs may NEED that visibility to match.

Chosen split for Pademelon (k=6, m=20): 6 ordinary heavy bodies; ctor +
reset + the wallaroo-called helpers `inline` in the .cpp; the 12 tail
accessors (incl. vfunc17) in-class in the header. wallaroo.o byte-identical
after switching it to the header.

## Jabiru case study (probe-verified refinements)

The Jabiru TU (2026-08-03) replaced hand reasoning with a simulation solver
(`tools/jabiru_sim.py`) plus direct cc1pln64 probes (compile a tiny .cpp
through the project pipeline and read the .s — no assembling needed):

```bash
mips-linux-gnu-cpp -nostdinc -undef -D__GNUC__=2 -D__OPTIMIZE__ -lang-c++ \
  -o /tmp/p.i /tmp/p.cpp && wibo tools/bin/cc1pln64.exe -quiet -G0 -O2 /tmp/p.i -o /tmp/p.s
```

Refined rules (each probe-verified; they supersede vaguer statements above):

1. **One deferral queue, emitted reversed.** In-class bodies enqueue at
   class-definition time (declaration order); `inline`-in-cpp bodies at their
   definition; auto-generated dtors at class **completion** (not at `new`).
   Ordinary and deferred definitions may interleave lexically — only relative
   order within each group matters.
2. **Vtables emit in reverse class-declaration order** (class declared first
   → vtable emitted last of the vtables).
3. **Retention:** every inline member of a polymorphic class whose vtable the
   TU owns is materialized even if unreferenced. Unreferenced inline members
   of non-polymorphic classes, free inlines, and `extern "C"` inlines are
   dropped. To keep an unreferenced helper in the deferred region, make it a
   member of the vtable-owning class with an `asm("name")` alias.
4. **`extern "C" inline` referenced only from deferred code emits in a
   separate group at the very end** — wrong slot. Use an inline member with
   `asm("name")` instead.
5. **Second-wave demotion:** a deferred fn whose only references come from
   other deferred bodies emits in a late extra wave (possibly `local`). A
   reference from ordinary code restores its proper queue slot. An
   INCLUDE_ASM'd caller is invisible to the compiler, so tails look wrong
   until that caller is decompiled — expect this and don't chase it.
6. **Inlining is textual:** inline body defined before a caller is inlined
   into it (no practical size limit observed at -O2 — a 0x500-byte body
   inlined fine); defined after → real `jal`. Holds for deferred callers too.
   This lets a "define inline late" arrangement force real calls between
   deferred functions, and dictates where each inline def must sit relative
   to its callers.
7. **MSVC6 cross-check** (same-engine Windows build, CL under wibo):
   ordinary defs in strict source order (one COMDAT each); vftable +
   generated dtors + in-class virtual bodies batch at first materialization;
   inline bodies emit at first use; unreferenced inlines dropped. PC address
   order ≈ source order for the ordinary defs and first-use order for
   inlines — a strong prior, but EXE layout passes through COMDAT selection,
   so treat single-function anomalies as possibly link-order artifacts.

Method note: the solver inverts the target — a placement assignment dictates
the source order (front region forward, deferred region reversed), which is
then checked against the PC order. Run candidates past an adversarial review
before editing source; both reviews (opus + gpt) caught a reversal sign error
and the extern-"C"-inline trap that hand reasoning missed.

## Commands

```bash
nm -n build/ntsc/src/foo.o
uv run ./tools/dump-rdata.py build/ntsc/src/foo.o
ninja build/ntsc/src/foo.o
```

When automating a search, record at least:

- placement for each body
- source order
- class declaration order
- pragma mode
- symbol order
- `.rdata` label order

The Otter harness can be run from the repo root as:

```bash
python3 .claude/skills/n64-cpp-emission/references/otter-codegen/gen_otter.py --preset matching_mixed_inline_split
```

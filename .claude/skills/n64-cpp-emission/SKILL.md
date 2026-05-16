---
name: n64-cpp-emission
description: Use when matching old SN/GCC-derived N64 C++ translation units where `.text` order, switch jump-table `.rdata`, inline bodies, vtables, or `#pragma interface` / `#pragma implementation` affect object layout. Trigger on C++ matching-decomp work involving reordered rodata, key virtuals, inline emission, vtable placement, or puzzling TU ownership.
---

# N64 C++ Emission

Use this skill when the source already matches function assembly, but the object still misses because C++ emission order changed `.text` or `.rdata`.

This skill is empirical. Old GCC-family behavior varies by toolchain; prefer object-file experiments over modern assumptions.

## Quick workflow

1. Capture the target object shape.
   - Dump original `.rdata` labels/order.
   - Dump candidate object `.rdata`.
   - Inspect symbol order with `nm -n`.
2. Classify each definition by emission style:
   - ordinary out-of-line definition in `.cpp`
   - `inline` definition in `.cpp`
   - in-class / header inline definition
   - pragma-controlled header implementation
3. Change one lever at a time and rebuild only the object first.
   - definition site
   - `inline` keyword
   - source order
   - class declaration order
   - pragma mode
4. Compare full object order, not just checksum success.
5. When several shapes match, prefer the one a human plausibly wrote.

Useful commands:

```bash
nm -n build/ntsc/src/foo.o
uv run ./tools/dump-rdata.py build/ntsc/src/foo.o
ninja build/ntsc/src/foo.o
ninja
```

## Core model

- Ordinary out-of-line definitions emit in source order.
- Switch-bearing definitions contribute jump-table `.rdata` at their own emission point.
- Deferred inline copies may emit later than their source location, often in reverse order.
- A vtable may appear between the ordinary-definition region and the deferred-inline region.
- Header-only source does not guarantee any object output; if the TU has no materializing trigger, it may emit nothing.
- `#pragma interface` / `#pragma implementation` changes which TU owns inline copies and the vtable, but may also move the vtable earlier than expected.

For the detailed behavior observed in this workspace, read [references/emission-model.md](references/emission-model.md).
For the concrete Otter preservation harness that kept the matched bodies stable
while exploring layout changes, inspect
[`references/otter-codegen/`](references/otter-codegen/).

## Matching heuristics

### If the vtable moved too late

Some definitions that were ordinary before the vtable may actually need to be deferred inline copies. Try making later bodies `inline` in the `.cpp` or moving cheap bodies into the class definition.

### If the vtable moved too early

Too much may be owned by the header / pragma implementation path. Try restoring one or more ordinary out-of-line virtuals before the deferred region.

### If `.rdata` count is right but order is wrong

You probably found the right emission classes but the wrong source order. Reverse the source order for deferred inline bodies and inspect `nm -n` again.

### If another TU uses the class

Separate “needs the class declaration/layout” from “causes this TU to emit code.” Consumers often require only declarations; they do not prove where the inline bodies or vtable came from.

## Pragmas

Treat old G++ pragmas as a lever, not a default explanation:

```cpp
// header
#pragma interface

// chosen implementation TU
#pragma implementation "foo.h"
#include "foo.h"
```

Use them when the class seems genuinely header-defined but one TU must own the vtable and inline copies. Verify the resulting object order; pragma ownership can be correct while `.rdata` placement is still wrong.

## Search strategy

When hand reasoning stalls, build a preservation harness first: keep the matched
bodies in one source of truth, then regenerate only the structural choices you
want to vary. The Otter reference under
[`references/otter-codegen/`](references/otter-codegen/) is a worked example:

```text
body text  ×  placement  ×  source order  ×  class order  ×  pragma mode
```

Then:

1. generate one candidate
2. build the object
3. compare `nm -n` and `.rdata`
4. keep matching candidates
5. rank by source plausibility

Do not stop at the first matching arrangement if it only works by making the source absurd.

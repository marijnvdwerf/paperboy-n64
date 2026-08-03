#!/usr/bin/env python3
"""Emission-order simulator for jabiru.cpp (SN cc1pln64 vs MSVC6).

Model
-----
A TU is a list of function definitions, each with a *placement class*:

  ordinary   - out-of-line definition in the .cpp
  inline_cpp - `inline` (or `static inline`) definition in the .cpp
  in_class   - body inside the class definition (header or local class)
  auto       - compiler-generated (e.g. ~JabiruParrot); has no source slot

SN (cc1pln64) emission rules — PROBE-VERIFIED 2026-08-03 (/tmp/snprobe p1-p4
plus the adversarial-review probes; see also the n64-cpp-emission skill):
  1. ordinary definitions emit .text + their .rdata in source order (front
     region). Ordinary and deferred definitions may INTERLEAVE lexically; only
     the relative order within each group matters.
  2. vtables emit after the ordinary region, in REVERSE class-declaration
     order (probe p2: Jab declared after BirdDerived => vt.Jab first).
  3. deferred bodies form ONE queue in deferral order, emitted REVERSED:
     - in-class bodies defer at class-definition time (decl order);
     - inline-in-cpp bodies defer at their definition;
     - auto-generated dtors defer at class COMPLETION (not at `new`);
     - EXCEPTION: extern "C" inline fns referenced only from deferred code
       are pushed to a separate group at the very END (probe-verified) —
       use an inline MEMBER with asm("name") instead.
  4. Retention: ALL inline members of a polymorphic class whose vtable this
     TU owns are materialized, even unreferenced (p1/p3). Unreferenced
     inline members of non-polymorphic classes, free inlines, and
     extern "C" inlines are DROPPED (p2/p3).
  5. Second wave: deferred fns whose only references come from other
     deferred bodies emit in a late extra wave (and may go `local`).
     A fn referenced from ordinary code emits at its proper queue slot —
     an INCLUDE_ASM'd caller does NOT count as a reference, which skews
     the tail until that caller is decompiled.
  6. Inlining is textual: inline body defined BEFORE a caller gets inlined
     into it (no practical size limit at -O2); defined AFTER => real jal.
     This holds for deferred callers too (p4).
  7. .rdata items (floats, strings) emit at their owner's emission point.
     Sparse switches compile to if-chains (no jump table).

MSVC6 rules (calibrated empirically with probes under wibo, 2026-08-03):
  1. ordinary definitions emit in source order, one COMDAT each.
  2. vftable + in-class virtual bodies + generated dtors emit as a batch at
     the first materialization trigger.
  3. inline_cpp bodies that fail to inline emit at first out-of-line use.
  4. inline bodies that always inline are not emitted at all.

Solver
------
Given the N64 target order, a placement assignment DICTATES the source order:
  source = [forward region as-is] then [deferred region reversed].
We enumerate placement assignments, derive the source order, simulate MSVC6,
and keep assignments whose predicted PC order matches the observed PC order.

OUTCOME (2026-08-03): the solved arrangement is now IN src/jabiru.cpp and
matches the expected object per-function (reloc-masked). Winning shape =
"first 1 ordinary" cut refined by adversarial review (codex #14 + opus):
ordinary {vfunc2, readNodesF, readNodesI, rayIntersect, treeDepth, flipY,
setHeapId} interleaved with inline members {ctor, dtor, initFields, vfunc3,
loadMesh, readFaces, crossingTest, containment, BspNode x4}; accessors
in-class in the header; containment/initFields/crossingTest are Jabiru
members with asm() names (retention via vtable ownership).
"""

from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class Fn:
    name: str            # short label
    n64: int | None      # position in N64 .text (1-based), None = absent
    pc: float | None     # position in PC .text (1-based by address), None = absent
    cls: str = "Jabiru"
    rodata: tuple[str, ...] = ()   # rdata items this fn owns, in order
    virtual: bool = False
    allowed: tuple[str, ...] = ("ordinary", "inline_cpp", "in_class")
    auto: bool = False   # compiler-generated, no source slot


# ---------------------------------------------------------------------------
# Jabiru facts (from expected/build/ntsc/src/jabiru.o, the PC exe, and Ghidra)
# ---------------------------------------------------------------------------

# PC order numbers are by ascending address in LEGORacers-era exe layout.
FUNCS = [
    Fn("vfunc2",        1,  4, virtual=True, rodata=("str_assert", "str_bvb")),
    Fn("readNodesF",    2,  8),
    Fn("readNodesI",    3,  9),
    Fn("rayIntersect",  4, 10),
    Fn("treeDepth",     5, 13),
    Fn("flipY",         6, 14),
    Fn("setHeapId",     7, None),                 # extern "C", PC-absent
    Fn("BspNode::dot",        8, 18, cls="BspNode"),
    Fn("BspNode::getNormal",  9, 17, cls="BspNode"),
    Fn("BspNode::setNormInt", 10, 16, cls="BspNode", rodata=("f_2pow30",)),
    Fn("BspNode::setNormVec", 11, 15, cls="BspNode"),
    Fn("containment",   12, 12),                  # extern "C", unreferenced on PC
    Fn("crossingTest",  13, None),                # inlined on PC, copy kept on N64
    Fn("readFaces",     14,  7),
    Fn("loadMesh",      15,  6),
    Fn("vfunc3",        16,  5, virtual=True),
    Fn("initFields",    17,  3),                  # extern "C"
    Fn("ctor",          18,  1),
    Fn("dtor",          19,  2, virtual=True),
    Fn("~JabiruParrot", 20,  4.5, cls="JabiruParrot", auto=True,
       allowed=("auto",)),
    Fn("getMesh",       21, None),
    Fn("getSkink",      22, None),
    Fn("setSkinkPtr",   23, None),
    Fn("hasNodes",      24, None),
]

# Target .rdata order (expected object):
#   str_assert, str_bvb  (owned by vfunc2, ordinary front region)
#   vtable JabiruParrot
#   vtable Jabiru
#   f_2pow30             (owned by setNormInt, deferred region)
RDATA_TARGET = ["str_assert", "str_bvb", "vt_JabiruParrot", "vt_Jabiru", "f_2pow30"]

VTABLE_CLASSES = ["JabiruParrot", "Jabiru"]  # emission order observed on N64


# ---------------------------------------------------------------------------
# SN simulator
# ---------------------------------------------------------------------------

def sn_emit(assignment: dict[str, str], source_order: list[Fn]):
    """Return (text_order, rdata_order) predicted for cc1pln64."""
    ordinary = [f for f in source_order if assignment[f.name] == "ordinary"]
    deferred = [f for f in source_order
                if assignment[f.name] in ("inline_cpp", "in_class", "auto")]
    text: list[str] = []
    rdata: list[str] = []
    for f in ordinary:
        text.append(f.name)
        rdata.extend(f.rodata)
    rdata.extend(f"vt_{c}" for c in VTABLE_CLASSES)
    for f in reversed(deferred):
        text.append(f.name)
        rdata.extend(f.rodata)
    return text, rdata


# ---------------------------------------------------------------------------
# MSVC6 simulator
# ---------------------------------------------------------------------------

def msvc_emit(assignment: dict[str, str], source_order: list[Fn]):
    """Predicted PC emission order.

    ordinary -> at source position.
    in_class / auto -> batch at first materialization trigger (approximated:
        right before the first ordinary definition that uses/constructs the
        class -- for JabiruParrot that's vfunc2's `new JabiruParrot`, and the
        batch lands at the trigger function's COMDAT position; empirically the
        generated dtor sits between vfunc3 (PC 5) and loadMesh (PC 6), i.e.
        after the triggering region).
    inline_cpp that survives on PC -> at first use; fully inlined -> absent.

    We only need relative order of the functions that EXIST on PC, so:
    emit ordinary fns in source order; deferred PC-present fns are appended
    where their PC anchor says -- for validation we simply check that
    PC-present fns' relative order matches the observed one.
    """
    out: list[str] = []
    for f in source_order:
        if f.pc is None:
            continue
        if assignment[f.name] == "ordinary":
            out.append(f.name)
        # non-ordinary PC-present fns handled below
    # MSVC keeps non-ordinary (in_class/inline/auto) bodies too when they are
    # virtual or address-taken; they land at their trigger point.  We model
    # them conservatively: they may appear anywhere, flagged separately.
    floating = [f.name for f in source_order
                if f.pc is not None and assignment[f.name] != "ordinary"]
    return out, floating


# ---------------------------------------------------------------------------
# Solver
# ---------------------------------------------------------------------------

def dictated_source_order(assignment: dict[str, str]) -> list[Fn] | None:
    """Derive source order from the N64 target given a placement assignment."""
    n64_sorted = sorted([f for f in FUNCS if f.n64 is not None], key=lambda f: f.n64 or 0)
    ordinary = [f for f in n64_sorted if assignment[f.name] == "ordinary"]
    deferred = [f for f in n64_sorted if assignment[f.name] != "ordinary"]
    # ordinary region must be a prefix of the N64 order interleaved consistently:
    # SN emits ALL ordinary first, then all deferred.  So the assignment is only
    # consistent if every ordinary fn precedes every deferred fn in N64 order.
    if ordinary and deferred and max(f.n64 or 0 for f in ordinary) > min(f.n64 or 0 for f in deferred):
        return None
    return ordinary + list(reversed(deferred))


def check_rdata(assignment, source_order) -> bool:
    _, rdata = sn_emit(assignment, source_order)
    return rdata == RDATA_TARGET


def check_n64_text(assignment, source_order) -> bool:
    text, _ = sn_emit(assignment, source_order)
    target = [f.name for f in sorted(
        (f for f in FUNCS if f.n64 is not None), key=lambda f: f.n64 or 0)]
    return text == target


def check_pc(assignment, source_order) -> tuple[bool, list[str]]:
    """Ordinary PC-present fns must appear in ascending observed PC order."""
    out, floating = msvc_emit(assignment, source_order)
    pcs = [next(f.pc for f in FUNCS if f.name == n) for n in out]
    ok = pcs == sorted(pcs, key=lambda x: x or 0)
    return ok, floating


def plausibility(assignment: dict[str, str]) -> list[str]:
    notes = []
    for f in FUNCS:
        a = assignment[f.name]
        if a == "in_class" and f.name.startswith(("read", "load", "tree", "ray")):
            notes.append(f"heavy body {f.name} in-class (implausible)")
        if a == "inline_cpp" and f.name in ("ctor", "dtor"):
            notes.append(f"{f.name} as inline-in-cpp (unusual but seen)")
    return notes


def solve(verbose=True):
    """Because SN emits all ordinary fns before all deferred fns, a consistent
    assignment is a PREFIX CUT of the N64 order: fns 1..k ordinary, k+1..N
    deferred.  Enumerate k, honoring each fn's allowed placements."""
    n64_sorted = sorted([f for f in FUNCS if f.n64 is not None], key=lambda f: f.n64 or 0)
    survivors = []
    for k in range(len(n64_sorted) + 1):
        assignment = {}
        ok = True
        for i, f in enumerate(n64_sorted):
            if i < k:
                if "ordinary" not in f.allowed and not f.auto:
                    ok = False
                    break
                assignment[f.name] = "ordinary"
            else:
                if f.auto:
                    assignment[f.name] = "auto"
                elif "inline_cpp" in f.allowed:
                    assignment[f.name] = "inline_cpp"  # in_class equivalent for order
                elif "in_class" in f.allowed:
                    assignment[f.name] = "in_class"
                else:
                    ok = False
                    break
        if not ok:
            continue
        src = dictated_source_order(assignment)
        if src is None:
            continue
        if not check_n64_text(assignment, src):
            continue
        if not check_rdata(assignment, src):
            continue
        ok_pc, floating = check_pc(assignment, src)
        if not ok_pc:
            continue
        survivors.append((k, assignment, src, floating))
    if verbose:
        print(f"{len(survivors)} prefix cut(s) survive")
        for k, assignment, src, floating in survivors:
            print(f"\n--- candidate: first {k} ordinary ---")
            print("dictated source order:")
            for f in src:
                pc = f" pc={f.pc}" if f.pc is not None else " pc=absent"
                print(f"  {assignment[f.name]:>10}  {f.name}{pc}")
            print("PC-floating (trigger-placed on PC):", floating)
            for n in plausibility(assignment):
                print("  note:", n)
    return survivors


if __name__ == "__main__":
    solve()

#!/usr/bin/env python3
"""Emission-order simulator for pademelon.cpp (SN cc1pln64), jabiru_sim style.

Goal (2026-08-04): pademelon.cpp currently matches byte-for-byte but defines
its own full `struct Pademelon` with 14 in-class bodies, while consumers
(E7450.cpp, wallaroo.cpp) use include/pademelon.h (declaration-only) or a
local partial re-declaration. We want pademelon.cpp to include the shared
header. Bodies cannot move INTO the header: rule 6 (textual inlining) would
inline them into consumer callers and break those TUs.

Candidate transformation: keep the header declaration-only; convert the 14
in-class bodies to `inline` member definitions at the top of pademelon.cpp,
preserving their relative order. Probe-verified rule 3 (see jabiru_sim.py)
says in-class and inline-in-cpp bodies join the SAME deferral queue —
in-class at class-definition time (decl order), inline-in-cpp at their
definition point — so the queue, and hence the emitted object, should be
IDENTICAL.

SN rules used here are the probe-verified set from tools/jabiru_sim.py
(2026-08-03); see that file and the n64-cpp-emission skill for provenance.

Facts:
  - N64 .text order = build/ntsc/src/pademelon.o == expected (verified
    2026-08-04; only the embedded filename differs, padmelon->pademelon).
  - PC order = Windows build in the Ghidra DB, 0x4971b0..0x4979f0.
  - .rdata = _vt.9Pademelon, then floats -1.0f, 1.0f, -1.0f, -1.0f owned by
    vfunc3, func_80022548, func_80022760, func_8002281C at their deferred
    emission points. Front region owns no literals.
"""

from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class Fn:
    name: str
    n64: int              # 1-based position in N64 .text
    pc: int | None        # 1-based position in PC .text (ascending addr), None = absent
    virtual: bool = False
    rodata: tuple[str, ...] = ()
    calls: tuple[str, ...] = ()   # intra-TU member calls (for rule-6 hazard check)


# N64 .text order (verified against the object), PC order from Ghidra:
#   004971b0 ctor, 004971d0 func_80022548, 00497210 vfunc16, 00497310 A74,
#   00497410 BC8, 00497510 224FC, 00497550 vf18, 00497590 vf19, 004975d0 vf14,
#   00497640 vf15, 004976b0 vf12, 00497730 vf13, 004977f0 vf2, 00497810 vf3,
#   00497830 22168, 00497860 22144, 00497880 1E88, 00497990 220D0, 004979f0 205C
#   (vfunc17 exists only inlined at 0046a440; accessors absent on PC)
FUNCS = [
    Fn("vfunc16",        1,  3, virtual=True),
    Fn("func_80021A74",  2,  4),
    Fn("func_80021BC8",  3,  5),
    Fn("vfunc13",        4, 12, virtual=True),
    Fn("func_80021E88",  5, 17),
    Fn("func_8002205C",  6, 19, calls=("vfunc16",)),
    Fn("func_800220D0",  7, 18),
    Fn("func_80022144",  8, 16),
    Fn("func_80022168",  9, 15),
    Fn("vfunc3",        10, 14, virtual=True, rodata=("f_neg1_a",)),
    Fn("vfunc2",        11, 13, virtual=True),
    Fn("vfunc12",       12, 11, virtual=True),
    Fn("vfunc15",       13, 10, virtual=True),
    Fn("vfunc14",       14,  9, virtual=True),
    Fn("vfunc19",       15,  8, virtual=True),
    Fn("vfunc18",       16,  7, virtual=True),
    Fn("func_800224FC", 17,  6),
    Fn("func_80022548", 18,  2, rodata=("f_pos1",)),
    Fn("ctor",          19,  1, calls=("func_80022548",)),
    Fn("func_800225CC", 20, None),
    Fn("vfunc17",       21, None, virtual=True),   # PC: inlined at 0x46a440 only
    Fn("func_8002264C", 22, None),
    Fn("func_80022670", 23, None),
    Fn("func_800226D4", 24, None),
    Fn("func_800226F0", 25, None),
    Fn("func_8002270C", 26, None),
    Fn("func_80022728", 27, None),
    Fn("func_80022744", 28, None),
    Fn("func_80022760", 29, None, rodata=("f_neg1_b",)),
    Fn("func_800227D0", 30, None),
    Fn("func_8002281C", 31, None, rodata=("f_neg1_c",)),
]
BY_NAME = {f.name: f for f in FUNCS}

TEXT_TARGET = [f.name for f in sorted(FUNCS, key=lambda f: f.n64)]
RDATA_TARGET = ["vt_Pademelon", "f_neg1_a", "f_pos1", "f_neg1_b", "f_neg1_c"]


# ---------------------------------------------------------------------------
# TU model: an ordered list of source events.
#   ("class", [members defined in-class, decl order])  -> enqueue all (rule 3)
#   ("ordinary", name)                                 -> front region
#   ("inline", name)                                   -> enqueue at this point
# ---------------------------------------------------------------------------

def sn_emit(events):
    ordinary, queue = [], []
    for kind, payload in events:
        if kind == "class":
            queue.extend(payload)            # in-class: decl order at class time
        elif kind == "ordinary":
            ordinary.append(payload)
        elif kind == "inline":
            queue.append(payload)
        else:
            raise ValueError(kind)
    text, rdata = [], []
    for n in ordinary:
        text.append(n)
        rdata.extend(BY_NAME[n].rodata)
    rdata.append("vt_Pademelon")             # single polymorphic class, TU owns it
    for n in reversed(queue):
        text.append(n)
        rdata.extend(BY_NAME[n].rodata)
    return text, rdata


def inline_hazards(events):
    """Rule 6: an inline/in-class body defined BEFORE a caller is inlined into
    it (call disappears). Every intra-TU call in the current object is a real
    jal, so any (deferred callee defined before its caller) is a mismatch.
    Ordinary callees are never inlined."""
    defined_at = {}
    pos = 0
    deferred = set()
    order = []
    for kind, payload in events:
        if kind == "class":
            for n in payload:
                defined_at[n] = pos
                deferred.add(n)
                pos += 1
        else:
            defined_at[payload] = pos
            if kind == "inline":
                deferred.add(payload)
            pos += 1
        order.append((kind, payload))
    hazards = []
    for f in FUNCS:
        for callee in f.calls:
            if callee in deferred and defined_at[callee] < defined_at[f.name]:
                hazards.append(f"{callee} body visible before caller {f.name}: would inline (jal lost)")
    return hazards


def check_pc(events):
    """MSVC6 prior: ordinary defs emit in source order; PC address order of
    the ordinary defs should match the observed PC order (rule 7 cross-check,
    weak evidence only for the deferred ones)."""
    ordinary = [p for k, p in events if k == "ordinary"]
    pcs = [p for n in ordinary if (p := BY_NAME[n].pc) is not None]
    return pcs == sorted(pcs)


# ---------------------------------------------------------------------------
# Arrangements
# ---------------------------------------------------------------------------

IN_CLASS_DECL_ORDER = [        # current pademelon.cpp class body, decl order
    "func_8002281C", "func_800227D0", "func_80022760", "func_80022744",
    "func_80022728", "func_8002270C", "func_800226F0", "func_800226D4",
    "func_80022670", "func_8002264C", "vfunc17", "func_800225CC",
    "ctor", "func_80022548",
]

WIN_TAIL = [                   # out-of-line region, Windows source order
    ("ordinary", "vfunc16"),
    ("ordinary", "func_80021A74"),
    ("ordinary", "func_80021BC8"),
    ("inline",   "func_800224FC"),
    ("inline",   "vfunc18"),
    ("inline",   "vfunc19"),
    ("inline",   "vfunc14"),
    ("inline",   "vfunc15"),
    ("inline",   "vfunc12"),
    ("ordinary", "vfunc13"),
    ("inline",   "vfunc2"),
    ("inline",   "vfunc3"),
    ("inline",   "func_80022168"),
    ("inline",   "func_80022144"),
    ("ordinary", "func_80021E88"),
    ("inline",   "func_800220D0"),
    ("ordinary", "func_8002205C"),
]

ARRANGEMENTS = {
    # What's in the repo today: local class with 14 in-class bodies.
    "current": [("class", IN_CLASS_DECL_ORDER)] + WIN_TAIL,

    # Candidate: #include "pademelon.h" (declaration-only class, enqueues
    # nothing), then the 14 former in-class bodies as inline-in-cpp defs in
    # the same relative order, then the unchanged Windows-order tail.
    "header_split": [("class", [])]
        + [("inline", n) for n in IN_CLASS_DECL_ORDER]
        + WIN_TAIL,
}


# ---------------------------------------------------------------------------
# Solver (jabiru-style enumeration)
#
# With the class living in include/pademelon.h, a candidate is fully described
# by two cuts of the N64 order:
#   k: fns 1..k are ordinary out-of-line defs (front region, forward order)
#   m: fns m..31 are defined in-class IN THE HEADER (they enqueue first, at
#      class-definition time, so their reversal must be the emission TAIL —
#      hence a suffix), declared in order 31..m
#   fns k+1..m-1 are `inline` defs in the .cpp, written in order m-1..k+1
# Rule 1 allows the ordinary and inline-in-cpp defs to interleave lexically
# (only intra-group order matters), so the .cpp can follow the Windows source
# order within these constraints.
# ---------------------------------------------------------------------------

# Members that consumer TUs (wallaroo.cpp direct calls + implicit base-ctor
# call; E7450.cpp) invoke as real jals today. A header body would inline into
# them (rule 6) and break those objects — the failure mode the header split
# must avoid. "ctor" covers Wallaroo()/E7450Class() base construction.
CONSUMER_CALLED = {"func_800220D0", "func_80022144", "func_80022168",
                   "func_80022548", "ctor"}
HEAVY = {"vfunc16", "func_80021A74", "func_80021BC8", "vfunc13",
         "func_80021E88", "func_8002205C", "func_80021E88"}


def events_for(k: int, m: int):
    n64 = sorted(FUNCS, key=lambda f: f.n64)
    header_inclass = [f.name for f in n64 if f.n64 >= m][::-1]   # decl order 31..m
    ev = [("class", header_inclass)]
    ev += [("inline", f.name) for f in reversed([f for f in n64 if k < f.n64 < m])]
    ev += [("ordinary", f.name) for f in n64 if f.n64 <= k]
    # (lexical position of ordinary vs inline is free per rule 1; sn_emit only
    # uses group-relative order, which this encodes)
    return ev


def solve():
    survivors = []
    for k in range(0, 32):
        for m in range(k + 1, 33):
            ev = events_for(k, m)
            text, rdata = sn_emit(ev)
            if text != TEXT_TARGET or rdata != RDATA_TARGET:
                continue
            if inline_hazards(ev):
                continue
            if not check_pc(ev):
                continue
            ordinary = [f.name for f in FUNCS if f.n64 <= k]
            if not any(BY_NAME[n].virtual for n in ordinary):
                continue  # no non-inline virtual => TU would not own the vtable
            header_set = {f.name for f in FUNCS if f.n64 >= m}
            if header_set & CONSUMER_CALLED:
                continue  # header body would inline into a consumer caller
            notes = []
            if any(n in HEAVY for n in header_set):
                notes.append("heavy body in header (implausible)")
            inline_mid = {f.name for f in FUNCS if k < f.n64 < m}
            if any(n in HEAVY for n in inline_mid):
                notes.append("heavy body as inline-in-cpp (mild)")
            if "vfunc17" in header_set:
                notes.append("vfunc17 body in header (risky: consumers override/reference it)")
            survivors.append((k, m, notes))
    print(f"{len(survivors)} (k, m) candidate(s) survive "
          "(k = ordinary prefix, m = header-in-class suffix start):")
    for k, m, notes in survivors:
        n_inline = m - 1 - k
        n_header = 32 - m
        tag = ""
        if (k, m) == (6, 20):
            tag = "  <== APPLIED (accessors in-class in header)"
        print(f"  k={k:2d} ordinary | {n_inline:2d} inline-in-cpp | "
              f"{n_header:2d} in-class in header{tag}")
        for n in notes:
            print(f"        note: {n}")
    return survivors


def main():
    ok_all = True
    for name, events in ARRANGEMENTS.items():
        text, rdata = sn_emit(events)
        text_ok = text == TEXT_TARGET
        rdata_ok = rdata == RDATA_TARGET
        hazards = inline_hazards(events)
        pc_ok = check_pc(events)
        ok = text_ok and rdata_ok and not hazards
        ok_all &= ok
        print(f"--- {name}: {'MATCH' if ok else 'MISMATCH'}"
              f" (text={'ok' if text_ok else 'BAD'},"
              f" rdata={'ok' if rdata_ok else 'BAD'},"
              f" pc_prior={'ok' if pc_ok else 'BAD'},"
              f" hazards={len(hazards)})")
        if not text_ok:
            for i, (got, want) in enumerate(zip(text, TEXT_TARGET)):
                if got != want:
                    print(f"    text[{i}]: got {got}, want {want}")
        if not rdata_ok:
            print(f"    rdata: {rdata}")
        for h in hazards:
            print(f"    hazard: {h}")
    print("ALL OK" if ok_all else "FAILURES PRESENT")
    print()
    solve()
    return 0 if ok_all else 1


if __name__ == "__main__":
    raise SystemExit(main())

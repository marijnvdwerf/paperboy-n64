#!/usr/bin/env python3
"""
Generate fennec.cpp with different function placement/ordering strategies.

Usage:
    python3 tools/gen_fennec.py [--preset NAME]
    python3 tools/gen_fennec.py --list-presets
"""
import argparse, sys
from collections import OrderedDict

FUNCS = OrderedDict()

def F(n64, sym, sig, body, *, virtual=False, new_virtual=False):
    """Register a function. sig/body are always member-style (no self param)."""
    FUNCS[n64] = dict(n64=n64, sym=sym, sig=sig, body=body, virtual=virtual, new_virtual=new_virtual)

# ---- virtual overrides ----

F(1, "vfunc9", "s32 vfunc9()", virtual=True, body="""\
if (unk54 & 4) {
    unk34 = unk38->entries[unk56];
}
return unk34->flags & 0x1100;""")

F(2, "vfunc8", "void vfunc8(SceneRenderer* scene)", virtual=True, body="""\
((RenderContext*)scene)->vfunc45((UNK)this);""")

F(5, "vfunc6", "void vfunc6(Vec3f* frustum, DingoFrustumResult* out)", virtual=True, body="""\
Vec3f myPos;
func_80122414(&myPos);
out->pad = 0;
f32 radius = unk50;
if (radius != 9.999999934e+36f) {
    Vec3f* p = &myPos;
    f32 dx = frustum->x - p->x;
    f32 dy = frustum->y - p->y;
    f32 dz = frustum->z - p->z;
    f32 distSq = dx * dx + dy * dy + dz * dz;
    if (distSq == 0.0f) {
        out->result = 0;
        return;
    }
    if (radius < distSq) {
        out->result = 0;
        return;
    }
}
out->result = func_80029480(frustum, &myPos, func_801223AC());""")

F(6, "vfunc21", "void vfunc21()", virtual=True, new_virtual=True, body="""\
unk34 = NULL;
unk54 = 0;
unk48 = 0;
unk4C = 0;""")

F(8, "vfunc20", "void vfunc20(void* obj, f32 width, f32 height, f32 radius)", virtual=True, new_virtual=True, body="""\
f32 hw = width * 0.5f;
f32 hh = height * 0.5f;
unk34 = (FennecEntry*)obj;
unk54 = 1;
unk48 = width;
unk4C = height;
unk50 = radius;
func_80122330(sqrtf(hw * hw + hh * hh));""")

F(15, "vfunc3", "void vfunc3(Vec3f* v)", virtual=True, body="""\
func_80122354(v);""")

# ---- non-virtual members ----

F(3, "func_801251EC", "FennecEntry* func_801251EC()", body="""\
if (unk54 & 4) {
    unk34 = unk38->entries[unk56];
}
return unk34;""")

F(4, "func_80125224", "void func_80125224(Vec3f* point, DingoFrustumResult* out)", body="""\
Vec3f myPos;
out->pad = 0;
func_80122414(&myPos);
Vec3f* p = &myPos;
f32 dx = point->x - p->x;
f32 dy = point->y - p->y;
f32 dz = point->z - p->z;
f32 distSq = dx * dx + dy * dy + dz * dz;
if (distSq == 0.0f) {
    out->result = 0;
    return;
}
if (unk50 < distSq) {
    out->result = 0;
    return;
}
out->result = 1;""")

F(7, "func_801253CC", "void func_801253CC(FennecAnimData* data, s32 index, f32 width, f32 height, f32 radius)", body="""\
unk56 = index;
unk38 = data;
FennecEntry* entry = data->entries[index];
vfunc20(entry, width, height, radius);
unk54 |= 4;""")

F(9, "ctor", "Fennec()", body="""\
unk34 = NULL;
unk38 = NULL;
unk3C.x = 0;
unk3C.y = 0;
unk54 = 0;
unk56 = 0;
unk48 = 0;
unk4C = 0;
unk3C.z = 1.0f;
unk50 = 9.999999934e+36f;""")

F(10, "func_80125524", "void* func_80125524()", body="""\
return unk38;""")

F(11, "func_80125530", "void func_80125530(f32 val)", body="""\
unk50 = val;""")

F(12, "func_80125538", "f32 func_80125538()", body="""\
return unk50;""")

F(13, "func_80125544", "void func_80125544()", body="""\
unk54 &= ~2;""")

F(14, "func_80125554", "void func_80125554(Vec3f* v)", body="""\
unk54 |= 2;
unk3C = *v;""")

F(16, "func_8012559C", "void func_8012559C(f32 val)", body="""\
unk4C = val;""")

F(17, "func_801255A4", "void func_801255A4(f32 val)", body="""\
unk48 = val;""")

F(18, "func_801255AC", "void func_801255AC(Vec3f* out)", body="""\
func_80122414(out);""")

F(19, "func_801255C8", "f32 func_801255C8()", body="""\
return unk4C;""")

F(20, "func_801255D4", "f32 func_801255D4()", body="""\
return unk48;""")

F(21, "func_801255E0", "void func_801255E0(FennecEntry* val)", body="""\
unk34 = val;""")

F(22, "func_801255E8", "s32 func_801255E8()", body="""\
return unk54 & 4;""")

F(23, "func_801255F4", "s32 func_801255F4()", body="""\
return unk54 & 2;""")

F(24, "func_80125600", "u16 func_80125600()", body="""\
return unk54;""")

# ---------- code generation ----------

PREAMBLE = """\
#include "common.h"
#include "dingo.h"
#include "render_context.h"

extern "C" s32 func_80029480(void* frustum, Vec3f* point, f32 radius);

extern "C" f32 sqrtf(f32);

struct FennecEntry {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 flags;
};

struct FennecAnimData {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ FennecEntry** entries;
};
"""

def indent(text, n=4):
    pad = " " * n
    return "\n".join(pad + line if line.strip() else "" for line in text.split("\n"))

def qualified_sig(f):
    sig = f["sig"]
    if f["sym"] == "ctor":
        return "Fennec::Fennec()"
    parts = sig.split("(", 1)
    ret_name = parts[0]
    params = "(" + parts[1]
    ret, name = ret_name.rsplit(" ", 1)
    return f"{ret} Fennec::{name}{params}"

def emit(placement, ordinary_order, inclass_order):
    """
    placement: dict {n64: "ordinary"|"inclass"|"inline"}
    ordinary_order: list of n64 ints for out-of-line defs (source order)
    inclass_order:  list of n64 ints for in-class defs (declaration order)
    """
    out = [PREAMBLE]
    out.append("struct Fennec : Dingo {")
    out.append("    /* 0x34 */ FennecEntry* unk34;")
    out.append("    /* 0x38 */ FennecAnimData* unk38;")
    out.append("    /* 0x3C */ Vec3f unk3C;")
    out.append("    /* 0x48 */ f32 unk48;")
    out.append("    /* 0x4C */ f32 unk4C;")
    out.append("    /* 0x50 */ f32 unk50;")
    out.append("    /* 0x54 */ u16 unk54;")
    out.append("    /* 0x56 */ u16 unk56;")
    out.append("")

    # Virtual override declarations (not in-class)
    for n in sorted(FUNCS.keys()):
        f = FUNCS[n]
        if f["virtual"] and not f["new_virtual"] and placement.get(n) != "inclass":
            out.append(f'    {f["sig"]} CXX_OVERRIDE;')

    # New virtual declarations — vfunc20 before vfunc21 (vtable order)
    new_virt_order = [8, 6]  # vfunc20, vfunc21
    for n in new_virt_order:
        f = FUNCS[n]
        if placement.get(n) != "inclass":
            out.append(f'    virtual {f["sig"]};')

    # Non-virtual declarations (not in-class)
    for n in sorted(FUNCS.keys()):
        f = FUNCS[n]
        if not f["virtual"] and placement.get(n) != "inclass":
            if f["sym"] == "ctor":
                out.append("    Fennec();")
            else:
                out.append(f'    {f["sig"]};')

    # New virtuals (always declared, never in-class for simplicity)
    # Already handled above for virtuals

    # In-class definitions
    if inclass_order:
        out.append("")
    for n in inclass_order:
        f = FUNCS[n]
        if f["virtual"] and f["new_virtual"]:
            out.append(f'    virtual {f["sig"]} {{')
        elif f["virtual"]:
            out.append(f'    {f["sig"]} CXX_OVERRIDE {{')
        elif f["sym"] == "ctor":
            out.append("    Fennec() {")
        else:
            out.append(f'    {f["sig"]} {{')
        out.append(indent(f["body"], 8))
        out.append("    }")

    out.append("};")
    out.append("")

    # Out-of-line definitions
    for n in ordinary_order:
        f = FUNCS[n]
        p = placement[n]
        prefix = "inline " if p == "inline" else ""
        out.append(f"{prefix}{qualified_sig(f)} {{")
        out.append(indent(f["body"]))
        out.append("}")
        out.append("")

    return "\n".join(out) + "\n"

# ---------- presets ----------

PRESETS = {}

def preset(name, doc=""):
    def decorator(fn):
        PRESETS[name] = (fn, doc)
        return fn
    return decorator

@preset("all_ordinary", "All ordinary out-of-line, N64 order")
def p_all_ordinary():
    placement = {n: "ordinary" for n in FUNCS}
    return emit(placement, sorted(FUNCS.keys()), [])

@preset("ordinary_4_inline_rest", "First 4 ordinary (no floats), rest inline in reverse N64 order")
def p_ordinary_4():
    """
    Ordinary: vfunc9, vfunc8, func_801251EC, func_80125224 (no float constants)
    Inline: everything else (reverse N64 order so deferred emission = N64 order)
    """
    ordinary_ids = {1, 2, 3, 4}
    placement = {}
    ordinary = []
    inline = []
    for n in sorted(FUNCS.keys()):
        if n in ordinary_ids:
            placement[n] = "ordinary"
            ordinary.append(n)
        else:
            placement[n] = "inline"
            inline.append(n)
    inline.reverse()  # reverse so deferred emission gives N64 order
    return emit(placement, ordinary + inline, [])

@preset("ordinary_4_inclass_rest", "First 4 ordinary, rest in-class (reverse N64 order)")
def p_ordinary_4_inclass():
    ordinary_ids = {1, 2, 3, 4}
    placement = {}
    ordinary = []
    inclass = []
    for n in sorted(FUNCS.keys()):
        if n in ordinary_ids:
            placement[n] = "ordinary"
            ordinary.append(n)
        else:
            placement[n] = "inclass"
            inclass.append(n)
    inclass.reverse()
    return emit(placement, ordinary, inclass)

@preset("ordinary_4_mixed", "First 4 ordinary, heavy bodies inline, trivial in-class")
def p_ordinary_4_mixed():
    """
    Ordinary: vfunc9, vfunc8, func_801251EC, func_80125224
    In-class: trivial accessors, ctor
    Inline: heavy functions (vfunc6, vfunc21, func_801253CC, vfunc20, vfunc3)
    """
    ordinary_ids = {1, 2, 3, 4}
    inclass_ids = {9, 10, 11, 12, 13, 14, 16, 17, 18, 19, 20, 21, 22, 23, 24}
    placement = {}
    ordinary = []
    inline = []
    inclass = []
    for n in sorted(FUNCS.keys()):
        if n in ordinary_ids:
            placement[n] = "ordinary"
            ordinary.append(n)
        elif n in inclass_ids:
            placement[n] = "inclass"
            inclass.append(n)
        else:
            placement[n] = "inline"
            inline.append(n)
    inline.reverse()
    inclass.reverse()
    return emit(placement, ordinary + inline, inclass)

# ---------- main ----------

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--preset", default="all_ordinary")
    ap.add_argument("--list-presets", action="store_true")
    ap.add_argument("--output", default="src/fennec.cpp")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    if args.list_presets:
        for name, (fn, doc) in PRESETS.items():
            print(f"  {name:30s} {doc}")
        return

    if args.preset not in PRESETS:
        print(f"Unknown preset: {args.preset}. Use --list-presets.", file=sys.stderr)
        sys.exit(1)

    code = PRESETS[args.preset][0]()
    if args.dry_run:
        print(code)
    else:
        with open(args.output, "w") as f:
            f.write(code)
        print(f"Wrote {args.output} [{args.preset}]")

if __name__ == "__main__":
    main()

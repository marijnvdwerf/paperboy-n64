#!/usr/bin/env python3
"""
Generate padmelon.cpp with different function placement/ordering strategies.

Usage:
    python3 tools/gen_padmelon.py [--preset NAME]
    python3 tools/gen_padmelon.py --list-presets
"""
import argparse, sys, re
from collections import OrderedDict

FUNCS = OrderedDict()

def F(n64, sym, sig, body, *, virtual=False):
    """Register a function. sig/body are always member-style (no self param)."""
    FUNCS[n64] = dict(n64=n64, sym=sym, sig=sig, body=body, virtual=virtual)

# ---- virtuals ----

F(1, "vfunc16", "void vfunc16(Vec3f* dir, Vec3f* up)", virtual=True, body="""\
Vec3f forward;
Vec3f cross;
Vec3f side;
Vec3f proj;

vec3_normalize(dir, &forward);
Vec3f* fwd = &forward;

f32 dot = up->x * fwd->x + up->y * fwd->y + up->z * fwd->z;
proj.x = fwd->x * dot;
proj.y = fwd->y * dot;
proj.z = fwd->z * dot;
side.x = up->x - proj.x;
side.y = up->y - proj.y;
side.z = up->z - proj.z;
vec3_normalize(&side, &side);

cross.x = side.y * fwd->z - side.z * fwd->y;
cross.y = side.z * fwd->x - side.x * fwd->z;
cross.z = side.x * fwd->y - side.y * fwd->x;

rotation.m[0][0] = forward.x;
rotation.m[0][1] = forward.y;
rotation.m[0][2] = forward.z;
rotation.m[1][0] = cross.x;
rotation.m[1][1] = cross.y;
rotation.m[1][2] = cross.z;
rotation.m[2][0] = side.x;
rotation.m[2][1] = side.y;
rotation.m[2][2] = side.z;""")

F(4, "vfunc13", "void vfunc13(Vec3f* in, Vec3f* out)", virtual=True, body="""\
out->x = in->x * rotation.m[0][0];
out->y = in->x * rotation.m[1][0];
out->z = in->x * rotation.m[2][0];
out->x += in->y * rotation.m[0][1];
out->y += in->y * rotation.m[1][1];
out->z += in->y * rotation.m[2][1];
out->x += in->z * rotation.m[0][2];
out->y += in->z * rotation.m[1][2];
out->z += in->z * rotation.m[2][2];
out->x -= rotation.m[0][0] * translation.x + rotation.m[0][1] * translation.y + rotation.m[0][2] * translation.z;
out->y -= rotation.m[1][0] * translation.x + rotation.m[1][1] * translation.y + rotation.m[1][2] * translation.z;
out->z -= rotation.m[2][0] * translation.x + rotation.m[2][1] * translation.y + rotation.m[2][2] * translation.z;""")

F(10, "vfunc3", "void vfunc3(Vec3f* v)", virtual=True, body="""\
translation.x = v->x;
translation.y = v->y;
translation.z = v->z;
halfExtent = -1.0f;""")

F(11, "vfunc2", "void vfunc2(Vec3f* out)", virtual=True, body="""\
out->x = translation.x;
out->y = translation.y;
out->z = translation.z;""")

F(12, "vfunc12", "void vfunc12(Vec3f* in, Vec3f* out)", virtual=True, body="""\
out->x = in->x * rotation.m[0][0];
out->y = in->x * rotation.m[0][1];
out->z = in->x * rotation.m[0][2];
out->x += in->y * rotation.m[1][0];
out->y += in->y * rotation.m[1][1];
out->z += in->y * rotation.m[1][2];
out->x += in->z * rotation.m[2][0];
out->y += in->z * rotation.m[2][1];
out->z += in->z * rotation.m[2][2];
out->x += translation.x;
out->y += translation.y;
out->z += translation.z;""")

F(13, "vfunc15", "void vfunc15(Vec3f* in, Vec3f* out)", virtual=True, body="""\
out->x = in->x * rotation.m[0][0];
out->y = in->x * rotation.m[1][0];
out->z = in->x * rotation.m[2][0];
out->x += in->y * rotation.m[0][1];
out->y += in->y * rotation.m[1][1];
out->z += in->y * rotation.m[2][1];
out->x += in->z * rotation.m[0][2];
out->y += in->z * rotation.m[1][2];
out->z += in->z * rotation.m[2][2];""")

F(14, "vfunc14", "void vfunc14(Vec3f* in, Vec3f* out)", virtual=True, body="""\
out->x = in->x * rotation.m[0][0];
out->y = in->x * rotation.m[0][1];
out->z = in->x * rotation.m[0][2];
out->x += in->y * rotation.m[1][0];
out->y += in->y * rotation.m[1][1];
out->z += in->y * rotation.m[1][2];
out->x += in->z * rotation.m[2][0];
out->y += in->z * rotation.m[2][1];
out->z += in->z * rotation.m[2][2];""")

F(15, "vfunc19", "void vfunc19(Mat3f* mat)", virtual=True, body="""\
rotation.m[0][0] = mat->m[0][0];
rotation.m[0][1] = mat->m[0][1];
rotation.m[0][2] = mat->m[0][2];
rotation.m[1][0] = mat->m[1][0];
rotation.m[1][1] = mat->m[1][1];
rotation.m[1][2] = mat->m[1][2];
rotation.m[2][0] = mat->m[2][0];
rotation.m[2][1] = mat->m[2][1];
rotation.m[2][2] = mat->m[2][2];""")

F(16, "vfunc18", "void vfunc18(Mat3f* out)", virtual=True, body="""\
out->m[0][0] = rotation.m[0][0];
out->m[0][1] = rotation.m[0][1];
out->m[0][2] = rotation.m[0][2];
out->m[1][0] = rotation.m[1][0];
out->m[1][1] = rotation.m[1][1];
out->m[1][2] = rotation.m[1][2];
out->m[2][0] = rotation.m[2][0];
out->m[2][1] = rotation.m[2][1];
out->m[2][2] = rotation.m[2][2];""")

F(21, "vfunc17", "void vfunc17(Vec3f* a, Vec3f* b)", virtual=True, body="""\
a->x = rotation.m[0][0];
a->y = rotation.m[0][1];
a->z = rotation.m[0][2];
b->x = rotation.m[2][0];
b->y = rotation.m[2][1];
b->z = rotation.m[2][2];""")

# ---- non-virtual members ----

F(2, "func_80021A74", "void func_80021A74(Vec3f* dir, Vec3f* up)", body="""\
Vec3f forward;
Vec3f side;
Vec3f cross;
Vec3f proj;

vec3_normalize(dir, &forward);
Vec3f* fwd = &forward;

f32 dot = up->x * fwd->x + up->y * fwd->y + up->z * fwd->z;
proj.x = fwd->x * dot;
proj.y = fwd->y * dot;
proj.z = fwd->z * dot;
side.x = up->x - proj.x;
side.y = up->y - proj.y;
side.z = up->z - proj.z;
vec3_normalize(&side, &side);

cross.x = fwd->y * side.z - fwd->z * side.y;
cross.y = fwd->z * side.x - fwd->x * side.z;
cross.z = fwd->x * side.y - fwd->y * side.x;

rotation.m[0][0] = forward.x;
rotation.m[0][1] = forward.y;
rotation.m[0][2] = forward.z;
rotation.m[1][0] = side.x;
rotation.m[1][1] = side.y;
rotation.m[1][2] = side.z;
rotation.m[2][0] = cross.x;
rotation.m[2][1] = cross.y;
rotation.m[2][2] = cross.z;""")

F(3, "func_80021BC8", "void func_80021BC8(Vec3f* dir, Vec3f* up)", body="""\
Vec3f forward;
Vec3f cross;
Vec3f upNorm;
Vec3f proj;
Vec3f* upPtr = up;

vec3_normalize(dir, &upNorm);

f32 dot = upNorm.x * upPtr->x + upNorm.y * upPtr->y + upNorm.z * upPtr->z;
proj.x = upNorm.x * dot;
proj.y = upNorm.y * dot;
proj.z = upNorm.z * dot;
forward.x = upPtr->x - proj.x;
Vec3f* fwd = &forward;
fwd->y = upPtr->y - proj.y;
fwd->z = upPtr->z - proj.z;
vec3_normalize(fwd, fwd);

cross.x = upNorm.y * fwd->z - upNorm.z * fwd->y;
cross.y = upNorm.z * forward.x - upNorm.x * fwd->z;
cross.z = upNorm.x * fwd->y - upNorm.y * forward.x;

rotation.m[0][0] = forward.x;
rotation.m[0][1] = forward.y;
rotation.m[0][2] = forward.z;
rotation.m[1][0] = cross.x;
rotation.m[1][1] = cross.y;
rotation.m[1][2] = cross.z;
rotation.m[2][0] = upNorm.x;
rotation.m[2][1] = upNorm.y;
rotation.m[2][2] = upNorm.z;""")

F(5, "func_80021E88", "void func_80021E88(Mat3f* arg1, f32 (*out)[3])", body="""\
Mat3f tmp;

tmp.m[0][0] = rotation.m[0][0] * arg1->m[0][0] + rotation.m[1][0] * arg1->m[1][0] + rotation.m[2][0] * arg1->m[2][0];
tmp.m[0][1] = rotation.m[0][0] * arg1->m[0][1] + rotation.m[1][0] * arg1->m[1][1] + rotation.m[2][0] * arg1->m[2][1];
tmp.m[0][2] = rotation.m[0][0] * arg1->m[0][2] + rotation.m[1][0] * arg1->m[1][2] + rotation.m[2][0] * arg1->m[2][2];
tmp.m[1][0] = rotation.m[0][1] * arg1->m[0][0] + rotation.m[1][1] * arg1->m[1][0] + rotation.m[2][1] * arg1->m[2][0];
tmp.m[1][1] = rotation.m[0][1] * arg1->m[0][1] + rotation.m[1][1] * arg1->m[1][1] + rotation.m[2][1] * arg1->m[2][1];
tmp.m[1][2] = rotation.m[0][1] * arg1->m[0][2] + rotation.m[1][1] * arg1->m[1][2] + rotation.m[2][1] * arg1->m[2][2];
tmp.m[2][0] = rotation.m[0][2] * arg1->m[0][0] + rotation.m[1][2] * arg1->m[1][0] + rotation.m[2][2] * arg1->m[2][0];
tmp.m[2][1] = rotation.m[0][2] * arg1->m[0][1] + rotation.m[1][2] * arg1->m[1][1] + rotation.m[2][2] * arg1->m[2][1];
tmp.m[2][2] = rotation.m[0][2] * arg1->m[0][2] + rotation.m[1][2] * arg1->m[1][2] + rotation.m[2][2] * arg1->m[2][2];
mat3_mul(tmp.m, rotation.m, out);""")

F(6, "func_8002205C", "void func_8002205C()", body="""\
Vec3f row0;
Vec3f row2;

row0.x = rotation.m[0][0];
row0.y = -rotation.m[0][1];
row0.z = rotation.m[0][2];
row2.x = rotation.m[2][0];
row2.y = -rotation.m[2][1];
row2.z = rotation.m[2][2];
translation.y = -translation.y;

vfunc16(&row0, &row2);""")

F(7, "func_800220D0", "void func_800220D0(Mat4f* out, f32 scale)", body="""\
out->m[0][0] = rotation.m[0][0] * scale;
out->m[0][1] = rotation.m[0][1] * scale;
out->m[0][2] = rotation.m[0][2] * scale;
out->m[1][0] = rotation.m[1][0] * scale;
out->m[1][1] = rotation.m[1][1] * scale;
out->m[1][2] = rotation.m[1][2] * scale;
out->m[2][0] = rotation.m[2][0] * scale;
out->m[2][1] = rotation.m[2][1] * scale;
out->m[2][2] = rotation.m[2][2] * scale;""")

F(8, "func_80022144", "void func_80022144(f32 halfExt)", body="""\
halfExtent = halfExt;
minBound = pos.x - halfExt;
maxBound = pos.x + halfExt;""")

F(9, "func_80022168", "void func_80022168(Vec3f* v)", body="""\
pos = *v;
minBound = pos.x - halfExtent;
maxBound = pos.x + halfExtent;""")

F(17, "func_800224FC", "void func_800224FC(Mat4f* out)", body="""\
out->m[0][0] = rotation.m[0][0];
out->m[0][1] = rotation.m[0][1];
out->m[0][2] = rotation.m[0][2];
out->m[1][0] = rotation.m[1][0];
out->m[1][1] = rotation.m[1][1];
out->m[1][2] = rotation.m[1][2];
out->m[2][0] = rotation.m[2][0];
out->m[2][1] = rotation.m[2][1];
out->m[2][2] = rotation.m[2][2];""")

F(18, "func_80022548", "void func_80022548()", body="""\
vel.x = 0;
vel.y = 0;
vel.z = 0;
rotation.m[0][1] = 0;
rotation.m[0][2] = 0;
rotation.m[1][0] = 0;
rotation.m[1][2] = 0;
rotation.m[2][0] = 0;
rotation.m[2][1] = 0;
translation.x = 0;
translation.y = 0;
translation.z = 0;
rotation.m[0][0] = 1.0f;
rotation.m[1][1] = 1.0f;
rotation.m[2][2] = 1.0f;""")

F(19, "ctor", "Pademelon()", body="func_80022548();")

F(20, "func_800225CC", "void func_800225CC(Vec3f* row0, Vec3f* row1, Vec3f* row2)", body="""\
row0->x = rotation.m[0][0];
row0->y = rotation.m[0][1];
row0->z = rotation.m[0][2];
row1->x = rotation.m[1][0];
row1->y = rotation.m[1][1];
row1->z = rotation.m[1][2];
row2->x = rotation.m[2][0];
row2->y = rotation.m[2][1];
row2->z = rotation.m[2][2];""")

F(22, "func_8002264C", "void func_8002264C(f32* quat)", body="""\
quat_to_mat3(quat, (f32*)&rotation);""")

F(23, "func_80022670", "void func_80022670(f32* out)", body="""\
out[0] = rotation.m[0][0];
out[3] = rotation.m[1][0];
out[6] = rotation.m[2][0];
out[1] = rotation.m[0][1];
out[4] = rotation.m[1][1];
out[7] = rotation.m[2][1];
out[2] = rotation.m[0][2];
out[5] = rotation.m[1][2];
out[8] = rotation.m[2][2];
out[9] = translation.x;
out[10] = translation.y;
out[11] = translation.z;""")

F(24, "func_800226D4", "void func_800226D4(f32* quat)", body="""\
mat3_to_quat(rotation.m, quat);""")

F(25, "func_800226F0", "void func_800226F0(Vec3f* out)", body="""\
out->x = rotation.m[2][0];
out->y = rotation.m[2][1];
out->z = rotation.m[2][2];""")

F(26, "func_8002270C", "void func_8002270C(Vec3f* out)", body="""\
out->x = rotation.m[1][0];
out->y = rotation.m[1][1];
out->z = rotation.m[1][2];""")

F(27, "func_80022728", "void func_80022728(Vec3f* out)", body="""\
out->x = rotation.m[0][0];
out->y = rotation.m[0][1];
out->z = rotation.m[0][2];""")

F(28, "func_80022744", "void func_80022744(Mat4f* out)", body="""\
out->m[3][0] = translation.x;
out->m[3][1] = translation.y;
out->m[3][2] = translation.z;""")

F(29, "func_80022760", "void func_80022760(Pademelon* other)", body="""\
rotation.m[0][0] = other->rotation.m[0][0];
rotation.m[0][1] = other->rotation.m[0][1];
rotation.m[0][2] = other->rotation.m[0][2];
rotation.m[1][0] = other->rotation.m[1][0];
rotation.m[1][1] = other->rotation.m[1][1];
rotation.m[1][2] = other->rotation.m[1][2];
rotation.m[2][0] = other->rotation.m[2][0];
rotation.m[2][1] = other->rotation.m[2][1];
rotation.m[2][2] = other->rotation.m[2][2];
translation = other->translation;
halfExtent = -1.0f;""")

F(30, "func_800227D0", "void func_800227D0(Pademelon* other)", body="""\
rotation.m[0][0] = other->rotation.m[0][0];
rotation.m[0][1] = other->rotation.m[0][1];
rotation.m[0][2] = other->rotation.m[0][2];
rotation.m[1][0] = other->rotation.m[1][0];
rotation.m[1][1] = other->rotation.m[1][1];
rotation.m[1][2] = other->rotation.m[1][2];
rotation.m[2][0] = other->rotation.m[2][0];
rotation.m[2][1] = other->rotation.m[2][1];
rotation.m[2][2] = other->rotation.m[2][2];""")

F(31, "func_8002281C", "void func_8002281C(Pademelon* other)", body="""\
translation = other->translation;
halfExtent = -1.0f;""")

# ---------- code generation ----------

def indent(text, n=4):
    pad = " " * n
    return "\n".join(pad + line if line.strip() else "" for line in text.split("\n"))

def qualified_sig(f):
    """Pademelon::funcname(...) form."""
    sig = f["sig"]
    if f["sym"] == "ctor":
        return "Pademelon::Pademelon()"
    parts = sig.split("(", 1)
    ret_name = parts[0]  # e.g. "void vfunc16"
    params = "(" + parts[1]
    ret, name = ret_name.rsplit(" ", 1)
    return f"{ret} Pademelon::{name}{params}"

def emit(placement, ordinary_order, inclass_order):
    """
    placement: dict {n64: "ordinary"|"inclass"|"inline"}
    ordinary_order: list of n64 ints for out-of-line defs (source order)
    inclass_order:  list of n64 ints for in-class defs (declaration order)
    """
    out = []
    out.append('#include "common.h"')
    out.append('#include "dingo.h"')
    out.append('#include "vector.h"')
    out.append("")
    out.append("struct Pademelon : public Dingo {")
    out.append("    /* 0x34 */ Mat3f rotation;")
    out.append("    /* 0x58 */ Vec3f translation;")
    out.append("")

    # Declarations for virtuals NOT defined in-class
    for n in sorted(FUNCS.keys()):
        f = FUNCS[n]
        if f["virtual"] and placement.get(n) != "inclass":
            out.append(f'    {f["sig"]} CXX_OVERRIDE;')

    # Declarations for non-virtuals NOT defined in-class
    for n in sorted(FUNCS.keys()):
        f = FUNCS[n]
        if not f["virtual"] and placement.get(n) != "inclass":
            if f["sym"] == "ctor":
                out.append(f'    Pademelon();')
            else:
                out.append(f'    {f["sig"]};')

    # In-class definitions
    if inclass_order:
        out.append("")
    for n in inclass_order:
        f = FUNCS[n]
        if f["virtual"]:
            out.append(f'    {f["sig"]} CXX_OVERRIDE {{')
        elif f["sym"] == "ctor":
            out.append(f'    Pademelon() {{')
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

@preset("inclass_18plus", "18+ in-class (reverse N64 order), rest ordinary")
def p_inclass_18plus():
    placement = {}
    ordinary = []
    inclass = []
    for n in sorted(FUNCS.keys()):
        if n >= 18:
            placement[n] = "inclass"
            inclass.append(n)
        else:
            placement[n] = "ordinary"
            ordinary.append(n)
    inclass.reverse()  # reverse so deferred emission is forward N64 order
    return emit(placement, ordinary, inclass)

@preset("inclass_10plus", "10+ in-class (reverse N64 order), rest ordinary — puts all float funcs after vtable")
def p_inclass_10plus():
    placement = {}
    ordinary = []
    inclass = []
    for n in sorted(FUNCS.keys()):
        if n >= 10:
            placement[n] = "inclass"
            inclass.append(n)
        else:
            placement[n] = "ordinary"
            ordinary.append(n)
    inclass.reverse()
    return emit(placement, ordinary, inclass)

@preset("win_split", "Win order source, ordinary/inline split matching N64 emission")
def p_win_split():
    ordinary_ids = {1, 2, 3, 4, 5, 6}   # N64#1-6
    # In-class: ctor, reset, and all no-Win helpers (20-31)
    inclass_ids = {18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31}
    # Everything else is inline

    placement = {}
    for n in FUNCS:
        if n in ordinary_ids:
            placement[n] = "ordinary"
        elif n in inclass_ids:
            placement[n] = "inclass"
        else:
            placement[n] = "inline"

    # Windows source order for ordinary+inline (Win#3 onwards)
    # Win#:  3=N1, 4=N2, 5=N3, 6=N17, 7=N16, 8=N15, 9=N14, 10=N13, 11=N12,
    #       12=N4, 13=N11, 14=N10, 15=N9, 16=N8, 17=N5, 18=N7, 19=N6
    win_source = [1, 2, 3, 17, 16, 15, 14, 13, 12, 4, 11, 10, 9, 8, 5, 7, 6]
    # Then append the no-Win functions (20-31) at the end
    # No-Win functions go BEFORE Win-ordered inlines so they emit AFTER them (reversed)
    # Their internal order: reverse of N64 order so they emit in N64 order
    no_win = []  # all no-Win functions are in-class now
    win_source = no_win + win_source

    # Reverse N64 order so deferred emission gives N64 order
    inclass_order = [31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18]

    return emit(placement, win_source, inclass_order)

@preset("win_order", "All ordinary, Windows source order")
def p_win_order():
    win = [19,18,1,2,3,17,16,15,14,13,12,4,11,10,9,8,5,7,6,21,20,22,23,24,25,26,27,28,29,30,31]
    placement = {n: "ordinary" for n in FUNCS}
    return emit(placement, win, [])

# ---------- main ----------

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--preset", default="all_ordinary")
    ap.add_argument("--list-presets", action="store_true")
    ap.add_argument("--output", default="src/padmelon.cpp")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    if args.list_presets:
        for name, (fn, doc) in PRESETS.items():
            print(f"  {name:25s} {doc}")
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

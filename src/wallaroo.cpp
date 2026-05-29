#include "common.h"
#include "dingo.h"
#include "vector.h"

struct Pademelon : public Dingo {
    /* 0x34 */ Mat3f rotation;
    /* 0x58 */ Vec3f translation;

    Pademelon();
    void func_80022548();
    void func_800220D0(Mat4f* out, f32 scale);
    void func_80022144(f32 halfExt);
    void func_80022168(Vec3f* v);
};

struct AnimEntry {
    /* 0x00 */ char pad0[4];
    /* 0x04 */ s32 flags;
};

struct AnimData {
    /* 0x00 */ char pad0[4];
    /* 0x04 */ s32 count;
    /* 0x08 */ AnimEntry** entries;
    /* 0x0C */ char pad1[0x18];
    /* 0x24 */ Vec3f offset;
    /* 0x30 */ f32 radius;
    /* 0x34 */ char pad2[0xC];
    /* 0x40 */ f32 extent;
    /* 0x44 */ char pad3[4];
    /* 0x48 */ // vtable
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9();
    virtual void vfunc10();
    virtual void vfunc11();
    virtual void vfunc12();
    virtual void vfunc13();
    virtual void vfunc14();
    virtual void vfunc15();
    virtual void vfunc16(void* arg); // vtable+0x80: update with arg
    virtual void vfunc17(); // vtable+0x88: update
};

extern "C" s32 func_80029480(Vec3f* a, Vec3f* b, f32 radius);

struct Wallaroo;

struct SceneRenderer {
    /* 0x000 */ char pad[0x12C];
    // vtable at 0x12C — 43 virtual functions
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9();
    virtual void vfunc10();
    virtual void vfunc11();
    virtual void vfunc12();
    virtual void vfunc13();
    virtual void vfunc14();
    virtual void vfunc15();
    virtual void vfunc16();
    virtual void vfunc17();
    virtual void vfunc18();
    virtual void vfunc19();
    virtual void vfunc20();
    virtual void vfunc21();
    virtual void vfunc22();
    virtual void vfunc23();
    virtual void vfunc24();
    virtual void vfunc25();
    virtual void vfunc26();
    virtual void vfunc27();
    virtual void vfunc28();
    virtual void vfunc29();
    virtual void vfunc30();
    virtual void vfunc31();
    virtual void vfunc32();
    virtual void vfunc33();
    virtual void vfunc34();
    virtual void vfunc35(Wallaroo* w); // vtable+0x118: default render
    virtual void vfunc36();
    virtual void vfunc37();
    virtual void vfunc38();
    virtual void vfunc39();
    virtual void vfunc40();
    virtual void vfunc41(Wallaroo* w); // vtable+0x148: rotated render
    virtual void vfunc42();
    virtual void vfunc43(Wallaroo* w, f32 yaw, f32 pitch); // vtable+0x158: yaw/pitch render
};

#define N_ANIM_SLOTS 1

#pragma interface

struct Wallaroo : public Pademelon {
    /* 0x64 */ f32 scale;
    /* 0x68 */ s32 flags;
    /* 0x6C */ u16 yawRate;
    /* 0x6E */ u16 pitchRate;
    /* 0x70 */ s32 yawVel;
    /* 0x74 */ s32 pitchVel;
    /* 0x78 */ AnimData* overrideAnim[N_ANIM_SLOTS];
    /* 0x7C */ AnimData* anim[N_ANIM_SLOTS];
    /* 0x80 */ f32 animRadius[N_ANIM_SLOTS];

    Wallaroo();

    // Overrides (vtable entries replaced via INCLUDE_RODATA):
    void vfunc1() CXX_OVERRIDE;
    void vfunc4(s32 dt) CXX_OVERRIDE;
    void vfunc6(Vec3f* point, DingoFrustumResult* result) CXX_OVERRIDE;
    void vfunc8(SceneRenderer* scene) CXX_OVERRIDE;
    s32 vfunc9() CXX_OVERRIDE;
    void vfunc10(void* arg) CXX_OVERRIDE;
    void vfunc11() CXX_OVERRIDE;

    // New virtual functions (vtable entries 20-24)
    virtual void vfunc20(s32 index); // func_800242D4
    virtual void vfunc21(AnimData* animData, f32 radius); // func_800246C8
    virtual void vfunc22(); // func_80024664
    virtual s32 vfunc23(); // func_80024410
    virtual void vfunc24(); // func_80024408

    // Non-virtual members
    void func_80023EA8(f32 val);
    void func_80023ED4(f32 val);
    f32 func_80023F00();
    f32 func_80023F1C();
    void func_80023F38(f32 val);
    void func_80023F64(f32 val);
    f32 func_80023F90();
    f32 func_80023FB0();
    void func_8002414C(s32 index, Vec3f* outPos, f32* outRadius);
    void func_800243D0(Mat4f* out, s32 index);
    void func_8002453C(Vec3f* point, DingoFrustumResult* result);
    void func_80024648(AnimData* animData, f32 radius);
    void func_800247DC(f32* out);
    void func_800248BC();
    s32 func_800248CC();
    void func_800248D8();
    void func_800248EC();
    s32 func_800248FC();
    void func_80024908();
    void func_8002491C();
    s32 func_8002492C();
    s32 func_80024938();
    s32 func_8002495C();
    s32 func_80024980(f32 dist);
    void func_800249B4(f32 s);
    f32 func_800249C8(s32 index);
    f32 func_800249E8();
    void func_800249F4(s32 index, f32 radius);
    f32 func_80024A04(s32 index);
    void* func_80024A18(s32 index);
    void func_80024A2C(AnimData* animData, s32 index);
    void* func_80024A3C(s32 index);
    void* func_80024A50(s32 index);
    s32 func_80024A78();
};

#pragma implementation

#ifdef NON_MATCHING
// a0/a1 register swap — AnimData* ends up in a1 instead of a0
s32 Wallaroo::vfunc9() {
    AnimData* a = overrideAnim[0];
    if (a == NULL) {
        a = anim[0];
        if (a == NULL)
            return 0;
    }
    u32 count = a->count;
    for (u32 i = 0; i < count; i++) {
        AnimEntry* entry = a->entries[i];
        if (entry == NULL)
            continue;
        if (entry->flags & 0x1100)
            return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", vfunc9__8Wallaroo);
#endif

INCLUDE_RODATA("asm/nonmatchings/wallaroo", _vt.8Wallaroo);

// setPitchVel
void Wallaroo::func_80023EA8(f32 val) {
    flags |= 8;
    pitchVel = (s32)(val * 65.536f);
}

// setYawVel
void Wallaroo::func_80023ED4(f32 val) {
    flags |= 8;
    yawVel = (s32)(val * 65.536f);
}

// getPitchVel
f32 Wallaroo::func_80023F00() {
    return pitchVel * 0.015258789f;
}

// getYawVel
f32 Wallaroo::func_80023F1C() {
    return yawVel * 0.015258789f;
}

// setPitchRate
void Wallaroo::func_80023F38(f32 val) {
    flags |= 8;
    pitchRate = (s32)(val * 65536.0f);
}

// setYawRate
void Wallaroo::func_80023F64(f32 val) {
    flags |= 8;
    yawRate = (s32)(val * 65536.0f);
}

// getPitchRate
f32 Wallaroo::func_80023F90() {
    return pitchRate * 0.000015258789f;
}

// getYawRate
f32 Wallaroo::func_80023FB0() {
    return yawRate * 0.000015258789f;
}

// vfunc11
void Wallaroo::vfunc11() {
    for (u32 i = 0; i < N_ANIM_SLOTS; i++) {
        if (anim[i] == NULL)
            return;
        anim[i]->vfunc17();
    }
}

// vfunc10
void Wallaroo::vfunc10(void* arg) {
    for (u32 i = 0; i < N_ANIM_SLOTS; i++) {
        if (anim[i] == NULL)
            return;
        anim[i]->vfunc16(arg);
    }
}

// vfunc8: render/draw
void Wallaroo::vfunc8(SceneRenderer* scene) {
    s32 f = flags;
    if (f & 0xC) {
        if (*(s32*)&yawRate != 0) {
            f32 yaw = func_80023FB0();
            f32 pitch = func_80023F90();
            scene->vfunc43(this, yaw, pitch);
            return;
        }
        if (f & 4) {
            scene->vfunc41(this);
            return;
        }
    }
    scene->vfunc35(this);
}

void Wallaroo::func_8002414C(s32 index, Vec3f* outPos, f32* outRadius) {
    if (index == 0) {
        func_80122414(outPos);
        *outRadius = func_801223AC();
        return;
    }
    vfunc20(index);
    outPos->x = pos.x;
    outPos->y = pos.y;
    outPos->z = pos.z;
    *outRadius = halfExtent;
}

#ifdef NON_MATCHING
// Stack/register allocation mismatch
void Wallaroo::vfunc4(s32 dt) {
    Vec3f pos;
    vfunc2(&pos);
    f32 fdt = (f32)dt;
    Vec3f delta;
    delta.x = vel.x * fdt;
    delta.y = vel.y * fdt;
    delta.z = vel.z * fdt;
    pos.x += delta.x;
    pos.y += delta.y;
    pos.z += delta.z;
    vfunc3(&pos);
    halfExtent = -1.0f;
    if (yawVel != 0 || pitchVel != 0) {
        yawRate = (u16)(yawRate + dt * yawVel);
        pitchRate = (u16)(pitchRate + dt * pitchVel);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", vfunc4__8Wallarool);
#endif

#ifdef NON_MATCHING
// fs0 vs ft1+stack-spill for radius (score 603). All diffs cascade from this
// one register choice — compiler uses callee-saved fs0 instead of spilling
// ft1 to the stack. No source-level lever found.
void Wallaroo::vfunc20(s32 index) {
    AnimData* a = anim[index];
    if (a == NULL) {
        func_80022144(0.0f);
        return;
    }
    Vec3f ofs = a->offset;
    f32 rad = a->radius;
    Vec3f worldOfs;
    Vec3f* p = &ofs;
    f32 s = scale;
    p->x *= s;
    p->y *= s;
    p->z *= s;
    vfunc12(p, &worldOfs);
    func_80022168(&worldOfs);
    func_80022144(rad * scale);
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", vfunc20__8Wallarool);
#endif

// vfunc1: calls vfunc20(0)
void Wallaroo::vfunc1() {
    vfunc20(0);
}

void Wallaroo::func_800243D0(Mat4f* out, s32 index) {
    f32 extent = anim[index]->extent;
    func_800220D0(out, scale * extent);
}

void Wallaroo::vfunc24() {
}

s32 Wallaroo::vfunc23() {
    return 0;
}

void Wallaroo::vfunc6(Vec3f* point, DingoFrustumResult* result) {
    s32 i = 0;
    if (animRadius[0] != 1e37f) {
        Vec3f pos;
        vfunc2(&pos);
        Vec3f* p = &pos;
        f32 dx = point->x - p->x;
        f32 dy = point->y - p->y;
        f32 dz = point->z - p->z;
        f32 distSq = dx * dx + dy * dy + dz * dz;
        f32 r = animRadius[0];
        if (r < distSq) {
            do {
                i++;
                if (i != 0) {
                    result->result = 0;
                    return;
                }
            } while (r < distSq);
        }
    }
    result->pad = i;
    if (anim[i] == NULL) {
        result->result = 0;
        return;
    }
    Vec3f dpos;
    func_80122414(&dpos);
    result->result = func_80029480(point, &dpos, func_801223AC());
}

#ifdef NON_MATCHING
// Float register allocation mismatch
void Wallaroo::func_8002453C(Vec3f* point, DingoFrustumResult* result) {
    result->pad = 0;
    s32 i = 0;
    if (animRadius[0] != 1e37f) {
        Vec3f pos;
        vfunc2(&pos);
        f32 dx = point->x - pos.x;
        f32 dy = point->y - pos.y;
        f32 dz = point->z - pos.z;
        f32 distSq = dx * dx + dy * dy + dz * dz;
        if (animRadius[0] < distSq) {
            do {
                i++;
                if (i != 0) {
                    result->pad = 1;
                    goto done;
                }
            } while (animRadius[0] < distSq);
        }
    }
    result->pad = i;
    {
        if (anim[i] == NULL) {
            goto done;
        }
    }
    return;
done:
    result->result = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", func_8002453C__8WallarooP5Vec3fP18DingoFrustumResult);
#endif

#ifdef NON_MATCHING
// Target keeps index=0 as a variable (mtc1+sll+addu+swc1 pattern),
// but the compiler constant-folds it to direct stores (sw a1,0x7c(a0); sw a2,0x80(a0)).
// Windows version is a sorted-insert do-while, but with N_ANIM_SLOTS=1 the do-while
// body gets inlined with branch checks that don't appear in the target.
void Wallaroo::func_80024648(AnimData* animData, f32 radius) {
    u32 i = 0;
    do {
        if (anim[i] == NULL)
            break;
        if (radius < animRadius[i]) {
            for (u32 j = N_ANIM_SLOTS - 1; j > i; j--) {
                anim[j] = anim[j - 1];
                animRadius[j] = animRadius[j - 1];
            }
            break;
        }
        i++;
    } while (i < N_ANIM_SLOTS - 1);
    anim[i] = animData;
    animRadius[i] = radius;
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", func_80024648__8WallarooP8AnimDataf);
#endif

// vfunc22: reset
void Wallaroo::vfunc22() {
    flags = 0;
    yawRate = 0;
    pitchRate = 0;
    yawVel = 0;
    pitchVel = 0;
    scale = 1.0f;
    for (u32 i = 0; i < N_ANIM_SLOTS; i++) {
        anim[i] = NULL;
        animRadius[i] = -1.0f;
        overrideAnim[i] = NULL;
    }
    func_80022548();
}

// vfunc21: setAnim
void Wallaroo::vfunc21(AnimData* animData, f32 radius) {
    if (flags & 1) {
        vfunc22();
    }
    anim[0] = animData;
    animRadius[0] = radius;
    yawRate = 0;
    pitchRate = 0;
    yawVel = 0;
    pitchVel = 0;
    scale = 1.0f;
    halfExtent = -1.0f;
    flags |= 1;
}

Wallaroo::Wallaroo() {
    flags = 0;
    yawRate = 0;
    pitchRate = 0;
    yawVel = 0;
    pitchVel = 0;
    scale = 1.0f;
    halfExtent = -1.0f;
    for (u32 i = 0; i < N_ANIM_SLOTS; i++) {
        anim[i] = NULL;
        animRadius[i] = -1.0f;
        overrideAnim[i] = NULL;
    }
}

#ifdef NON_MATCHING
// Instruction scheduling mismatch
void Wallaroo::func_800247DC(f32* out) {
    f32 s = scale * anim[0]->extent;
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
    out[11] = translation.z;
    out[0] *= s;
    out[3] *= s;
    out[6] *= s;
    out[11] = translation.z;
    out[1] *= s;
    out[4] *= s;
    out[7] *= s;
    out[2] *= s;
    out[5] *= s;
    out[8] *= s;
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", func_800247DC__8WallarooPf);
#endif

void Wallaroo::func_800248BC() {
    flags |= 0x10;
}

s32 Wallaroo::func_800248CC() {
    return flags & 0x10;
}

void Wallaroo::func_800248D8() {
    flags &= ~4;
}

void Wallaroo::func_800248EC() {
    flags |= 4;
}

s32 Wallaroo::func_800248FC() {
    return flags & 4;
}

void Wallaroo::func_80024908() {
    flags &= ~2;
}

void Wallaroo::func_8002491C() {
    flags |= 2;
}

s32 Wallaroo::func_8002492C() {
    return flags & 2;
}

s32 Wallaroo::func_80024938() {
    s32 result = 0;
    if (yawVel != 0 || pitchVel != 0) {
        result = 1;
    }
    return result;
}

s32 Wallaroo::func_8002495C() {
    s32 result = 0;
    if (yawVel != 0 || pitchVel != 0) {
        result = 1;
    }
    return result;
}

s32 Wallaroo::func_80024980(f32 dist) {
    for (u32 i = 0; i < N_ANIM_SLOTS; i++) {
        if (dist <= animRadius[i]) {
            return i;
        }
    }
    return 1;
}

void Wallaroo::func_800249B4(f32 s) {
    scale = s;
    halfExtent = -1.0f;
}

f32 Wallaroo::func_800249C8(s32 index) {
    f32 extent = anim[index]->extent;
    return scale * extent;
}

f32 Wallaroo::func_800249E8() {
    return scale;
}

void Wallaroo::func_800249F4(s32 index, f32 radius) {
    animRadius[index] = radius;
}

f32 Wallaroo::func_80024A04(s32 index) {
    return animRadius[index];
}

void* Wallaroo::func_80024A18(s32 index) {
    return anim[index];
}

void Wallaroo::func_80024A2C(AnimData* animData, s32 index) {
    if (index == 0) {
        overrideAnim[0] = animData;
    }
}

void* Wallaroo::func_80024A3C(s32 index) {
    return overrideAnim[index];
}

void* Wallaroo::func_80024A50(s32 index) {
    AnimData* result = overrideAnim[index];
    if (result != NULL) {
        return result;
    }
    return anim[index];
}

s32 Wallaroo::func_80024A78() {
    return flags & 1;
}

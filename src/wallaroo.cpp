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
    virtual void vfunc16(void* arg);  // vtable+0x80: update with arg
    virtual void vfunc17();           // vtable+0x88: update
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
    virtual void vfunc35(Wallaroo* w);               // vtable+0x118: default render
    virtual void vfunc36();
    virtual void vfunc37();
    virtual void vfunc38();
    virtual void vfunc39();
    virtual void vfunc40();
    virtual void vfunc41(Wallaroo* w);               // vtable+0x148: rotated render
    virtual void vfunc42();
    virtual void vfunc43(Wallaroo* w, f32 yaw, f32 pitch); // vtable+0x158: yaw/pitch render
};

#define N_ANIM_SLOTS 1

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
    // vfunc1  -> func_800243A4
    // vfunc4  -> func_800241D4
    // vfunc6  -> func_80024418
    // vfunc8  -> func_80024090
    // vfunc9  -> func_80023E30
    // vfunc10 -> func_80024028
    // vfunc11 -> func_80023FD0

    // New virtual functions (vtable entries 20-24)
    virtual void vfunc20(s32 index);            // func_800242D4
    virtual void vfunc21(void* animData, f32 radius); // func_800246C8
    virtual void vfunc22();                     // func_80024664
    virtual s32 vfunc23();                      // func_80024410
    virtual void vfunc24();                     // func_80024408
};

#ifdef NON_MATCHING
// a0/a1 register swap — AnimData* ends up in a1 instead of a0
extern "C" s32 func_80023E30(Wallaroo* self) {
    AnimData* a = self->overrideAnim[0];
    if (a == NULL) {
        a = self->anim[0];
        if (a == NULL) return 0;
    }
    u32 count = a->count;
    for (u32 i = 0; i < count; i++) {
        AnimEntry* entry = a->entries[i];
        if (entry == NULL) continue;
        if (entry->flags & 0x1100) return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", func_80023E30);
#endif

INCLUDE_RODATA("asm/nonmatchings/wallaroo", _vt.8Wallaroo);

// setPitchVel
extern "C" void func_80023EA8(Wallaroo* self, f32 val) {
    self->flags |= 8;
    self->pitchVel = (s32)(val * 65.536f);
}

// setYawVel
extern "C" void func_80023ED4(Wallaroo* self, f32 val) {
    self->flags |= 8;
    self->yawVel = (s32)(val * 65.536f);
}

// getPitchVel
extern "C" f32 func_80023F00(Wallaroo* self) {
    return self->pitchVel * 0.015258789f;
}

// getYawVel
extern "C" f32 func_80023F1C(Wallaroo* self) {
    return self->yawVel * 0.015258789f;
}

// setPitchRate
extern "C" void func_80023F38(Wallaroo* self, f32 val) {
    self->flags |= 8;
    self->pitchRate = (s32)(val * 65536.0f);
}

// setYawRate
extern "C" void func_80023F64(Wallaroo* self, f32 val) {
    self->flags |= 8;
    self->yawRate = (s32)(val * 65536.0f);
}

// getPitchRate
extern "C" f32 func_80023F90(Wallaroo* self) {
    return self->pitchRate * 0.000015258789f;
}

// getYawRate
extern "C" f32 func_80023FB0(Wallaroo* self) {
    return self->yawRate * 0.000015258789f;
}

// vfunc11
extern "C" void func_80023FD0(Wallaroo* self) {
    for (u32 i = 0; i < N_ANIM_SLOTS; i++) {
        if (self->anim[i] == NULL) return;
        self->anim[i]->vfunc17();
    }
}

// vfunc10
extern "C" void func_80024028(Wallaroo* self, void* arg) {
    for (u32 i = 0; i < N_ANIM_SLOTS; i++) {
        if (self->anim[i] == NULL) return;
        self->anim[i]->vfunc16(arg);
    }
}

// vfunc8: render/draw
extern "C" void func_80024090(Wallaroo* self, SceneRenderer* scene) {
    s32 f = self->flags;
    if (f & 0xC) {
        if (*(s32*)&self->yawRate != 0) {
            f32 yaw = func_80023FB0(self);
            f32 pitch = func_80023F90(self);
            scene->vfunc43(self, yaw, pitch);
            return;
        }
        if (f & 4) {
            scene->vfunc41(self);
            return;
        }
    }
    scene->vfunc35(self);
}

extern "C" void func_8002414C(Wallaroo* self, s32 index, Vec3f* outPos, f32* outRadius) {
    if (index == 0) {
        self->func_80122414(outPos);
        *outRadius = self->func_801223AC();
        return;
    }
    self->vfunc20(index);
    outPos->x = self->pos.x;
    outPos->y = self->pos.y;
    outPos->z = self->pos.z;
    *outRadius = self->halfExtent;
}

#ifdef NON_MATCHING
// Stack/register allocation mismatch
extern "C" void func_800241D4(Wallaroo* self, s32 dt) {
    Vec3f pos;
    self->vfunc2(&pos);
    f32 fdt = (f32)dt;
    Vec3f delta;
    delta.x = self->vel.x * fdt;
    delta.y = self->vel.y * fdt;
    delta.z = self->vel.z * fdt;
    pos.x += delta.x;
    pos.y += delta.y;
    pos.z += delta.z;
    self->vfunc3(&pos);
    self->halfExtent = -1.0f;
    if (self->yawVel != 0 || self->pitchVel != 0) {
        self->yawRate = (u16)(self->yawRate + dt * self->yawVel);
        self->pitchRate = (u16)(self->pitchRate + dt * self->pitchVel);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", func_800241D4);
#endif

#ifdef NON_MATCHING
// fs0 vs ft1+stack-spill for radius (score 603). All diffs cascade from this
// one register choice — compiler uses callee-saved fs0 instead of spilling
// ft1 to the stack. No source-level lever found.
extern "C" void func_800242D4(Wallaroo* self, s32 index) {
    AnimData* a = self->anim[index];
    if (a == NULL) {
        self->func_80022144(0.0f);
        return;
    }
    Vec3f ofs = a->offset;
    f32 rad = a->radius;
    Vec3f worldOfs;
    Vec3f* p = &ofs;
    f32 s = self->scale;
    p->x *= s;
    p->y *= s;
    p->z *= s;
    self->vfunc12(p, &worldOfs);
    self->func_80022168(&worldOfs);
    self->func_80022144(rad * self->scale);
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", func_800242D4);
#endif

// vfunc1: calls vfunc20(0)
extern "C" void func_800243A4(Wallaroo* self) {
    self->vfunc20(0);
}

extern "C" void func_800243D0(Wallaroo* self, Mat4f* out, s32 index) {
    f32 extent = self->anim[index]->extent;
    self->func_800220D0(out, self->scale * extent);
}

extern "C" void func_80024408(void) {
}

extern "C" s32 func_80024410(Wallaroo* self) {
    return 0;
}

extern "C" void func_80024418(Wallaroo* self, Vec3f* point, DingoFrustumResult* result) {
    s32 i = 0;
    if (self->animRadius[0] != 1e37f) {
        Vec3f pos;
        self->vfunc2(&pos);
        Vec3f* p = &pos;
        f32 dx = point->x - p->x;
        f32 dy = point->y - p->y;
        f32 dz = point->z - p->z;
        f32 distSq = dx * dx + dy * dy + dz * dz;
        f32 r = self->animRadius[0];
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
    if (self->anim[i] == NULL) {
        result->result = 0;
        return;
    }
    Vec3f dpos;
    self->func_80122414(&dpos);
    result->result = func_80029480(point, &dpos, self->func_801223AC());
}

#ifdef NON_MATCHING
// Float register allocation mismatch
extern "C" void func_8002453C(Wallaroo* self, Vec3f* point, DingoFrustumResult* result) {
    result->pad = 0;
    s32 i = 0;
    if (self->animRadius[0] != 1e37f) {
        Vec3f pos;
        self->vfunc2(&pos);
        f32 dx = point->x - pos.x;
        f32 dy = point->y - pos.y;
        f32 dz = point->z - pos.z;
        f32 distSq = dx * dx + dy * dy + dz * dz;
        if (self->animRadius[0] < distSq) {
            do {
                i++;
                if (i != 0) {
                    result->pad = 1;
                    goto done;
                }
            } while (self->animRadius[0] < distSq);
        }
    }
    result->pad = i;
    {
        if (self->anim[i] == NULL) {
            goto done;
        }
    }
    return;
done:
    result->result = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", func_8002453C);
#endif

#ifdef NON_MATCHING
// Target keeps index=0 as a variable (mtc1+sll+addu+swc1 pattern),
// but the compiler constant-folds it to direct stores (sw a1,0x7c(a0); sw a2,0x80(a0)).
// Windows version is a sorted-insert do-while, but with N_ANIM_SLOTS=1 the do-while
// body gets inlined with branch checks that don't appear in the target.
extern "C" void func_80024648(Wallaroo* self, AnimData* animData, f32 radius) {
    u32 i = 0;
    do {
        if (self->anim[i] == NULL) break;
        if (radius < self->animRadius[i]) {
            for (u32 j = N_ANIM_SLOTS - 1; j > i; j--) {
                self->anim[j] = self->anim[j - 1];
                self->animRadius[j] = self->animRadius[j - 1];
            }
            break;
        }
        i++;
    } while (i < N_ANIM_SLOTS - 1);
    self->anim[i] = animData;
    self->animRadius[i] = radius;
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", func_80024648);
#endif

// vfunc22: reset
extern "C" void func_80024664(Wallaroo* self) {
    self->flags = 0;
    self->yawRate = 0;
    self->pitchRate = 0;
    self->yawVel = 0;
    self->pitchVel = 0;
    self->scale = 1.0f;
    for (u32 i = 0; i < N_ANIM_SLOTS; i++) {
        self->anim[i] = NULL;
        self->animRadius[i] = -1.0f;
        self->overrideAnim[i] = NULL;
    }
    self->func_80022548();
}

// vfunc21: setAnim
extern "C" void func_800246C8(Wallaroo* self, AnimData* animData, f32 radius) {
    if (self->flags & 1) {
        self->vfunc22();
    }
    self->anim[0] = animData;
    self->animRadius[0] = radius;
    self->yawRate = 0;
    self->pitchRate = 0;
    self->yawVel = 0;
    self->pitchVel = 0;
    self->scale = 1.0f;
    self->halfExtent = -1.0f;
    self->flags |= 1;
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
extern "C" void func_800247DC(Wallaroo* self, f32* out) {
    f32 s = self->scale * self->anim[0]->extent;
    out[0] = self->rotation.m[0][0];
    out[3] = self->rotation.m[1][0];
    out[6] = self->rotation.m[2][0];
    out[1] = self->rotation.m[0][1];
    out[4] = self->rotation.m[1][1];
    out[7] = self->rotation.m[2][1];
    out[2] = self->rotation.m[0][2];
    out[5] = self->rotation.m[1][2];
    out[8] = self->rotation.m[2][2];
    out[9] = self->translation.x;
    out[10] = self->translation.y;
    out[11] = self->translation.z;
    out[0] *= s;
    out[3] *= s;
    out[6] *= s;
    out[11] = self->translation.z;
    out[1] *= s;
    out[4] *= s;
    out[7] *= s;
    out[2] *= s;
    out[5] *= s;
    out[8] *= s;
}
#else
INCLUDE_ASM("asm/nonmatchings/wallaroo", func_800247DC);
#endif

extern "C" void func_800248BC(Wallaroo* self) {
    self->flags |= 0x10;
}

extern "C" s32 func_800248CC(Wallaroo* self) {
    return self->flags & 0x10;
}

extern "C" void func_800248D8(Wallaroo* self) {
    self->flags &= ~4;
}

extern "C" void func_800248EC(Wallaroo* self) {
    self->flags |= 4;
}

extern "C" s32 func_800248FC(Wallaroo* self) {
    return self->flags & 4;
}

extern "C" void func_80024908(Wallaroo* self) {
    self->flags &= ~2;
}

extern "C" void func_8002491C(Wallaroo* self) {
    self->flags |= 2;
}

extern "C" s32 func_8002492C(Wallaroo* self) {
    return self->flags & 2;
}

extern "C" s32 func_80024938(Wallaroo* self) {
    s32 result = 0;
    if (self->yawVel != 0 || self->pitchVel != 0) {
        result = 1;
    }
    return result;
}

extern "C" s32 func_8002495C(Wallaroo* self) {
    s32 result = 0;
    if (self->yawVel != 0 || self->pitchVel != 0) {
        result = 1;
    }
    return result;
}

extern "C" s32 func_80024980(Wallaroo* self, f32 dist) {
    for (u32 i = 0; i < N_ANIM_SLOTS; i++) {
        if (dist <= self->animRadius[i]) {
            return i;
        }
    }
    return 1;
}

extern "C" void func_800249B4(Wallaroo* self, f32 s) {
    self->scale = s;
    self->halfExtent = -1.0f;
}

extern "C" f32 func_800249C8(Wallaroo* self, s32 index) {
    f32 extent = self->anim[index]->extent;
    return self->scale * extent;
}

extern "C" f32 func_800249E8(Wallaroo* self) {
    return self->scale;
}

extern "C" void func_800249F4(Wallaroo* self, s32 index, f32 radius) {
    self->animRadius[index] = radius;
}

extern "C" f32 func_80024A04(Wallaroo* self, s32 index) {
    return self->animRadius[index];
}

extern "C" void* func_80024A18(Wallaroo* self, s32 index) {
    return self->anim[index];
}

extern "C" void func_80024A2C(Wallaroo* self, AnimData* animData, s32 index) {
    if (index == 0) {
        self->overrideAnim[0] = animData;
    }
}

extern "C" void* func_80024A3C(Wallaroo* self, s32 index) {
    return self->overrideAnim[index];
}

extern "C" void* func_80024A50(Wallaroo* self, s32 index) {
    AnimData* result = self->overrideAnim[index];
    if (result != NULL) {
        return result;
    }
    return self->anim[index];
}

extern "C" s32 func_80024A78(Wallaroo* self) {
    return self->flags & 1;
}

#ifndef FENNEC_H
#define FENNEC_H

#include "dingo.h"

struct FennecEntry {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 flags;
};

struct FennecAnimData {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ FennecEntry** entries;
};

struct Fennec : Dingo {
    /* 0x34 */ FennecEntry* unk34;
    /* 0x38 */ FennecAnimData* unk38;
    /* 0x3C */ Vec3f unk3C;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ u16 unk54;
    /* 0x56 */ u16 unk56;

    Fennec();

    s32 vfunc9() CXX_OVERRIDE;
    void vfunc8(SceneRenderer*) CXX_OVERRIDE;
    void vfunc6(Vec3f* frustum, DingoFrustumResult* out) CXX_OVERRIDE;
    void vfunc3(Vec3f* v) CXX_OVERRIDE;
    virtual void vfunc20(void* obj, f32 width, f32 height, f32 radius);
    virtual void vfunc21();

    FennecEntry* func_801251EC();
    void func_80125224(Vec3f* point, DingoFrustumResult* out);
    void func_801253CC(FennecAnimData* data, s32 index, f32 width, f32 height, f32 radius);
    void* func_80125524();
    void func_80125530(f32 val);
    f32 func_80125538();
    void func_80125544();
    void func_80125554(Vec3f* v);
    void func_8012559C(f32 val);
    void func_801255A4(f32 val);
    void func_801255AC(Vec3f* out);
    f32 func_801255C8();
    f32 func_801255D4();
    void func_801255E0(FennecEntry* val);
    s32 func_801255E8();
    s32 func_801255F4();
    u16 func_80125600();
};

#endif

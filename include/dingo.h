#ifndef DINGO_H
#define DINGO_H

#include "common.h"

struct DingoVec3f {
    f32 x, y, z;
};

struct DingoMat3f {
    f32 m[3][3];
};

struct DingoFrustumResult {
    s32 result;
    s32 pad;
};

struct Dingo {
    /* 0x00 */ DingoVec3f pos;
    /* 0x0C */ f32 halfExtent;
    /* 0x10 */ f32 minBound;
    /* 0x14 */ f32 maxBound;
    /* 0x18 */ DingoVec3f vel;
    /* 0x24 */ DingoVec3f target;
    /* 0x30 */ // vtable

    Dingo();
    virtual void vfunc1();
    virtual void vfunc2(DingoVec3f* out);
    virtual void vfunc3(DingoVec3f* v);
    virtual void vfunc4(s32 steps);
    virtual void vfunc5(f32 dt);
    virtual void vfunc6(void* frustum, DingoFrustumResult* out);
    virtual s32 vfunc7(Dingo* other);
    virtual void vfunc8();
    virtual s32 vfunc9();
    virtual void vfunc10();
    virtual void vfunc11();
    virtual void vfunc12(DingoVec3f* a, DingoVec3f* out);
    virtual void vfunc13(DingoVec3f* a, DingoVec3f* out);
    virtual void vfunc14(DingoVec3f* src, DingoVec3f* dst);
    virtual void vfunc15(DingoVec3f* src, DingoVec3f* dst);
    virtual void vfunc16();
    virtual void vfunc17(DingoVec3f* scale, DingoVec3f* translation);
    virtual void vfunc18(DingoMat3f* out);
    virtual void vfunc19();

    void func_80122314(DingoVec3f* v);
    void func_80122330(f32 halfExtent);
    void func_80122354(DingoVec3f* v);
    void func_80122390(DingoVec3f* out);
    f32 func_801223AC();
    void func_80122414(DingoVec3f* out);
    f32 func_801225B0(DingoVec3f* point);
    f32 func_8012288C();
    f32 func_801228DC();
    void func_8012292C(DingoVec3f* v);
    void func_80122948(DingoVec3f* out);
};

#endif

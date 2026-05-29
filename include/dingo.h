#ifndef DINGO_H
#define DINGO_H

#include "common.h"
#include "vector.h"

struct DingoFrustumResult {
    s32 result;
    s32 pad;
};

struct SceneRenderer;

struct Dingo {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 halfExtent;
    /* 0x10 */ f32 minBound;
    /* 0x14 */ f32 maxBound;
    /* 0x18 */ Vec3f vel;
    /* 0x24 */ Vec3f target;
    /* 0x30 */ // vtable

    Dingo();
    virtual void vfunc1();
    virtual void vfunc2(Vec3f* out);
    virtual void vfunc3(Vec3f* v);
    virtual void vfunc4(s32 steps);
    virtual void vfunc5(f32 dt);
    virtual void vfunc6(Vec3f* point, DingoFrustumResult* out);
    virtual s32 vfunc7(Dingo* other);
    virtual void vfunc8(SceneRenderer* scene);
    virtual s32 vfunc9();
    virtual void vfunc10(void* arg);
    virtual void vfunc11();
    virtual void vfunc12(Vec3f* a, Vec3f* out);
    virtual void vfunc13(Vec3f* a, Vec3f* out);
    virtual void vfunc14(Vec3f* src, Vec3f* dst);
    virtual void vfunc15(Vec3f* src, Vec3f* dst);
    virtual void vfunc16(Vec3f* dir, Vec3f* up);
    virtual void vfunc17(Vec3f* scale, Vec3f* translation);
    virtual void vfunc18(Mat3f* out);
    virtual void vfunc19(Mat3f* mat);

    void func_80122314(Vec3f* v);
    void func_80122330(f32 halfExtent);
    void func_80122354(Vec3f* v);
    void func_80122390(Vec3f* out);
    f32 func_801223AC();
    void func_80122414(Vec3f* out);
    f32 func_801225B0(Vec3f* point);
    f32 func_8012288C();
    f32 func_801228DC();
    void func_8012292C(Vec3f* v);
    void func_80122948(Vec3f* out);
};

#endif

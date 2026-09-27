#ifndef GOL_TRANSFORM_BASE_H
#define GOL_TRANSFORM_BASE_H

#include "common.h"

struct GolTransformBase {
    GolTransformBase* parent; // 0x0
    GolTransformBase* next; // 0x4
    GolTransformBase* child; // 0x8
    // vptr at 0xC
    GolTransformBase();
    void addChild(GolTransformBase* child);
    void removeChild(GolTransformBase* child);
    virtual void vfunc1() = 0;
    virtual void vfunc2(f32* src, f32* dst) = 0;
    virtual void vfunc3(f32* src, f32* dst) = 0;
    virtual void vfunc4(f32* src, f32* dst) = 0;
    virtual void vfunc5(f32* src, f32* dst) = 0;
    virtual void vfunc6(f32* quat) = 0;
    virtual void vfunc7(f32* a, f32* b, f32* c) = 0;
    virtual void vfunc8(f32* a, f32* b) = 0;
    virtual void vfunc9(f32* a, f32* b) = 0;
    virtual void vfunc10(f32* a, f32* b) = 0;
    virtual void vfunc11(f32* a, f32* b) = 0;
    virtual void vfunc12(f32* quat) = 0;
    virtual void vfunc13(f32* vec) = 0;
    virtual void vfunc14(f32* vec) = 0;
    virtual void vfunc15(f32* vec) = 0;
    virtual f32 vfunc16(f32* vec) = 0;
    virtual void vfunc17(f32* vec) = 0;
    virtual void vfunc18(f32* vec) = 0;
    virtual void vfunc19(GolTransformBase* src) = 0;
};

#endif

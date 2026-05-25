#ifndef POSSUM_H
#define POSSUM_H

#include "common.h"

struct PossumColor {
    u8 r, g, b, a;
};

struct PossumBase {
    virtual void vfunc1(PossumColor* dst, s32 start, u32 num);
    virtual void vfunc2(PossumColor* src, s32 start, u32 num);
    virtual void vfunc3(PossumColor* dst, s32 index);
    virtual void vfunc4(PossumBase* src);
    virtual s32 vfunc5(PossumColor* color);
    virtual s32 vfunc6();
    virtual s32 vfunc7();
    virtual s32 vfunc8();
};

struct PossumInfo {
    u8 pad[0x16];
    u16 bitDepth;
};

struct Possum : PossumBase {
    /* 0x04 */ PossumColor* data;
    /* 0x08 */ u32 count;

    Possum();
    virtual ~Possum();

    void vfunc1(PossumColor* dst, s32 start, u32 num) CXX_OVERRIDE;
    void vfunc2(PossumColor* src, s32 start, u32 num) CXX_OVERRIDE;
    void vfunc3(PossumColor* dst, s32 index) CXX_OVERRIDE;
    void vfunc4(PossumBase* src) CXX_OVERRIDE;
    s32 vfunc5(PossumColor* color) CXX_OVERRIDE;
    s32 vfunc6() CXX_OVERRIDE;
    s32 vfunc7() CXX_OVERRIDE;
    s32 vfunc8() CXX_OVERRIDE;

    void func_80026D68();
    void func_80026DA0(PossumInfo* info);
    PossumColor* func_80026EEC();
    s32 func_80026EF8();

    static void func_80026B50(s32 heap);
};

#endif

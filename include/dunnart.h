#ifndef DUNNART_H
#define DUNNART_H

#include "common.h"
#include "pixel_format.h"

// TODO: figure out real class
struct DunnartColor {
    u8 r, g, b, a;
};

// TODO: figure out real class
struct DunnartBase {
    virtual void vfunc1(u8* dst, s32 start, u32 num);
};

struct Dunnart : DunnartBase {
    /* 0x04 */ u16* data;
    /* 0x08 */ u32 count;

    // Virtual functions (vtable slot order):
    virtual void vfunc1(u8* dst, s32 start, u32 num) CXX_OVERRIDE;
    virtual void vfunc2(DunnartColor* colors, s32 start, u32 num);
    virtual void vfunc3(u8* dst, s32 index);
    virtual void vfunc4(Dunnart* src);
    virtual s32 vfunc5(u8* color);
    virtual s32 vfunc6();
    virtual u32 vfunc7();
    virtual u32 vfunc8();

    Dunnart();
    ~Dunnart();

    void func_80030B50();
    void func_80030B88(PixelFormat* pf);
    u16* func_80030CC0();
    s32 func_80030CCC();

    static void func_800308D8(s32 val);
    static s32 func_80030CB0();
};

#endif // DUNNART_H

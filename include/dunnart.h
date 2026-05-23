#ifndef DUNNART_H
#define DUNNART_H

#include "common.h"
#include "surfaces.h"

struct Dunnart : Surface16970Palette {
    /* 0x04 */ u16* data;
    /* 0x08 */ u32 count;

    virtual void vfunc1(u8* dst, s32 start, u32 num) CXX_OVERRIDE;
    virtual void vfunc2(DunnartColor* colors, s32 start, u32 num) CXX_OVERRIDE;
    virtual void vfunc3(u8* dst, s32 index) CXX_OVERRIDE;
    virtual void vfunc4(Surface16970Palette* src) CXX_OVERRIDE;
    virtual s32 vfunc5(u8* color);
    virtual s32 vfunc6();
    virtual u32 vfunc7();
    virtual u32 vfunc8();

    Dunnart();
    virtual ~Dunnart();

    void func_80030B50();
    void func_80030B88(PixelFormat* pf);
    u16* func_80030CC0();
    s32 func_80030CCC();

    static void func_800308D8(s32 val);
    static s32 func_80030CB0();
};

#endif // DUNNART_H

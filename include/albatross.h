#ifndef ALBATROSS_H
#define ALBATROSS_H

#include "dugong.h"

struct Surface16970;
class Surface16970Palette;
struct TiledSurface;

struct Albatross : public Dugong {
    /* 0x5B0 */ char pathBuf[0x40];
    /* 0x5F0 */ u8 unk5F0[0x300];
    /* 0x8F0 */ u8 unk8F0[0x5DC];
    /* 0xECC */ s32 unkECC;
    /* 0xED0 */ s32 unkED0;
    /* 0xED4 */ s32 unkED4;

    Albatross();
    void vfunc1() CXX_OVERRIDE;
    void vfunc3(const char* filename) CXX_OVERRIDE;
    const char* vfunc5() CXX_OVERRIDE;
    void vfunc6(Surface16970* dstSurface, s32 flipFlag, u8* transColor) CXX_OVERRIDE;
    void vfunc7(TiledSurface* dstSurface, s32 flipFlag, u8* transColor) CXX_OVERRIDE;
    void vfunc8(u8* dst) CXX_OVERRIDE;
    void vfunc9(u8* srcBuffer, Surface16970* dstSurface, s32 flipFlag, u8* transColor) CXX_OVERRIDE;
};

#endif

#ifndef CORMORANT_H
#define CORMORANT_H

#include "dugong.h"

struct Surface16970;
struct TiledSurface;

struct Cormorant : public Dugong {
    /* 0x5B0 */ s32 idLength;
    /* 0x5B4 */ s32 colorMapType;
    /* 0x5B8 */ s32 imageType;
    /* 0x5BC */ s32 colorMapStart;
    /* 0x5C0 */ s32 colorMapLength;
    /* 0x5C4 */ s32 colorMapBitDepth;
    /* 0x5C8 */ s32 xOrigin;
    /* 0x5CC */ s32 yOrigin;
    /* 0x5D0 */ s32 imageDescriptor;
    /* 0x5D4 */ s32 dataOffset;
    /* 0x5D8 */ s32 dataSize;

    Cormorant();
    void vfunc1() CXX_OVERRIDE;
    void vfunc3(const char* filename) CXX_OVERRIDE;
    const char* vfunc5() CXX_OVERRIDE;
    void vfunc6(Surface16970* dstSurface, s32 flipFlag, u8* transColor) CXX_OVERRIDE;
    void vfunc7(TiledSurface* dstSurface, s32 flipFlag, u8* transColor) CXX_OVERRIDE;
    void vfunc8(u8* dst) CXX_OVERRIDE;
    void vfunc9(u8* srcBuffer, Surface16970* dstSurface, s32 flipFlag, u8* transColor) CXX_OVERRIDE;

    void decompressTgaRle(u8* src, u8* dst);
};

#endif

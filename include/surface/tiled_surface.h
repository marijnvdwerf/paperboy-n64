#ifndef SURFACE_TILED_SURFACE_H
#define SURFACE_TILED_SURFACE_H

#include "common.h"

class GolSurface;

struct TiledSurface {
    /* 0x00 */ s32* tileWidths;
    /* 0x04 */ s32* tileHeights;
    /* 0x08 */ u8 unk8[0x20];
    /* 0x28 */ u32 rowCount;
    /* 0x2C */ u32 colCount;
    /* 0x30 */ s32 totalWidth;
    /* 0x34 */ s32 totalHeight;
    /* 0x38 */ s32 flags;
    /* 0x3C */ u8 unk3C[0x10];

    virtual ~TiledSurface() = 0;
    virtual void vfunc2() = 0;
    virtual void vfunc3() = 0;
    virtual void vfunc4() = 0;
    virtual void vfunc5() = 0;
    virtual void vfunc6() = 0;
    virtual void vfunc7() = 0;
    virtual GolSurface* vfunc8(s32 row, s32 col) = 0;
};

#endif

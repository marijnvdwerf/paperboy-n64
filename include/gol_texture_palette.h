#ifndef GOL_TEXTURE_PALETTE_H
#define GOL_TEXTURE_PALETTE_H

#include "common.h"
#include "gol_surface_format.h"
#include "gol_surface.h"

struct PossumInfo {
    u8 pad[0x16];
    u16 bitDepth;
};

struct GolTexturePalette : GolPaletteBase {
    /* 0x04 */ ColorRGBA* data;
    /* 0x08 */ u32 count;

    GolTexturePalette();
    virtual ~GolTexturePalette();

    void vfunc1(u8* dst, s32 start, u32 num) CXX_OVERRIDE;
    void vfunc2(ColorRGBA* src, s32 start, u32 num) CXX_OVERRIDE;
    void vfunc3(u8* dst, s32 index) CXX_OVERRIDE;
    void vfunc4(GolPaletteBase* src) CXX_OVERRIDE;
    s32 vfunc5(u8* color) CXX_OVERRIDE;
    s32 vfunc6() CXX_OVERRIDE;
    u32 vfunc7() CXX_OVERRIDE;
    u32 vfunc8() CXX_OVERRIDE;

    void func_80026D68();
    void func_80026DA0(GolSurfaceFormat* pf);
    ColorRGBA* func_80026EEC();
    s32 func_80026EF8();

    static void func_80026B50(s32 heap);
};

#endif

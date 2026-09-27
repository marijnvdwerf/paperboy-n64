#ifndef GOL_PALETTED_TEXTURE_H
#define GOL_PALETTED_TEXTURE_H

#include "common.h"
#include "gol_texture.h"
#include "gol_texture_palette.h"

class GolPalettedTexture : public GolTexture {
  public:
    /* 0x40 */ GolTexturePalette palette;

    virtual ~GolPalettedTexture();
    GolPaletteBase* vfunc8() CXX_OVERRIDE;
    void vfunc14(GolRenderDevice* ctx, GolSurfaceFormat* pf, s32 w, s32 h) CXX_OVERRIDE;
    void vfunc15() CXX_OVERRIDE;
    void vfunc16(GolRenderDevice* ctx, BunyipScene* scene) CXX_OVERRIDE;

    GolTexturePalette* func_800270E8();

    static void func_80026F10(s32 val);
    static s32 func_800270D8();
};

#endif // GOL_PALETTED_TEXTURE_H

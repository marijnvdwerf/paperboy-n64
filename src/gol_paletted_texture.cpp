#include "common.h"
#include "gol_paletted_texture.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);
extern s32 D_80070B50;
extern const char D_80002150[];

void GolPalettedTexture::func_80026F10(s32 val) {
    D_80070B50 = val;
}

GolPaletteBase* GolPalettedTexture::vfunc8() {
    return (GolPaletteBase*)&palette;
}

void GolPalettedTexture::vfunc15() {
    palette.func_80026D68();
    if (unk18 != NULL) {
        delete[] unk18;
        unk18 = NULL;
    }
    unk22 = 0;
}

void GolPalettedTexture::vfunc14(GolRenderDevice* ctx, GolSurfaceFormat* pf, s32 w, s32 h) {
    if (unk22 & 1) {
        vfunc15();
    }

    unk26 = w;
    unk28 = h;
    unk22 |= 1;

    hdr = *pf;

    if (pf->paletteMask != 0) {
        palette.func_80026DA0(pf);
    }

    unk20 = BITS_TO_BYTES(w * pf->bitDepth);

    func_8004B3BC(D_80070B50);
    unk18 = new u8[unk20 * h];
    func_8004B390();

    if (unk18 == NULL) {
        __assert(D_80002150, 0, 0, 0);
    }
}

GolPalettedTexture::~GolPalettedTexture() {
    vfunc15();
}

s32 GolPalettedTexture::func_800270D8() {
    return D_80070B50;
}

GolTexturePalette* GolPalettedTexture::func_800270E8() {
    return &palette;
}

void GolPalettedTexture::vfunc16(GolRenderDevice* ctx, BunyipScene* scene) {
    GolTexture::vfunc16(ctx, scene);
}

INCLUDE_RODATA("asm/nonmatchings/gol_paletted_texture", D_80002150);

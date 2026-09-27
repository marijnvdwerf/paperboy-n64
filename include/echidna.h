#ifndef ECHIDNA_H
#define ECHIDNA_H

#include "common.h"
#include "gol_texture.h"
#include "dunnart.h"

struct Echidna : public GolTexture {
    /* 0x40 */ Dunnart palette;

    ~Echidna();

    GolPaletteBase* vfunc8() CXX_OVERRIDE;
    void vfunc14(GolRenderDevice* ctx, GolSurfaceFormat* pf, s32 w, s32 h) CXX_OVERRIDE;
    void vfunc15() CXX_OVERRIDE;

    static void func_8003A340(s32 val);
    static s32 func_8003A508();
};

#endif // ECHIDNA_H

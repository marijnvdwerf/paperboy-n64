#ifndef ECHIDNA_H
#define ECHIDNA_H

#include "common.h"
#include "bunyip.h"
#include "dunnart.h"

struct Echidna : public Bunyip {
    /* 0x40 */ Dunnart palette;

    ~Echidna();

    Surface16970Palette* vfunc8() CXX_OVERRIDE;
    void vfunc14(BunyipRenderer* ctx, PixelFormat* pf, s32 w, s32 h) CXX_OVERRIDE;
    void vfunc15() CXX_OVERRIDE;

    static void func_8003A340(s32 val);
    static s32 func_8003A508();
};

#endif // ECHIDNA_H

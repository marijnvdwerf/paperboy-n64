#include "common.h"
#include "echidna.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390();

extern s32 D_800740A0;

void Echidna::func_8003A340(s32 val) {
    D_800740A0 = val;
}

Surface16970Palette* Echidna::vfunc8() {
    return &palette;
}

void Echidna::vfunc15() {
    palette.func_80030B50();
    if (unk18 != NULL) {
        delete[] unk18;
        unk18 = NULL;
    }
    unk22 = 0;
}

void Echidna::vfunc14(BunyipRenderer* ctx, PixelFormat* pf, s32 w, s32 h) {
    if (unk22 & 1) {
        vfunc15();
    }

    unk26 = w;
    unk28 = h;
    unk22 |= 1;

    hdr = *pf;

    if (pf->paletteMask != 0) {
        palette.func_80030B88(pf);
    }

    unk20 = BITS_TO_BYTES(w * pf->bitDepth);

    func_8004B3BC(D_800740A0);
    unk18 = new u8[unk20 * h];
    func_8004B390();

    if (unk18 == NULL) {
        __assert("", 0, 0, 0);
    }
}

Echidna::~Echidna() {
    vfunc15();
}

s32 Echidna::func_8003A508() {
    return D_800740A0;
}

#include "common.h"
#include "porcupine.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);
extern s32 D_80070B50;
extern const char D_80002150[];

void Porcupine::func_80026F10(s32 val) {
    D_80070B50 = val;
}

Surface16970Palette* Porcupine::vfunc8() {
    return (Surface16970Palette*)&palette;
}

void Porcupine::vfunc15() {
    palette.func_80026D68();
    if (unk18 != NULL) {
        delete[] unk18;
        unk18 = NULL;
    }
    unk22 = 0;
}

void Porcupine::vfunc14(BunyipRenderer* ctx, PixelFormat* pf, s32 w, s32 h) {
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

Porcupine::~Porcupine() {
    vfunc15();
}

s32 Porcupine::func_800270D8() {
    return D_80070B50;
}

Possum* Porcupine::func_800270E8() {
    return &palette;
}

void Porcupine::vfunc16(BunyipRenderer* ctx, BunyipScene* scene) {
    Bunyip::vfunc16(ctx, scene);
}

INCLUDE_RODATA("asm/nonmatchings/porcupine", D_80002150);

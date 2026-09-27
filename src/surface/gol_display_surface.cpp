#include "gol_surface.h"

void GolDisplaySurface::vfunc15() {
    this->unk30 = NULL;
    this->unk34 = 0;
}

void GolDisplaySurface::vfunc14(void* buf, u16 w, u16 h, u32 bpp) {
    this->unk30 = buf;
    this->unk26 = w;
    this->unk28 = h;
    this->hdr.bitDepth = bpp;
}

GolDisplaySurface::~GolDisplaySurface() {
    this->vfunc15();
}

GolDisplaySurface::GolDisplaySurface() {
    this->unk30 = NULL;
    this->unk34 = 0;
}

s32 GolDisplaySurface::func_80016C58() {
    return this->unk34 & 2;
}

s32 GolDisplaySurface::func_80016C64() {
    return this->unk34 & 1;
}

void* GolDisplaySurface::func_80016C70() {
    return this->unk30;
}

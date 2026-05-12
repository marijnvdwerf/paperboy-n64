#include "surfaces.h"

void Surface177B0::vfunc15() {
    this->unk30 = NULL;
    this->unk34 = 0;
}

void Surface177B0::vfunc14(void* buf, u16 w, u16 h, u32 bpp) {
    this->unk30 = buf;
    this->unk26 = w;
    this->unk28 = h;
    this->hdr.unk16 = bpp;
}

Surface177B0::~Surface177B0() {
    this->vfunc15();
}

Surface177B0::Surface177B0() {
    this->unk30 = NULL;
    this->unk34 = 0;
}

s32 Surface177B0::func_80016C58() {
    return this->unk34 & 2;
}

s32 Surface177B0::func_80016C64() {
    return this->unk34 & 1;
}

void* Surface177B0::func_80016C70() {
    return this->unk30;
}

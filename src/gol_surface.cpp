#include "gol_surface.h"

#ifdef NON_MATCHING
void GolSurface::vfunc9(u32 fill) {
    u8* addr;
    s32 pitch;
    s32 didLock;
    if (this->unk22 & 2) {
        addr = this->unk18;
        pitch = this->unk20;
        didLock = 0;
    } else {
        this->vfunc2(&addr, &pitch, 2);
        didLock = 1;
    }
    u32 width = this->unk26;
    u32 height = this->unk28;
    switch (this->hdr.bitDepth) {
        case 4:
            fill &= 0xF;
            width = this->unk26 / 2;
            fill = fill | fill << 4;
            // fallthrough
        case 8: {
            u32 y = 0;
            if (height != 0) {
                u32 v = fill;
                do {
                    u8* p = addr;
                    u8* end = p + width;
                    if (p < end) {
                        do {
                            *p = v;
                            p++;
                        } while (p < end);
                    }
                    addr += pitch;
                    y++;
                } while (y < height);
            }
            break;
        }
        case 16: {
            u32 y = 0;
            if (height != 0) {
                width *= 2;
                do {
                    u8* p = addr;
                    u8* end = p + width;
                    if (p < end) {
                        do {
                            *(u16*)p = fill;
                            p += 2;
                        } while (p < end);
                    }
                    addr += pitch;
                    y++;
                } while (y < height);
            }
            break;
        }
        case 24: {
            u32 hi = fill >> 16;
            u32 mid = fill >> 8;
            u32 y = 0;
            if (height != 0) {
                width = width * 2 + width;
                do {
                    u8* p = addr;
                    u8* end = p + width;
                    if (p < end) {
                        do {
                            *p++ = hi;
                            *p++ = mid;
                            *p++ = fill;
                        } while (p < end);
                    }
                    addr += pitch;
                    y++;
                } while (y < height);
            }
            break;
        }
        case 32: {
            u32 y = 0;
            if (height != 0) {
                width *= 4;
                do {
                    u8* p = addr;
                    u8* end = p + width;
                    if (p < end) {
                        do {
                            *(u32*)p = fill;
                            p += 4;
                        } while (p < end);
                    }
                    addr += pitch;
                    y++;
                } while (y < height);
            }
            break;
        }
    }
    if (didLock) {
        this->vfunc3();
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/gol_surface", vfunc9__10GolSurfaceUl);
#endif

#ifdef NON_MATCHING
void GolSurface::vfunc11(s32 dstX, s32 dstY, u8* srcAddr, s32 srcPitch, Rect16970* clip) {
    if (clip->unk0 >= clip->unk8) {
        return;
    }
    u32 srcW = clip->unk8 - clip->unk0;
    if (clip->unk4 >= clip->unkC) {
        return;
    }
    u32 srcH = clip->unkC - clip->unk4;
    if ((u32)dstX >= this->unk26) {
        return;
    }
    if ((u32)dstY >= this->unk28) {
        return;
    }
    if (dstX + srcW > this->unk26) {
        srcW = this->unk26 - dstX;
    }
    if (dstY + srcH > this->unk28) {
        srcH = this->unk28 - dstY;
    }
    u8* dstAddr;
    s32 dstPitch;
    s32 didLock;
    if (this->unk22 & 2) {
        dstAddr = this->unk18;
        dstPitch = this->unk20;
        didLock = 0;
    } else {
        this->vfunc2(&dstAddr, &dstPitch, 2);
        didLock = 1;
    }
    if (this->hdr.bitDepth == 4) {
        srcAddr += srcPitch * clip->unk4;
        dstAddr += dstPitch * dstY;
        u32 dstMaskInit;
        u32 dstShiftInit;
        if (dstX & 1) {
            dstMaskInit = 0xF0;
            dstShiftInit = 0;
        } else {
            dstMaskInit = 0x0F;
            dstShiftInit = 4;
        }
        u32 srcMaskInit;
        u32 srcShiftInit;
        if (clip->unk0 & 1) {
            srcMaskInit = 0x0F;
            srcShiftInit = 0;
        } else {
            srcMaskInit = 0xF0;
            srcShiftInit = 4;
        }
        u32 y = 0;
        if (srcH != 0) {
            do {
                u32 dstMask = dstMaskInit;
                u32 dstShift = dstShiftInit;
                u32 srcMask = srcMaskInit;
                u32 srcShift = srcShiftInit;
                u32 x = 0;
                if (srcW != 0) {
                    do {
                        u8* dByte = dstAddr + ((dstX + x) >> 1);
                        u8* sByte = srcAddr + ((clip->unk0 + x) >> 1);
                        x++;
                        u32 dVal = *dByte & dstMask;
                        dstMask = ~dstMask;
                        u32 sVal = *sByte & srcMask;
                        srcMask = ~srcMask;
                        u32 nibble = (sVal >> srcShift) << dstShift;
                        dstShift ^= 4;
                        srcShift ^= 4;
                        *dByte = dVal | nibble;
                    } while (x < srcW);
                }
                dstAddr += dstPitch;
                srcAddr += srcPitch;
                y++;
            } while (y < srcH);
        }
    } else {
        u32 bpp = this->hdr.bitDepth >> 3;
        srcAddr += bpp * clip->unk0;
        srcAddr += srcPitch * clip->unk4;
        dstAddr += bpp * dstX;
        u32 rowBytes = srcW * bpp;
        dstAddr += dstPitch * dstY;
        u32 y = 0;
        if (srcH != 0) {
            do {
                u8* d = dstAddr;
                u8* dEnd = d + rowBytes;
                if (d < dEnd) {
                    u8* s = srcAddr;
                    do {
                        *d++ = *s++;
                    } while (d < dEnd);
                }
                dstAddr += dstPitch;
                srcAddr += srcPitch;
                y++;
            } while (y < srcH);
        }
    }
    if (didLock) {
        this->vfunc3();
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/gol_surface", vfunc11__10GolSurfacellPUclP9Rect16970);
#endif

void GolSurface::vfunc10(s32 a1, s32 a2, GolSurface* src, Rect16970* clip) {
    if (this->hdr.bitDepth != src->hdr.bitDepth) {
        return;
    }
    if ((s32)src->unk26 < clip->unk8) {
        clip->unk8 = src->unk26;
    }
    if ((s32)src->unk28 < clip->unkC) {
        clip->unkC = src->unk28;
    }
    if (this->hdr.paletteMask) {
        this->vfunc8()->vfunc4(src->vfunc8());
    }
    u8* addr;
    s32 pitch;
    s32 didLock = 0;
    if (src->unk22 & 2) {
        addr = src->unk18;
        pitch = src->unk20;
    } else {
        src->vfunc2(&addr, &pitch, 1);
        didLock = 1;
    }
    this->vfunc11(a1, a2, addr, pitch, clip);
    if (didLock) {
        src->vfunc3();
    }
}

void GolSurface::vfunc13(void*, s16, s16, s32) {
}

void GolSurface::vfunc12() {
}

GolPaletteBase* GolSurface::vfunc8() {
    return NULL;
}

void GolSurface::vfunc7() {
}

void GolSurface::vfunc6() {
}

void GolSurface::vfunc5() {
    u16 v = this->unk24;
    if (v & 2) {
        this->unk24 = v & 0xFFE5;
    }
}

void GolSurface::vfunc4(u8** outAddr, s32* outPitch, s32 mode) {
    *outPitch = this->unk20;
    *outAddr = (u8*)this->unk1C;
    u16 v = this->unk24;
    this->unk24 = v | 2;
    if (mode & 1) {
        this->unk24 = v | 0xA;
    }
    if (mode & 2) {
        this->unk24 = this->unk24 | 0x10;
    }
}

void GolSurface::vfunc3() {
    u16 v = this->unk22;
    if (v & 2) {
        this->unk22 = v & 0xFFE5;
    }
}

void GolSurface::vfunc2(u8** outAddr, s32* outPitch, s32 mode) {
    *outPitch = this->unk20;
    *outAddr = this->unk18;
    u16 v = this->unk22;
    this->unk22 = v | 2;
    if (mode & 1) {
        this->unk22 = v | 0xA;
    }
    if (mode & 2) {
        this->unk22 = this->unk22 | 0x10;
    }
}

#if 0
GolSurface::~GolSurface() {
}
#else
INCLUDE_ASM("asm/nonmatchings/gol_surface", _._10GolSurface);
#endif

GolSurface::GolSurface() {
    this->unk18 = NULL;
    this->unk1C = 0;
    this->unk20 = 0;
    this->unk22 = 0;
    this->unk24 = 0;
    this->unk26 = 0;
    this->unk28 = 0;
}

extern "C" s32 func_800165C4(GolSurface* self, const u8* rgba) {
    return self->hdr.packColor(rgba);
}

extern "C" u16 func_800165E0(GolSurface* self) {
    return self->hdr.paletteMask;
}

extern "C" s32 func_800165EC(GolSurface* self) {
    return self->hdr.unk10;
}

extern "C" s32 func_800165F8(GolSurface* self) {
    return self->hdr.maskAlpha;
}

extern "C" s32 func_80016604(GolSurface* self) {
    return self->hdr.maskBlue;
}

extern "C" s32 func_80016610(GolSurface* self) {
    return self->hdr.maskGreen;
}

extern "C" s32 func_8001661C(GolSurface* self) {
    return self->hdr.maskRed;
}

extern "C" s32 func_80016628(GolSurface* self) {
    return self->hdr.ctzPaletteMask();
}

extern "C" s32 func_80016644(GolSurface* self) {
    return self->hdr.ctzUnk10();
}

extern "C" s32 func_80016660(GolSurface* self) {
    return self->hdr.ctzMaskAlpha();
}

extern "C" s32 func_8001667C(GolSurface* self) {
    return self->hdr.ctzMaskBlue();
}

extern "C" s32 func_80016698(GolSurface* self) {
    return self->hdr.ctzMaskGreen();
}

extern "C" s32 func_800166B4(GolSurface* self) {
    return self->hdr.ctzMaskRed();
}

extern "C" s32 func_800166D0(GolSurface* self) {
    return self->hdr.bitWidthPaletteMask();
}

extern "C" s32 func_800166EC(GolSurface* self) {
    return self->hdr.bitWidthUnk10();
}

extern "C" s32 func_80016708(GolSurface* self) {
    return self->hdr.bitWidthMaskAlpha();
}

extern "C" s32 func_80016724(GolSurface* self) {
    return self->hdr.bitWidthMaskBlue();
}

extern "C" s32 func_80016740(GolSurface* self) {
    return self->hdr.bitWidthMaskGreen();
}

extern "C" s32 func_8001675C(GolSurface* self) {
    return self->hdr.bitWidthMaskRed();
}

extern "C" s32 func_80016778(GolSurface* self) {
    return self->hdr.unk10 != 0;
}

extern "C" s32 func_80016784(GolSurface* self) {
    return self->hdr.maskAlpha != 0;
}

extern "C" s32 func_80016790(GolSurface* self) {
    return self->hdr.paletteMask != 0;
}

extern "C" s32 func_8001679C(GolSurface* self) {
    return self->unk22 & 4;
}

extern "C" s32 func_800167A8(GolSurface* self) {
    return self->unk24 & 2;
}

extern "C" s32 func_800167B4(GolSurface* self) {
    return self->unk22 & 2;
}

extern "C" s32 func_800167C0(GolSurface* self) {
    return self->unk22 & 1;
}

extern "C" void func_800167CC(GolSurface* self, GolSurface* dst) {
    dst->hdr = self->hdr;
}

extern "C" u16 func_80016800(GolSurface* self) {
    return self->hdr.bitDepth;
}

extern "C" u16 func_8001680C(GolSurface* self) {
    return self->unk28;
}

extern "C" u16 func_80016818(GolSurface* self) {
    return self->unk28;
}

extern "C" u16 func_80016824(GolSurface* self) {
    return self->unk26;
}

extern "C" u16 func_80016830(GolSurface* self) {
    return self->unk26;
}

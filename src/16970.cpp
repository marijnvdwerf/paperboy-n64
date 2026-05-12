#include "common.h"

// TODO: real names unknown. Looks like a Surface/Bitmap class with a virtual
// "lock/unlock" pair (slots 2/3), a palette accessor (slot 8) and a blit
// (slot 11). Local stubs while we work out the real hierarchy.
class Surface16970;

class Surface16970Palette {
  public:
    virtual void vfunc1() = 0;
    virtual void vfunc2() = 0;
    virtual void vfunc3() = 0;
    virtual void vfunc4(Surface16970Palette*) = 0;
};

struct Rect16970 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
};

struct Surface16970Header {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u16 unk16;
};

class Surface16970 {
  public:
    /* 0x00 */ Surface16970Header hdr;
    /* 0x18 */ u32 unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u16 unk22;
    /* 0x24 */ u16 unk24;
    /* 0x26 */ u16 unk26;
    /* 0x28 */ u16 unk28;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ // vptr (SN places it after data)

    Surface16970();

    // slot 1 = dtor (func_80016548)
    virtual ~Surface16970();
    // slot 2 — lock unk18 (func_800164FC)
    virtual void vfunc2(s32* outAddr, s32* outPitch, s32 mode);
    // slot 3 — unlock unk22 (func_800164E0)
    virtual void vfunc3();
    // slot 4 — lock unk1C (func_80016494)
    virtual void vfunc4(s32* outAddr, s32* outPitch, s32 mode);
    // slot 5 — unlock unk24 (func_80016478)
    virtual void vfunc5();
    // slot 6 — empty stub (func_80016470)
    virtual void vfunc6();
    // slot 7 — empty stub (func_80016468)
    virtual void vfunc7();
    // slot 8 — palette getter, base returns NULL (func_80016460)
    virtual Surface16970Palette* vfunc8();
    // slot 9 — fill (func_80015D70)
    virtual void vfunc9(u32 fillVal);
    // slot 10 — blit-with-rect (func_800162D4)
    virtual void vfunc10(s32 a, s32 b, Surface16970* src, Rect16970* clip);
    // slot 11 — raw blit with addr/pitch (func_80015FD4)
    virtual void vfunc11(s32 a, s32 b, s32 addr, s32 pitch, Rect16970* clip);
    // slot 12 — empty stub (func_80016458)
    virtual void vfunc12();
    // slot 13 — empty stub (func_80016450)
    virtual void vfunc13();
};

#ifdef NON_MATCHING
void Surface16970::vfunc9(u32 fill) {
    s32 addr;
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
    switch (this->hdr.unk16) {
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
                    u8* p = (u8*)addr;
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
                    u8* p = (u8*)addr;
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
                    u8* p = (u8*)addr;
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
                    u8* p = (u8*)addr;
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
INCLUDE_ASM("asm/nonmatchings/16970", vfunc9__12Surface16970Ul);
#endif

#ifdef NON_MATCHING
void Surface16970::vfunc11(s32 dstX, s32 dstY, s32 srcAddr, s32 srcPitch, Rect16970* clip) {
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
    s32 dstAddr;
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
    if (this->hdr.unk16 == 4) {
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
                        u8* dByte = (u8*)dstAddr + ((dstX + x) >> 1);
                        u8* sByte = (u8*)srcAddr + ((clip->unk0 + x) >> 1);
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
        u32 bpp = this->hdr.unk16 >> 3;
        srcAddr += bpp * clip->unk0;
        srcAddr += srcPitch * clip->unk4;
        dstAddr += bpp * dstX;
        u32 rowBytes = srcW * bpp;
        dstAddr += dstPitch * dstY;
        u32 y = 0;
        if (srcH != 0) {
            do {
                u8* d = (u8*)dstAddr;
                u8* dEnd = d + rowBytes;
                if (d < dEnd) {
                    u8* s = (u8*)srcAddr;
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
INCLUDE_ASM("asm/nonmatchings/16970", vfunc11__12Surface16970llllP9Rect16970);
#endif

void Surface16970::vfunc10(s32 a1, s32 a2, Surface16970* src, Rect16970* clip) {
    if (this->hdr.unk16 != src->hdr.unk16) {
        return;
    }
    if ((s32)src->unk26 < clip->unk8) {
        clip->unk8 = src->unk26;
    }
    if ((s32)src->unk28 < clip->unkC) {
        clip->unkC = src->unk28;
    }
    if (this->hdr.unk14) {
        this->vfunc8()->vfunc4(src->vfunc8());
    }
    s32 addr;
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

void Surface16970::vfunc13() {
}

void Surface16970::vfunc12() {
}

Surface16970Palette* Surface16970::vfunc8() {
    return NULL;
}

void Surface16970::vfunc7() {
}

void Surface16970::vfunc6() {
}

void Surface16970::vfunc5() {
    u16 v = this->unk24;
    if (v & 2) {
        this->unk24 = v & 0xFFE5;
    }
}

void Surface16970::vfunc4(s32* outAddr, s32* outPitch, s32 mode) {
    *outPitch = this->unk20;
    *outAddr = this->unk1C;
    u16 v = this->unk24;
    this->unk24 = v | 2;
    if (mode & 1) {
        this->unk24 = v | 0xA;
    }
    if (mode & 2) {
        this->unk24 = this->unk24 | 0x10;
    }
}

void Surface16970::vfunc3() {
    u16 v = this->unk22;
    if (v & 2) {
        this->unk22 = v & 0xFFE5;
    }
}

void Surface16970::vfunc2(s32* outAddr, s32* outPitch, s32 mode) {
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
Surface16970::~Surface16970() {
}
#else
INCLUDE_ASM("asm/nonmatchings/16970", _._12Surface16970);
#endif

Surface16970::Surface16970() {
    this->hdr.unk0 = 0;
    this->hdr.unk4 = 0;
    this->hdr.unk8 = 0;
    this->hdr.unkC = 0;
    this->hdr.unk10 = 0;
    this->hdr.unk14 = 0;
    this->hdr.unk16 = 0;
    this->unk18 = 0;
    this->unk1C = 0;
    this->unk20 = 0;
    this->unk22 = 0;
    this->unk24 = 0;
    this->unk26 = 0;
    this->unk28 = 0;
}

extern "C" s32 func_80015510(Surface16970Header* a, Surface16970Header* b);

extern "C" s32 func_800165C4(Surface16970* a, Surface16970* b) {
    return func_80015510(&a->hdr, &b->hdr);
}

extern "C" u16 func_800165E0(Surface16970* self) {
    return self->hdr.unk14;
}

extern "C" s32 func_800165EC(Surface16970* self) {
    return self->hdr.unk10;
}

extern "C" s32 func_800165F8(Surface16970* self) {
    return self->hdr.unkC;
}

extern "C" s32 func_80016604(Surface16970* self) {
    return self->hdr.unk8;
}

extern "C" s32 func_80016610(Surface16970* self) {
    return self->hdr.unk4;
}

extern "C" s32 func_8001661C(Surface16970* self) {
    return self->hdr.unk0;
}

extern "C" s32 func_8001596C(Surface16970Header* hdr);
extern "C" s32 func_800159A8(Surface16970Header* hdr);
extern "C" s32 func_800159E0(Surface16970Header* hdr);
extern "C" s32 func_80015A18(Surface16970Header* hdr);
extern "C" s32 func_80015A50(Surface16970Header* hdr);
extern "C" s32 func_80015A88(Surface16970Header* hdr);
extern "C" s32 func_80015AC0(Surface16970Header* hdr);
extern "C" s32 func_80015B18(Surface16970Header* hdr);
extern "C" s32 func_80015B6C(Surface16970Header* hdr);
extern "C" s32 func_80015BC0(Surface16970Header* hdr);
extern "C" s32 func_80015C14(Surface16970Header* hdr);
extern "C" s32 func_80015C68(Surface16970Header* hdr);

extern "C" s32 func_80016628(Surface16970* self) {
    return func_8001596C(&self->hdr);
}

extern "C" s32 func_80016644(Surface16970* self) {
    return func_800159A8(&self->hdr);
}

extern "C" s32 func_80016660(Surface16970* self) {
    return func_800159E0(&self->hdr);
}

extern "C" s32 func_8001667C(Surface16970* self) {
    return func_80015A18(&self->hdr);
}

extern "C" s32 func_80016698(Surface16970* self) {
    return func_80015A50(&self->hdr);
}

extern "C" s32 func_800166B4(Surface16970* self) {
    return func_80015A88(&self->hdr);
}

extern "C" s32 func_800166D0(Surface16970* self) {
    return func_80015AC0(&self->hdr);
}

extern "C" s32 func_800166EC(Surface16970* self) {
    return func_80015B18(&self->hdr);
}

extern "C" s32 func_80016708(Surface16970* self) {
    return func_80015B6C(&self->hdr);
}

extern "C" s32 func_80016724(Surface16970* self) {
    return func_80015BC0(&self->hdr);
}

extern "C" s32 func_80016740(Surface16970* self) {
    return func_80015C14(&self->hdr);
}

extern "C" s32 func_8001675C(Surface16970* self) {
    return func_80015C68(&self->hdr);
}

extern "C" s32 func_80016778(Surface16970* self) {
    return self->hdr.unk10 != 0;
}

extern "C" s32 func_80016784(Surface16970* self) {
    return self->hdr.unkC != 0;
}

extern "C" s32 func_80016790(Surface16970* self) {
    return self->hdr.unk14 != 0;
}

extern "C" s32 func_8001679C(Surface16970* self) {
    return self->unk22 & 4;
}

extern "C" s32 func_800167A8(Surface16970* self) {
    return self->unk24 & 2;
}

extern "C" s32 func_800167B4(Surface16970* self) {
    return self->unk22 & 2;
}

extern "C" s32 func_800167C0(Surface16970* self) {
    return self->unk22 & 1;
}

extern "C" void func_800167CC(Surface16970* self, Surface16970* dst) {
    dst->hdr = self->hdr;
}

extern "C" u16 func_80016800(Surface16970* self) {
    return self->hdr.unk16;
}

extern "C" u16 func_8001680C(Surface16970* self) {
    return self->unk28;
}

extern "C" u16 func_80016818(Surface16970* self) {
    return self->unk28;
}

extern "C" u16 func_80016824(Surface16970* self) {
    return self->unk26;
}

extern "C" u16 func_80016830(Surface16970* self) {
    return self->unk26;
}

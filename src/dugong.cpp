#include "common.h"
#include "file.h"
#include "pixel_format.h"

extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strcat(char* dst, const char* src);

extern "C" const char D_80001620[]; // "\nFile: "
extern "C" const char D_80001628[]; // ""
extern "C" const char D_800016A4[]; // ".img"

struct Dugong {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ u8* unkC;
    /* 0x010 */ File file;
    /* 0x040 */ s32 unk40;
    /* 0x044 */ s32 unk44;
    /* 0x048 */ s32 unk48;
    /* 0x04C */ PixelFormat pixelFormat;
    /* 0x064 */ s32 unk64;
    /* 0x068 */ s32 unk68;
    /* 0x06C */ s32 unk6C;
    /* 0x070 */ s32 unk70;
    /* 0x074 */ s32 unk74;
    /* 0x078 */ s32 unk78;
    /* 0x07C */ s32 unk7C;
    /* 0x080 */ s32 unk80;
    /* 0x084 */ s32 unk84;
    /* 0x088 */ s32 unk88;
    /* 0x08C */ s32 unk8C;
    /* 0x090 */ s32 unk90;
    /* 0x094 */ s32 unk94;
    /* 0x098 */ u8 unk98;
    /* 0x099 */ u8 unk99;
    /* 0x09A */ u8 unk9A;
    /* 0x09B */ u8 unk9B;
    /* 0x09C */ u8 unk9C;
    /* 0x09D */ u8 unk9D;
    /* 0x09E */ u8 unk9E;
    /* 0x09F */ u8 unk9F;
    /* 0x0A0 */ s32 unkA0;
    /* 0x0A4 */ u8 palette[0x400];
    /* 0x4A4 */ u8 pad4A4[0x100];
    /* 0x5A4 */ s32 unk5A4;
    /* 0x5A8 */ s32 unk5A8;
    /* 0x5AC */ // vtable

    Dugong();
    virtual void vfunc1();
    virtual ~Dugong();
    virtual void vfunc3(const char* filename);
    virtual void vfunc4();
    virtual const char* vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9();
};

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001B230);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001B5C8);

INCLUDE_RODATA("asm/nonmatchings/dugong", D_80001620);

INCLUDE_RODATA("asm/nonmatchings/dugong", D_80001628);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001B794);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001BAE0);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001BD54);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001BF54);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001C158);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001C3AC);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001C5B0);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001C8E4);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001CC1C);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001D040);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001D378);

INCLUDE_RODATA("asm/nonmatchings/dugong", D_800016A4);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001D500);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001D738);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001DCFC);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001DD90);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001DEC0);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001E0BC);

void Dugong::vfunc9() {
}

void Dugong::vfunc8() {
}

void Dugong::vfunc1() {
}

void Dugong::vfunc7() {
}

void Dugong::vfunc6() {
}

const char* Dugong::vfunc5() {
    return D_800016A4;
}

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001E1DC);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001E320);

INCLUDE_ASM("asm/nonmatchings/dugong", func_8001E464);

extern "C" void func_8001E5A4(Dugong* self, u8* src, u8* dst) {
    u8* end = dst + self->unk40;
    while (dst < end) {
        *dst = self->pad4A4[*src];
        dst++;
        src++;
    }
}

extern "C" void func_8001E5E0(Dugong* self, u8* src, u8* dst) {
    u8* end = dst + self->unk40;
    u8* dst2 = dst + 1;
    while (dst2 < end) {
        *dst = self->pad4A4[*src >> 4];
        *dst2 = self->pad4A4[*src & 0xF];
        dst += 2;
        dst2 += 2;
        src++;
    }
    if (dst < end) {
        *dst = self->pad4A4[*src & 0xF];
    }
}

extern "C" void func_8001E65C(Dugong* self, u8* src, u8* dst) {
    u8* end = dst + ((u32)(self->unk40 + 1) >> 1);
    if (dst < end) {
        do {
            u8 lo = self->pad4A4[*src & 0xF];
            *dst = lo;
            u8 hi = self->pad4A4[*src >> 4];
            *dst = lo | (hi << 4);
            dst++;
            src++;
        } while (dst < end);
    }
}

extern "C" void func_8001E6C0(Dugong* self, u8* src, u8* dst) {
    u8* end = dst + ((u32)(self->unk40 + 1) >> 1);
    if (dst < end) {
        do {
            u8 lo = self->pad4A4[*src & 0xF];
            *dst = lo;
            u8 hi = self->pad4A4[*src >> 4];
            *dst = lo | (hi << 4);
            dst++;
            src++;
        } while (dst < end);
    }
}

void Dugong::vfunc4() {
    file.close();
    if (unkC != NULL) {
        delete[] unkC;
        unkC = NULL;
    }
}

INCLUDE_ASM("asm/nonmatchings/dugong", vfunc3__6DugongPCc);

Dugong::~Dugong() {
    vfunc4();
}

Dugong::Dugong() {
    unk40 = 0;
    unk44 = 0;
    unk48 = 0;
    unk94 = 0;
    unk68 = 0;
    unk98 = 0;
    unk99 = 0;
    unk9A = 0;
    unk9B = 0;
    unk70 = 0;
    unk64 = 0;
    unk9C = 0;
    unk9D = 0;
    unk9E = 0;
    unk9F = 0;
    unkA0 = 0;
    unk5A4 = 0;
    unk5A8 = 0;
    unk0 = 0;
    unk4 = 0;
    unk8 = 0;
    unkC = NULL;
}

extern "C" void func_8001E94C(Dugong* self) {
    self->unk5A8 = 0;
}

extern "C" void func_8001E954(Dugong* self) {
    self->unk5A8 = 1;
}

extern "C" void func_8001E960(Dugong* self) {
    self->unk5A4 = 0;
}

extern "C" void func_8001E968(Dugong* self) {
    self->unk5A4 = 1;
}

extern "C" s32 func_8001E974(Dugong* self) {
    return self->unk48 * self->unk44;
}

extern "C" void func_8001E98C(Dugong* self, u8* src) {
    memcpy(&self->unk9C, src, 4);
}

extern "C" void func_8001E9A4(Dugong* self, u8* src) {
    memcpy(self->palette, src, 0x400);
}

extern "C" u8* func_8001EA40(Dugong* self) {
    return self->palette;
}

extern "C" u16 func_8001EA48(Dugong* self) {
    return self->pixelFormat.paletteMask;
}

extern "C" u32 func_8001EA54(Dugong* self) {
    return self->pixelFormat.unk10;
}

extern "C" u32 func_8001EA60(Dugong* self) {
    return self->pixelFormat.maskAlpha;
}

extern "C" u32 func_8001EA6C(Dugong* self) {
    return self->pixelFormat.maskBlue;
}

extern "C" u32 func_8001EA78(Dugong* self) {
    return self->pixelFormat.maskGreen;
}

extern "C" u32 func_8001EA84(Dugong* self) {
    return self->pixelFormat.maskRed;
}

extern "C" s32 func_8001EA90(Dugong* self) {
    return self->pixelFormat.bitWidthPaletteMask();
}

extern "C" s32 func_8001EAAC(Dugong* self) {
    return self->pixelFormat.bitWidthUnk10();
}

extern "C" s32 func_8001EAC8(Dugong* self) {
    return self->pixelFormat.bitWidthMaskAlpha();
}

extern "C" s32 func_8001EAE4(Dugong* self) {
    return self->pixelFormat.bitWidthMaskBlue();
}

extern "C" s32 func_8001EB00(Dugong* self) {
    return self->pixelFormat.bitWidthMaskGreen();
}

extern "C" s32 func_8001EB1C(Dugong* self) {
    return self->pixelFormat.bitWidthMaskRed();
}

extern "C" s32 func_8001EB38(Dugong* self) {
    return self->pixelFormat.unk10 != 0;
}

extern "C" s32 func_8001EB44(Dugong* self) {
    return self->pixelFormat.maskAlpha != 0;
}

extern "C" s32 func_8001EB50(Dugong* self) {
    return self->pixelFormat.paletteMask != 0;
}

extern "C" u16 func_8001EB5C(Dugong* self) {
    return self->pixelFormat.bitDepth;
}

extern "C" void func_8001EB68(Dugong* self, PixelFormat* out) {
    *out = self->pixelFormat;
}

extern "C" s32 func_8001EBA0(Dugong* self) {
    return self->unk44;
}

extern "C" s32 func_8001EBAC(Dugong* self) {
    return self->unk48;
}

extern "C" s32 func_8001EBB8(Dugong* self) {
    return self->unk40;
}

extern "C" s32 func_8001EBC4(Dugong* self) {
    return self->file.state & 1;
}

// D_80001778 / func_800017C4 / func_800017F4 = auto-generated _vt.4File (compiler emits this)

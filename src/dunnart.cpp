#include "common.h"
#include "dunnart.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390();

extern s32 D_80072C10;
extern "C" const char D_80002CF0[];

#ifdef NON_MATCHING
void Dunnart::vfunc1(u8* dst, s32 start, u32 num) {
    DunnartColor* colors = (DunnartColor*)dst;
    u32 i = 0;
    if (num != 0) {
        do {
            colors[i].r = (data[i + start] & 0xF800) >> 8;

            u16 px = data[i + start];
            s32 g6 = px & 0x40;
            colors[i].g = ((px & 0x7C0) >> 3) | (g6 << 2) | (g6 << 1) | g6;

            u16 px2 = data[i + start];
            colors[i].a = 0;
            s32 bmask = px2 & 0x3E;
            s32 b1 = px2 & 0x2;
            colors[i].b = (bmask << 2) | (b1 << 2) | (b1 << 1) | b1;

            if (data[i + start] & 0x1) {
                colors[i].a = 0xFF;
            }
            i++;
        } while (i < num);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/dunnart", func_800307F0);
#endif

u32 Dunnart::vfunc8() {
    return count;
}

u32 Dunnart::vfunc7() {
    return count;
}

s32 Dunnart::vfunc6() {
    return 0;
}

void Dunnart::func_800308D8(s32 val) {
    D_80072C10 = val;
}

#ifdef NON_MATCHING
s32 Dunnart::vfunc5(u8* rgba) {
    Dunnart* t = this;
    s32 i = 0;
    u8 b = rgba[2];
    u8 a = rgba[3];
    u8 r = rgba[0];
    u8 g = rgba[1];

    u16 pixel = ((r & 0xF8) << 8) | ((g & 0xF8) << 3) | ((b & 0xF8) >> 2) | (a >> 7);

    s32 n = t->count;
    if (n != 0) {
        pixel &= 0xFFFF;
        s32 cnt = n;
        u16* p = t->data;
        do {
            if (*p == pixel) {
                return i;
            }
            i++;
            p++;
        } while ((u32)i < (u32)cnt);
    }
    return -1;
}
#else
INCLUDE_ASM("asm/nonmatchings/dunnart", func_800308E4);
#endif

void Dunnart::vfunc4(Dunnart* src) {
    u8 rgba[4];

    count = src->vfunc7();
    for (u32 i = 0; i < count; i++) {
        src->vfunc3(rgba, i);
        data[i] = ((rgba[0] & 0xF8) << 8) | ((rgba[1] & 0xF8) << 3) |
                  ((rgba[2] & 0xF8) >> 2) | (rgba[3] >> 7);
    }
}

void Dunnart::vfunc3(u8* dst, s32 index) {
    dst[0] = (data[index] & 0xF800) >> 8;

    u16 px = data[index];
    s32 g6 = px & 0x40;
    dst[1] = ((px & 0x7C0) >> 3) | (g6 << 2) | (g6 << 1) | g6;

    u16 px2 = data[index];
    dst[3] = 0;
    s32 bmask = px2 & 0x3E;
    s32 b1 = px2 & 0x2;
    dst[2] = (bmask << 2) | (b1 << 2) | (b1 << 1) | b1;

    if (data[index] & 0x1) {
        dst[3] = 0xFF;
    }
}

void Dunnart::vfunc2(DunnartColor* colors, s32 start, u32 num) {
    u32 i = 0;
    if (num != 0) {
        do {
            data[i + start] = ((colors[i].r & 0xF8) << 8) |
                              ((colors[i].g & 0xF8) << 3) |
                              ((colors[i].b & 0xF8) >> 2) | (colors[i].a >> 7);
            i++;
        } while (i < num);
    }
}

void Dunnart::func_80030B50() {
    count = 0;
    if (data != NULL) {
        delete[] data;
        data = NULL;
    }
}

void Dunnart::func_80030B88(PixelFormat* pf) {
    if (data != NULL) {
        func_80030B50();
    }

    u32 n = 1 << pf->bitDepth;
    count = n;
    func_8004B3BC(D_80072C10);
    data = new u16[count];
    func_8004B390();

    if (data == NULL) {
        __assert(D_80002CF0, 0, 0, 0);
    }

    for (u32 i = 0; i < count; i++) {
        data[i] = 0;
    }
}

Dunnart::~Dunnart() {
    func_80030B50();
}

Dunnart::Dunnart() {
    data = NULL;
    count = 0;
}

s32 Dunnart::func_80030CB0() {
    return D_80072C10;
}

u16* Dunnart::func_80030CC0() {
    return data;
}

s32 Dunnart::func_80030CCC() {
    return data != 0;
}

INCLUDE_RODATA("asm/nonmatchings/dunnart", D_80002CF0);

INCLUDE_RODATA("asm/nonmatchings/dunnart", _vt.7Dunnart);

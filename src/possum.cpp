#include "possum.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);

extern "C" s32 D_80070B40;
extern "C" const char D_800020F0[];

void Possum::func_80026B50(s32 heap) {
    D_80070B40 = heap;
}

s32 Possum::vfunc8() {
    return count;
}

s32 Possum::vfunc7() {
    return count;
}

s32 Possum::vfunc6() {
    return 0;
}

s32 Possum::vfunc5(PossumColor* color) {
    for (u32 i = 0; i < count; i++) {
        if (data[i].r != color->r)
            continue;
        if (data[i].g != color->g)
            continue;
        if (data[i].b != color->b)
            continue;
        if (data[i].a != color->a)
            continue;
        return i;
    }
    return -1;
}

void Possum::vfunc4(PossumBase* src) {
    s32 num = src->vfunc7();
    src->vfunc1(data, 0, num);
}

void Possum::vfunc3(PossumColor* dst, s32 index) {
    dst->r = data[index].r;
    dst->g = data[index].g;
    dst->b = data[index].b;
    dst->a = data[index].a;
}

void Possum::vfunc2(PossumColor* src, s32 start, u32 num) {
    for (u32 i = 0; i < num; i++) {
        data[i + start].r = src[i].r;
        data[i + start].g = src[i].g;
        data[i + start].b = src[i].b;
        data[i + start].a = src[i].a;
    }
}

void Possum::vfunc1(PossumColor* dst, s32 start, u32 num) {
    for (u32 i = 0; i < num; i++) {
        dst[i].r = data[i + start].r;
        dst[i].g = data[i + start].g;
        dst[i].b = data[i + start].b;
        dst[i].a = data[i + start].a;
    }
}

void Possum::func_80026D68() {
    count = 0;
    if (data != NULL) {
        delete[] data;
        data = NULL;
    }
}

void Possum::func_80026DA0(PixelFormat* pf) {
    if (data != NULL) {
        func_80026D68();
    }
    count = 1 << pf->bitDepth;
    func_8004B3BC(D_80070B40);
    data = new PossumColor[count];
    func_8004B390();
    if (data == NULL) {
        __assert(D_800020F0, 0, 0, 0);
    }
    for (u32 i = 0; i < count; i++) {
        data[i].r = 0;
        data[i].g = 0;
        data[i].b = 0;
        data[i].a = 0;
    }
}

Possum::~Possum() {
    func_80026D68();
}

Possum::Possum() {
    data = NULL;
    count = 0;
}

PossumColor* Possum::func_80026EEC() {
    return data;
}

s32 Possum::func_80026EF8() {
    return data != NULL;
}

INCLUDE_RODATA("asm/nonmatchings/possum", D_800020F0);

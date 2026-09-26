#include "common.h"
#include "marsupial.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);

extern s32 D_800740B0;

void Marsupial::vfunc14(PotorooTruffle* adj) {
    s32 c0, c1, c2, c3;
    MarsupialVertex* vertex;
    u8* color;
    u8* end;

    if (unk14 == NULL) {
        func_8004B3BC(D_800740B0);
        unk14 = new u8[unk0 * 4];
        func_8004B390();
        if (unk14 == NULL) {
            __assert("", NULL, 0, NULL);
        }
        vertex = unkC;
        color = unk14;
        end = color + unk0 * 4;
        while (color < end) {
            color[0] = vertex->unkC;
            color[1] = vertex->unkD;
            color[2] = vertex->unkE;
            color[3] = vertex->unkF;
            vertex++;
            color += 4;
        }
    }

    vertex = unkC;
    color = unk14;
    end = color + unk0 * 4;
    while (color < end) {
        c0 = color[0];
        c1 = color[1];
        c2 = color[2];
        c3 = color[3];

        c0 = (c0 >> adj->unk0) + adj->unk10;
        if (c0 >= 256)
            c0 = 255;

        c1 = (c1 >> adj->unk4) + adj->unk14;
        if (c1 >= 256)
            c1 = 255;

        c2 = (c2 >> adj->unk8) + adj->unk18;
        if (c2 >= 256)
            c2 = 255;

        c3 = (c3 >> adj->unkC) + adj->unk1C;
        if (c3 >= 256)
            c3 = 255;

        vertex->unkC = c0;
        vertex->unkD = c1;
        vertex->unkE = c2;
        vertex->unkF = c3;
        vertex++;
        color += 4;
    }
    unk10 = 1;
}

void Marsupial::vfunc15(void) {
    if (unk10 == 0) {
        return;
    }
    u8* src = unk14;
    MarsupialVertex* dst = unkC;
    u8* end = src + unk0 * 4;
    while (src < end) {
        dst->unkC = src[0];
        dst->unkD = src[1];
        dst->unkE = src[2];
        dst->unkF = src[3];
        dst++;
        src += 4;
    }
    unk10 = 0;
}

void Marsupial::func_8003B1B4(s32 arg0) {
    D_800740B0 = arg0;
}

void Marsupial::vfunc4(void) {
    unk0 = 0;
    if (unk14 != NULL) {
        delete[] unk14;
        unk14 = NULL;
    }
    unk10 = 0;
}

Marsupial::~Marsupial() {
    vfunc4();
}

Marsupial::Marsupial() {
    unk0 = 0;
    unk14 = NULL;
    unk10 = 0;
}

MarsupialVertex* Marsupial::func_8003B284(s32 index) {
    return &unkC[index];
}

u16 Marsupial::func_8003B294() {
    return unk0;
}

s32 Marsupial::func_8003B2A0() {
    return unk0 != 0;
}

#include "common.h"
#include "marsupial.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);

extern s32 D_800740B0;
extern "C" const char D_80003970[];

// load ordering diff (unkC vs unk0 field load order) + register allocation
#ifdef NON_MATCHING
void Marsupial::vfunc14(PotorooTruffle* adj) {
    s32 c0, c1, c2, c3;

    if (unk14 == NULL) {
        func_8004B3BC(D_800740B0);
        unk14 = new u8[unk0 * 4];
        func_8004B390();
        if (unk14 == NULL) {
            __assert(D_80003970, NULL, 0, NULL);
        }
        MarsupialVertex* src = unkC;
        u8* dst = unk14;
        u8* end = dst + (u32)unk0 * 4;
        while (dst < end) {
            dst[0] = src->unkC;
            dst[1] = src->unkD;
            dst[2] = src->unkE;
            dst[3] = src->unkF;
            src++;
            dst += 4;
        }
    }

    MarsupialVertex* dst2 = unkC;
    u8* src2 = unk14;
    u8* end2 = src2 + (u32)unk0 * 4;
    s32 result = 1;
    if (src2 < end2) {
        do {
            c0 = src2[0];
            c1 = src2[1];
            c2 = src2[2];
            c3 = src2[3];

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

            dst2->unkC = c0;
            dst2->unkD = c1;
            dst2->unkE = c2;
            dst2->unkF = c3;
            dst2++;
            src2 += 4;
        } while (src2 < end2);
        result = 1;
    }
    unk10 = result;
}
#else
INCLUDE_ASM("asm/nonmatchings/marsupial", vfunc14__9MarsupialP14PotorooTruffle);
#endif

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

INCLUDE_RODATA("asm/nonmatchings/marsupial", D_80003970);

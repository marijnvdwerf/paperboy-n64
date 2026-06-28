#include "currawong.h"
#include "parrot.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);
extern s32 D_800740B0;

void Currawong::vfunc2(Parrot* parrot) {
    if (unk0 != 0) {
        vfunc4();
    }
    unk2 = 3;
    parrot->expectToken(TOKEN_OPEN_BRACKET);
    s32 count = parrot->readInt();
    if (count == 0) {
        parrot->parseError(4);
    }
    parrot->expectToken(TOKEN_CLOSE_BRACKET);
    parrot->expectToken(TOKEN_OPEN_BRACE);
    vfunc3(count);
    for (u32 i = 0; i < unk0; i++) {
        unkC[i].unk0 = (s16)parrot->readFloat();
        unkC[i].unk2 = (s16)parrot->readFloat();
        unkC[i].unk4 = (s16)parrot->readFloat();
        unkC[i].unk6 = 0;
        unkC[i].unk8 = (s16)(parrot->readFloat() * 127.0f * 32.0f);
        unkC[i].unkA = (s16)(parrot->readFloat() * 127.0f * 32.0f);
        unkC[i].unkC = 0xFF;
        unkC[i].unkD = 0xFF;
        unkC[i].unkE = 0xFF;
        unkC[i].unkF = 0xFF;
        unk18[i].x = parrot->readFloat();
        unk18[i].y = parrot->readFloat();
        unk18[i].z = parrot->readFloat();
    }
    parrot->expectToken(TOKEN_CLOSE_BRACE);
}

void Currawong::vfunc5(void) {
    if (unk18 != NULL) {
        delete[] unk18;
        unk18 = NULL;
    }
    unk2 = 1;
}

void Currawong::vfunc12(s32 index, Vec3f* src) {
    unk18[index] = *src;
}

void Currawong::vfunc8(s32 index, Vec3f* out) {
    *out = unk18[index];
}

void Currawong::vfunc4(void) {
    if (unkC != NULL) {
        delete[] unkC;
        unkC = NULL;
    }
    if (unk18 != NULL) {
        delete[] unk18;
        unk18 = NULL;
    }
    unk2 = 1;
}

void Currawong::vfunc3(s32 newCount) {
    if (unk0 != 0) {
        vfunc4();
    }
    unk2 = 3;
    unk0 = newCount;
    func_8004B3BC(D_800740B0);
    unkC = new MarsupialVertex[unk0];
    unk18 = new Vec3f[unk0];
    func_8004B390();
    if (!unkC || !unk18) {
        __assert("", NULL, 0, NULL);
    }
    memset(unkC, 0, 4);
    memset(unk18, 0, 4);
}

Currawong::Currawong() {
    unkC = NULL;
    unk18 = NULL;
    unk2 = 1;
}

INCLUDE_ASM("asm/nonmatchings/currawong", _._9Currawong);

__asm__(".section .rdata\n.word 0\n");
INCLUDE_RODATA("asm/nonmatchings/currawong", _vt.9Currawong);

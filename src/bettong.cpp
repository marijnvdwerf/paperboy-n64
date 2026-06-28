#include "bettong.h"
#include "parrot.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);

extern "C" s32 D_800740B0;

void Bettong::vfunc2(Parrot* parrot) {
    if (unk0 != 0) {
        vfunc4();
    }
    parrot->expectToken(TOKEN_OPEN_BRACKET);
    unk0 = parrot->readInt();
    if (unk0 == 0) {
        parrot->parseError(4);
    }
    parrot->expectToken(TOKEN_CLOSE_BRACKET);
    parrot->expectToken(TOKEN_OPEN_BRACE);
    func_8004B3BC(D_800740B0);
    unkC = new MarsupialVertex[unk0];
    func_8004B390();
    if (!unkC) {
        __assert("", NULL, 0, NULL);
    }
    memset(unkC, 0, unk0 * sizeof(MarsupialVertex));

    for (u32 i = 0; i < unk0; i++) {
        unkC[i].unk0 = (s16)parrot->readFloat();
        unkC[i].unk2 = (s16)parrot->readFloat();
        unkC[i].unk4 = (s16)parrot->readFloat();
        unkC[i].unk8 = (s16)(parrot->readFloat() * 127.0f * 32.0f);
        unkC[i].unkA = (s16)(parrot->readFloat() * 127.0f * 32.0f);
        unkC[i].unk6 = 0;
        unkC[i].unkC = parrot->readInt();
        unkC[i].unkD = parrot->readInt();
        unkC[i].unkE = parrot->readInt();
        unkC[i].unkF = parrot->readInt();
    }
    parrot->expectToken(TOKEN_CLOSE_BRACE);
}

void Bettong::vfunc13(s32 index, u8* src) {
    unkC[index].unkC = src[0];
    unkC[index].unkD = src[1];
    unkC[index].unkE = src[2];
    unkC[index].unkF = src[3];
}

__asm__(".section .rdata\n.word 0\n");
INCLUDE_RODATA("asm/nonmatchings/bettong", _vt.7Bettong);

void Bettong::vfunc12(s32 index, Vec3f* src) {
}

void Bettong::vfunc11(s32 index, Vec3f* src) {
    unkC[index].unk8 = (s16)(src->x * 127.0f * 32.0f);
    unkC[index].unkA = (s16)(src->y * 127.0f * 32.0f);
}

void Bettong::vfunc10(s32 index, Vec3f* src) {
    unkC[index].unk0 = (s16)src->x;
    unkC[index].unk2 = (s16)src->y;
    unkC[index].unk4 = (s16)src->z;
}

void Bettong::vfunc9(s32 index, u8* out) {
    out[0] = unkC[index].unkC;
    out[1] = unkC[index].unkD;
    out[2] = unkC[index].unkE;
    out[3] = unkC[index].unkF;
}

void Bettong::vfunc8(s32 index, Vec3f* out) {
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
}

void Bettong::vfunc7(s32 index, Vec3f* out) {
    out->x = (f32)unkC[index].unk8 * 0.03125f / 127.0f;
    out->y = (f32)unkC[index].unkA * 0.03125f / 127.0f;
}

void Bettong::vfunc6(s32 index, Vec3f* out) {
    out->x = (f32)unkC[index].unk0;
    out->y = (f32)unkC[index].unk2;
    out->z = (f32)unkC[index].unk4;
}

void Bettong::vfunc4(void) {
    if (unkC != NULL) {
        delete[] unkC;
        unkC = NULL;
    }
}

void Bettong::vfunc3(s32 newCount) {
    if (unk0 != 0) {
        vfunc4();
    }
    unk0 = newCount;
    func_8004B3BC(D_800740B0);
    unkC = new MarsupialVertex[unk0];
    func_8004B390();
    if (!unkC) {
        __assert("", NULL, 0, NULL);
    }
    memset(unkC, 0, unk0 * sizeof(MarsupialVertex));
}

Bettong::Bettong() {
    unkC = NULL;
    unk2 = 1;
}

INCLUDE_ASM("asm/nonmatchings/bettong", _._7Bettong);

#include "wallaby.h"
#include "parrot.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);

extern "C" s32 D_800740B0;

// INCLUDE_RODATA("asm/nonmatchings/wallaby", D_80003830);

void Wallaby::vfunc2(Parrot* parrot) {
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
    if (unkC == NULL) {
        __assert("", 0, 0, 0);
    }
    memset(unkC, 0, unk0 * sizeof(MarsupialVertex));
    for (u32 i = 0; i < unk0; i++) {
        unkC[i].unk0 = (s16)parrot->readFloat();
        unkC[i].unk2 = (s16)parrot->readFloat();
        unkC[i].unk4 = (s16)parrot->readFloat();
        unkC[i].unk6 = 0;
        unkC[i].unk8 = (s16)(parrot->readFloat() * 127.0f * 32.0f);
        unkC[i].unkA = (s16)(parrot->readFloat() * 127.0f * 32.0f);
        unkC[i].unkC = (s8)(parrot->readFloat() * 127.0f);
        unkC[i].unkD = (s8)(parrot->readFloat() * 127.0f);
        unkC[i].unkE = (s8)(parrot->readFloat() * 127.0f);
        unkC[i].unkF = 0xFF;
    }
    parrot->expectToken(TOKEN_CLOSE_BRACE);
}

void Wallaby::vfunc13(void) {
}

__asm__(".section .rdata\n"
        "    .word 0\n");
INCLUDE_RODATA("asm/nonmatchings/wallaby", _vt.7Wallaby);

void Wallaby::vfunc12(s32 index, Vec3f* src) {
    unkC[index].unkC = (s8)(src->x * 127.0f);
    unkC[index].unkD = (s8)(src->y * 127.0f);
    unkC[index].unkE = (s8)(src->z * 127.0f);
    unkC[index].unkF = 0xFF;
}

void Wallaby::vfunc11(s32 index, Vec3f* src) {
    unkC[index].unk8 = (s16)(src->x * 127.0f * 32.0f);
    unkC[index].unkA = (s16)(src->y * 127.0f * 32.0f);
}

void Wallaby::vfunc10(s32 index, Vec3f* src) {
    unkC[index].unk0 = (s16)src->x;
    unkC[index].unk2 = (s16)src->y;
    unkC[index].unk4 = (s16)src->z;
}

void Wallaby::vfunc9(s32, u8*) {
}

void Wallaby::vfunc8(s32 index, Vec3f* out) {
    out->x = (f32)(s8)unkC[index].unkC / 127.0f;
    out->y = (f32)(s8)unkC[index].unkD / 127.0f;
    out->z = (f32)(s8)unkC[index].unkE / 127.0f;
}

void Wallaby::vfunc7(s32 index, Vec3f* out) {
    out->x = (f32)unkC[index].unk8 * 0.03125f / 127.0f;
    out->y = (f32)unkC[index].unkA * 0.03125f / 127.0f;
}

void Wallaby::vfunc6(s32 index, Vec3f* out) {
    out->x = (f32)unkC[index].unk0;
    out->y = (f32)unkC[index].unk2;
    out->z = (f32)unkC[index].unk4;
}

void Wallaby::vfunc4(void) {
    if (unkC != NULL) {
        delete[] unkC;
        unkC = NULL;
    }
}

void Wallaby::vfunc3(s32 count) {
    if (unk0 != 0) {
        vfunc4();
    }
    unk0 = count;
    func_8004B3BC(D_800740B0);
    unkC = new MarsupialVertex[unk0];
    func_8004B390();
    if (unkC == NULL) {
        __assert("", 0, 0, 0);
    }
    memset(unkC, 0, unk0 * sizeof(MarsupialVertex));
}

Wallaby::Wallaby() {
    unkC = NULL;
    unk2 = 2;
}

INCLUDE_ASM("asm/nonmatchings/wallaby", _._7Wallaby);

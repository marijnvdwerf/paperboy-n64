#include "wombat.h"
#include "parrot.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);

extern "C" s32 D_800740B0;

// vfunc2
extern "C" void func_800394B0(Wombat* self, Parrot* parrot) {
    if (self->unk0 != 0) {
        self->vfunc4();
    }
    parrot->expectToken(TOKEN_OPEN_BRACKET);
    self->unk0 = parrot->readInt();
    if (self->unk0 == 0) {
        parrot->parseError(4);
    }
    parrot->expectToken(TOKEN_CLOSE_BRACKET);
    parrot->expectToken(TOKEN_OPEN_BRACE);
    func_8004B3BC(D_800740B0);
    self->unkC = new MarsupialVertex[self->unk0];
    func_8004B390();
    if (self->unkC == NULL) {
        __assert("", 0, 0, 0);
    }
    memset(self->unkC, 0, self->unk0 * sizeof(MarsupialVertex));
    for (u32 i = 0; i < self->unk0; i++) {
        self->unkC[i].unk0 = (s16)parrot->readFloat();
        self->unkC[i].unk2 = (s16)parrot->readFloat();
        self->unkC[i].unk4 = (s16)parrot->readFloat();
        self->unkC[i].unk8 = (s16)(parrot->readFloat() * 127.0f * 32.0f);
        self->unkC[i].unkA = (s16)(parrot->readFloat() * 127.0f * 32.0f);
        self->unkC[i].unk6 = 0;
        self->unkC[i].unkC = 0xFF;
        self->unkC[i].unkD = 0xFF;
        self->unkC[i].unkE = 0xFF;
        self->unkC[i].unkF = 0xFF;
    }
    parrot->expectToken(TOKEN_CLOSE_BRACE);
}

// vfunc13
extern "C" void func_800396F0(Wombat* self, s32 index, u8* src) {
    self->unkC[index].unkC = src[0];
    self->unkC[index].unkD = src[1];
    self->unkC[index].unkE = src[2];
    self->unkC[index].unkF = src[3];
}

extern "C" void func_80039738(void) {
}

__asm__(".section .rdata\n"
        "    .word 0\n");
INCLUDE_RODATA("asm/nonmatchings/wombat", _vt.6Wombat);

// vfunc11
extern "C" void func_80039740(Wombat* self, s32 index, Vec3f* src) {
    self->unkC[index].unk8 = (s16)(src->x * 127.0f * 32.0f);
    self->unkC[index].unkA = (s16)(src->y * 127.0f * 32.0f);
}

// vfunc10
extern "C" void func_8003979C(Wombat* self, s32 index, Vec3f* src) {
    self->unkC[index].unk0 = (s16)src->x;
    self->unkC[index].unk2 = (s16)src->y;
    self->unkC[index].unk4 = (s16)src->z;
}

// vfunc9
extern "C" void func_800397EC(Wombat* self, s32 index, u8* out) {
    out[0] = self->unkC[index].unkC;
    out[1] = self->unkC[index].unkD;
    out[2] = self->unkC[index].unkE;
    out[3] = self->unkC[index].unkF;
}

// vfunc8
extern "C" void func_80039834(Wombat* self, s32 index, Vec3f* out) {
    out->x = 0;
    out->y = 0;
    out->z = 0;
}

// vfunc7
extern "C" void func_80039844(Wombat* self, s32 index, Vec3f* out) {
    out->x = (f32)self->unkC[index].unk8 * 0.03125f / 127.0f;
    out->y = (f32)self->unkC[index].unkA * 0.03125f / 127.0f;
}

// vfunc6
extern "C" void func_8003989C(Wombat* self, s32 index, Vec3f* out) {
    out->x = (f32)self->unkC[index].unk0;
    out->y = (f32)self->unkC[index].unk2;
    out->z = (f32)self->unkC[index].unk4;
}

// vfunc3
extern "C" void func_800398EC(Wombat* self, u16 count) {
    if (self->unk0 != 0) {
        self->vfunc4();
    }
    self->unk0 = count;
    func_8004B3BC(D_800740B0);
    self->unkC = new MarsupialVertex[self->unk0];
    func_8004B390();
    if (self->unkC == NULL) {
        __assert("", 0, 0, 0);
    }
    memset(self->unkC, 0, self->unk0 * sizeof(MarsupialVertex));
}

// vfunc4
extern "C" void func_8003998C(Wombat* self) {
    if (self->unkC != NULL) {
        delete[] self->unkC;
        self->unkC = NULL;
    }
}

Wombat::Wombat() {
    unkC = NULL;
    unk2 = 1;
}

INCLUDE_ASM("asm/nonmatchings/wombat", _._6Wombat);

#include "common.h"
#include "potoroo.h"
#include "parrot.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);

extern s32 D_80070B90;
extern "C" const char D_800024C0[];

void Potoroo::vfunc2(Parrot* parrot) {
    if (unk0 != 0) {
        vfunc4();
    }
    unk0 = parrot->beginArray();
    func_8004B3BC(D_80070B90);
    unk4 = new Vec3f[unk0];
    func_8004B390();
    if (unk4 == NULL) {
        __assert(D_800024C0, NULL, 0, NULL);
    }
    memset(unk4, 0, unk0 * sizeof(Vec3f));
    for (u32 i = 0; i < unk0; i++) {
        unk4[i].x = parrot->readFloat();
        unk4[i].y = parrot->readFloat();
        unk4[i].z = parrot->readFloat();
    }
    parrot->expectToken(TOKEN_CLOSE_BRACE);
}

void Potoroo::func_80029654(s32 arg0) {
    D_80070B90 = arg0;
}

void Potoroo::vfunc5(void) {
}

void Potoroo::vfunc15(void) {
}

void Potoroo::vfunc14(void) {
}

void Potoroo::vfunc13(void) {
}

void Potoroo::vfunc12(void) {
}

void Potoroo::vfunc11(void) {
}

void Potoroo::vfunc10(s32 index, Vec3f* src) {
    unk4[index].x = src->x;
    unk4[index].y = src->y;
    unk4[index].z = src->z;
}

void Potoroo::vfunc9(UNK, u8* out) {
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
}

void Potoroo::vfunc8(UNK, Vec3f* out) {
    out->x = 0;
    out->y = 0;
    out->z = 0;
}

void Potoroo::vfunc7(UNK, s32* out) {
    out[0] = 0;
    out[1] = 0;
}

void Potoroo::vfunc6(s32 index, Vec3f* out) {
    out->x = unk4[index].x;
    out->y = unk4[index].y;
    out->z = unk4[index].z;
}

void Potoroo::vfunc4(void) {
    if (unk4 != NULL) {
        delete[] unk4;
        unk4 = NULL;
    }
}

#ifdef NON_MATCHING
void Potoroo::vfunc3(s32 newCount) {
    Vec3f* mem;

    if (unk0 != 0) {
        vfunc4();
    }
    unk0 = newCount;
    mem = new Vec3f[(u16)newCount];
    unk4 = mem;
    if (mem == NULL) {
        __assert(D_800024C0, NULL, 0, NULL);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/potoroo", vfunc3__7Potorool);
#endif

Potoroo::~Potoroo() {
    vfunc4();
}

Potoroo::Potoroo() {
    unk0 = 0;
    unk2 = 5;
    unk4 = NULL;
}

u16 Potoroo::func_8002986C() {
    return unk2;
}

u16 Potoroo::func_80029878() {
    return unk0;
}

s32 Potoroo::func_80029884() {
    return unk0 != 0;
}

Vec3f* Potoroo::func_80029890() {
    return unk4;
}

INCLUDE_RODATA("asm/nonmatchings/potoroo", D_800024C0);

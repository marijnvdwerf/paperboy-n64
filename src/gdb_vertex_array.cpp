#include "common.h"
#include "gdb_vertex_array.h"
#include "gol_file_parser.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);

extern s32 D_80070B90;
extern "C" const char D_800024C0[];

void GdbVertexArray::vfunc2(GolFileParser* parser) {
    if (unk0 != 0) {
        vfunc4();
    }
    unk0 = parser->beginArray();
    func_8004B3BC(D_80070B90);
    unk4 = new Vec3f[unk0];
    func_8004B390();
    if (unk4 == NULL) {
        __assert(D_800024C0, NULL, 0, NULL);
    }
    memset(unk4, 0, unk0 * sizeof(Vec3f));
    for (u32 i = 0; i < unk0; i++) {
        unk4[i].x = parser->readFloat();
        unk4[i].y = parser->readFloat();
        unk4[i].z = parser->readFloat();
    }
    parser->expectToken(TOKEN_CLOSE_BRACE);
}

void GdbVertexArray::func_80029654(s32 arg0) {
    D_80070B90 = arg0;
}

void GdbVertexArray::vfunc5(void) {
}

void GdbVertexArray::vfunc15(void) {
}

void GdbVertexArray::vfunc14(PotorooTruffle* op) {
}

void GdbVertexArray::vfunc13(s32, u8*) {
}

void GdbVertexArray::vfunc12(s32, Vec3f*) {
}

void GdbVertexArray::vfunc11(s32, Vec3f*) {
}

void GdbVertexArray::vfunc10(s32 index, Vec3f* src) {
    unk4[index].x = src->x;
    unk4[index].y = src->y;
    unk4[index].z = src->z;
}

void GdbVertexArray::vfunc9(s32, u8* out) {
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
}

void GdbVertexArray::vfunc8(s32, Vec3f* out) {
    out->x = 0;
    out->y = 0;
    out->z = 0;
}

void GdbVertexArray::vfunc7(s32, Vec3f* out) {
    out->x = 0;
    out->y = 0;
}

void GdbVertexArray::vfunc6(s32 index, Vec3f* out) {
    out->x = unk4[index].x;
    out->y = unk4[index].y;
    out->z = unk4[index].z;
}

void GdbVertexArray::vfunc4(void) {
    if (unk4 != NULL) {
        delete[] unk4;
        unk4 = NULL;
    }
}

void GdbVertexArray::vfunc3(s32 newCount) {
    if (unk0 != 0) {
        vfunc4();
    }
    unk0 = newCount;
    unk4 = new Vec3f[unk0];
    if (unk4 == NULL) {
        __assert(D_800024C0, NULL, 0, NULL);
    }
}

GdbVertexArray::~GdbVertexArray() {
    vfunc4();
}

GdbVertexArray::GdbVertexArray() {
    unk0 = 0;
    unk2 = 5;
    unk4 = NULL;
}

u16 GdbVertexArray::func_8002986C() {
    return unk2;
}

u16 GdbVertexArray::func_80029878() {
    return unk0;
}

s32 GdbVertexArray::func_80029884() {
    return unk0 != 0;
}

Vec3f* GdbVertexArray::func_80029890() {
    return unk4;
}

INCLUDE_RODATA("asm/nonmatchings/gdb_vertex_array", D_800024C0);

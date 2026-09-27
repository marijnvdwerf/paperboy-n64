#ifndef GOL_MODEL_BASE_H
#define GOL_MODEL_BASE_H

#include "gdb_model_index_array.h"
#include "gdb_vertex_array.h"
#include "gol_model_material_table.h"
#include "vector.h"

extern s32 D_80070B20;

struct PlatypusPlane {
    /* 0x00 */ Vec3f normal;
    /* 0x0C */ f32 distance;
};

struct GolModelBase {
    /* 0x00 */ GolModelMaterialTable materialTable;
    /* 0x0C */ GdbVertexArray* unkC;
    /* 0x10 */ GdbVertexArray* unk10;
    /* 0x14 */ GdbModelIndexArray* unk14;
    /* 0x18 */ PlatypusPlane* unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u32* unk20;
    /* 0x24 */ Vec3f unk24;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ Vec3f unk34;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s32 unk44;

    s32 func_80023E24() {
        return unk20 != NULL;
    }

    GolModelMaterialTable* func_80023E1C() {
        return &materialTable;
    }

    void func_80023DF8(Vec3f* out, f32* radius) {
        *out = unk24;
        *radius = unk30;
    }

    void func_80023DD8(Vec3f* out) {
        *out = unk34;
    }

    void func_80023DB8(Vec3f* pos, f32 radius) {
        unk24 = *pos;
        unk30 = radius;
    }

    u32 func_80023DAC() {
        return unk1C;
    }

    u32 func_80023D94(s32 idx) {
        return unk20[idx];
    }

    u32* func_80023D88() {
        return unk20;
    }

    void func_80023D74(s32 idx, u32 val) {
        unk20[idx] = val;
    }

    void func_80023D00(s32 idx, s32 arg2, s32 arg3, s32 arg4) {
        unk20[idx] = 0;
        unk20[idx] |= (arg2 & 0x3F) << 22;
        unk20[idx] |= ((arg4 - 1) & 0x3F) << 16;
        unk20[idx] |= arg3 & 0xFFFF;
        unk44 = 1;
    }

    void func_80023CAC(s32 idx, s32 arg2, s32 arg3) {
        unk20[idx] = 0x10000000;
        unk20[idx] |= (arg3 & 0xFFF) << 16;
        unk20[idx] |= arg2 & 0xFFFF;
        unk44 = 1;
    }

    void func_80023C6C(s32 idx, s32 val) {
        unk20[idx] = 0x50000000;
        unk20[idx] |= val & 0xFFFFFF;
        unk44 = 1;
    }

    void func_80023C2C(s32 idx, s32 val) {
        unk20[idx] = 0x40000000;
        unk20[idx] |= val & 0xFFFFFF;
        unk44 = 1;
    }

    void func_80023C0C(s32 idx) {
        unk20[idx] = 0x60000000;
        unk44 = 1;
    }

    s32 func_80023C00() {
        return unk44;
    }

    f32 func_80023BF4() {
        return unk40;
    }

    void func_80023BEC(f32 arg1) {
        unk40 = arg1;
    }

    PlatypusPlane* func_80023BDC(s32 idx) {
        return &unk18[idx];
    }

    s32 func_80023BB0() {
        u16 val = unkC->unk2;
        // TODO: The named constant prevents this comparison from becoming a range test.
        s32 two = 2;
        s32 result = 0;
        if (val == two || val == 3) {
            result = 1;
        }
        return result;
    }

    u32 func_80023BA0() {
        return unk14->unk0;
    }

    GdbModelIndexArray* func_80023B94() {
        return unk14;
    }

    static s32 func_80023B84() {
        return D_80070B20;
    }

    GolModelBase();

    virtual void vfunc1(GolFileParser* file);
    virtual void vfunc2(GolFileParser* file);
    virtual void vfunc3(GolFileParser* file);
    virtual void vfunc4(GolFileParser* file) = 0;
    virtual void vfunc5(GolFileParser* file) = 0;
    virtual void vfunc6(GolFileParser* file);
    virtual ~GolModelBase();
    virtual void vfunc8(GolRenderDevice* ctx, const char* path, GolFileParser* file);
    virtual void vfunc9() = 0;
    virtual void vfunc10();
    virtual void vfunc11(GdbVertexArray** out);
    virtual void vfunc12(s32 arg1, s32 arg2);
    virtual void vfunc13(GdbModelIndexArray** out);
    virtual void vfunc14(s32 flags);
    virtual void vfunc15(Vec3f* center, f32* radius, f32 scale);
    virtual void vfunc16(PotorooTruffle* op);
    virtual void vfunc17();
};

#endif

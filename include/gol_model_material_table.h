#ifndef GOL_MODEL_MATERIAL_TABLE_H
#define GOL_MODEL_MATERIAL_TABLE_H

#include "common.h"

class GolRenderDevice;
class GolFileParser;

class GolMaterial;

struct GolModelMaterialTable {
    /* 0x00 */ GolRenderDevice* context;
    /* 0x04 */ u32 count;
    /* 0x08 */ GolMaterial** entries;

    GolModelMaterialTable();
    ~GolModelMaterialTable();

    void func_80021380(GolRenderDevice* ctx, GolFileParser* file);
    static void func_8002151C(s32 value);
    void func_80021528(s32 index, GolMaterial* value);
    void func_8002153C(s32 index, const char* name);
    s32 func_800215DC(const char* name);
    void func_80021680();
    void func_800216C0(GolRenderDevice* ctx, const char* path, s32 useBinaryParser);
    void func_80021808(GolRenderDevice* ctx, u32 count);
};

#endif

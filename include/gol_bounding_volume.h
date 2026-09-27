#ifndef GOL_BOUNDING_VOLUME_H
#define GOL_BOUNDING_VOLUME_H

#include "common.h"
#include "gdb_vertex_array.h"
#include "gol_model_material_table.h"
#include "vector.h"

struct BspNode;
struct BspFace;
struct BspStackEntry;
struct TriData;
class GolFileParser;
struct GolRenderDevice;

struct GolBoundingVolume {
    /* 0x00 */ u32 numNodes;
    /* 0x04 */ BspNode* nodes;
    /* 0x08 */ GdbVertexArray* mesh;
    /* 0x0C */ u32 numFaces;
    /* 0x10 */ BspFace* faces;
    /* 0x14 */ GolModelMaterialTable materialTable;
    /* 0x20 */ GolModelMaterialTable* materialTablePtr;
    /* 0x24 */ s32 depth;
    /* 0x28 */ BspStackEntry* stack;
    /* 0x2C */ Vec3f start;
    /* 0x38 */ Vec3f end;
    /* 0x44 */ f32 distA;
    /* 0x48 */ f32 distB;
    /* 0x4C */ BspNode* hitNode;
    /* 0x50 */ TriData* triData;
    /* 0x54 */ Vec3f* hitPoint;
    /* 0x58 */ s32* hitIndex;
    /* 0x5C */ f32* hitT;
    /* 0x60 */ // vtable

    GolBoundingVolume();
    virtual ~GolBoundingVolume();
    virtual void vfunc2(GolRenderDevice* ctx, const char* path, s32 useBinaryParser);
    virtual void vfunc3();

    s32 func_80120344();
    s32 func_8012021C(Vec3f* point);
    void func_80120AD0();
    s32 func_8011F7B4(Vec3f* start, Vec3f* end, TriData* triData, Vec3f* hitPoint, s32* hitIndex, f32* hitT);
    void func_8011F404(GolFileParser* file);
    void func_8011F5F4(GolFileParser* file);
    s32 func_8011FF88();
    void func_8012084C(GolFileParser* file);
    void func_80120954(GolFileParser* file);

    s32 func_80120BDC() {
        return this->nodes != NULL;
    }

    void func_80120BC4(GolModelMaterialTable* s) {
        if (s == NULL) {
            this->materialTablePtr = &this->materialTable;
        } else {
            this->materialTablePtr = s;
        }
    }

    GolModelMaterialTable* func_80120BBC() {
        return &this->materialTable;
    }

    GdbVertexArray* func_80120BB0() {
        return this->mesh;
    }
};

#endif

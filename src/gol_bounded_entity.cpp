#include "common.h"
#include "gol_bounding_volume.h"
#include "gol_oriented_entity.h"

struct GolBoundedEntity : public GolOrientedEntity {
    /* 0x64 */ GolBoundingVolume* boundingVolume;
    /* 0x68 */ char pad68[4];
    /* 0x6C */ s32 unk6C;

    GolBoundedEntity();
    void func_8011F070(Vec3f* a, Vec3f* b, TriData* triData, Vec3f* hitPoint, s32* hitIndex);
    void func_8011F118();
    void func_8011F124(GolBoundingVolume* j);
};

void GolBoundedEntity::func_8011F070(Vec3f* a, Vec3f* b, TriData* triData, Vec3f* hitPoint, s32* hitIndex) {
    Vec3f localA;
    Vec3f localB;
    vfunc13(a, &localA);
    vfunc13(b, &localB);
    boundingVolume->func_8011F7B4(&localA, &localB, triData, hitPoint, hitIndex, NULL);
}

void GolBoundedEntity::func_8011F118() {
    boundingVolume = NULL;
    unk6C = 0;
}

void GolBoundedEntity::func_8011F124(GolBoundingVolume* j) {
    boundingVolume = j;
}

GolBoundedEntity::GolBoundedEntity() {
    boundingVolume = NULL;
    unk6C = 0;
}

#include "common.h"
#include "dingo.h"
#include "vector.h"

struct Pademelon : public Dingo {
    /* 0x34 */ Mat3f rotation;
    /* 0x58 */ Vec3f translation;

    Pademelon();

    void vfunc2(Vec3f* out) CXX_OVERRIDE;
    void vfunc3(Vec3f* v) CXX_OVERRIDE;
    void vfunc12(Vec3f* a, Vec3f* out) CXX_OVERRIDE;
    void vfunc13(Vec3f* a, Vec3f* out) CXX_OVERRIDE;
    void vfunc14(Vec3f* src, Vec3f* dst) CXX_OVERRIDE;
    void vfunc15(Vec3f* src, Vec3f* dst) CXX_OVERRIDE;
    void vfunc16(Vec3f* dir, Vec3f* up) CXX_OVERRIDE;
    void vfunc17(Vec3f* a, Vec3f* b) CXX_OVERRIDE;
    void vfunc18(Mat3f* out) CXX_OVERRIDE;
    void vfunc19(Mat3f* mat) CXX_OVERRIDE;
};

// vfunc16 - build rotation from direction and up vector
void Pademelon::vfunc16(Vec3f* dir, Vec3f* up) {
    Vec3f forward;
    Vec3f cross;
    Vec3f side;
    Vec3f proj;

    vec3_normalize(dir, &forward);
    Vec3f* fwd = &forward;

    f32 dot = up->x * fwd->x + up->y * fwd->y + up->z * fwd->z;
    proj.x = fwd->x * dot;
    proj.y = fwd->y * dot;
    proj.z = fwd->z * dot;
    side.x = up->x - proj.x;
    side.y = up->y - proj.y;
    side.z = up->z - proj.z;
    vec3_normalize(&side, &side);

    cross.x = side.y * fwd->z - side.z * fwd->y;
    cross.y = side.z * fwd->x - side.x * fwd->z;
    cross.z = side.x * fwd->y - side.y * fwd->x;

    rotation.m[0][0] = forward.x;
    rotation.m[0][1] = forward.y;
    rotation.m[0][2] = forward.z;
    rotation.m[1][0] = cross.x;
    rotation.m[1][1] = cross.y;
    rotation.m[1][2] = cross.z;
    rotation.m[2][0] = side.x;
    rotation.m[2][1] = side.y;
    rotation.m[2][2] = side.z;
}

extern "C" void func_80021A74(Pademelon* self, Vec3f* dir, Vec3f* up) {
    Vec3f forward;
    Vec3f side;
    Vec3f cross;
    Vec3f proj;

    vec3_normalize(dir, &forward);
    Vec3f* fwd = &forward;

    f32 dot = up->x * fwd->x + up->y * fwd->y + up->z * fwd->z;
    proj.x = fwd->x * dot;
    proj.y = fwd->y * dot;
    proj.z = fwd->z * dot;
    side.x = up->x - proj.x;
    side.y = up->y - proj.y;
    side.z = up->z - proj.z;
    vec3_normalize(&side, &side);

    cross.x = fwd->y * side.z - fwd->z * side.y;
    cross.y = fwd->z * side.x - fwd->x * side.z;
    cross.z = fwd->x * side.y - fwd->y * side.x;

    self->rotation.m[0][0] = forward.x;
    self->rotation.m[0][1] = forward.y;
    self->rotation.m[0][2] = forward.z;
    self->rotation.m[1][0] = side.x;
    self->rotation.m[1][1] = side.y;
    self->rotation.m[1][2] = side.z;
    self->rotation.m[2][0] = cross.x;
    self->rotation.m[2][1] = cross.y;
    self->rotation.m[2][2] = cross.z;
}

extern "C" void func_80021BC8(Pademelon* self, Vec3f* dir, Vec3f* up) {
    Vec3f forward;
    Vec3f cross;
    Vec3f upNorm;
    Vec3f proj;
    Vec3f* upPtr = up;

    vec3_normalize(dir, &upNorm);

    f32 dot = upNorm.x * upPtr->x + upNorm.y * upPtr->y + upNorm.z * upPtr->z;
    proj.x = upNorm.x * dot;
    proj.y = upNorm.y * dot;
    proj.z = upNorm.z * dot;
    forward.x = upPtr->x - proj.x;
    Vec3f* fwd = &forward;
    fwd->y = upPtr->y - proj.y;
    fwd->z = upPtr->z - proj.z;
    vec3_normalize(fwd, fwd);

    cross.x = upNorm.y * fwd->z - upNorm.z * fwd->y;
    cross.y = upNorm.z * forward.x - upNorm.x * fwd->z;
    cross.z = upNorm.x * fwd->y - upNorm.y * forward.x;

    self->rotation.m[0][0] = forward.x;
    self->rotation.m[0][1] = forward.y;
    self->rotation.m[0][2] = forward.z;
    self->rotation.m[1][0] = cross.x;
    self->rotation.m[1][1] = cross.y;
    self->rotation.m[1][2] = cross.z;
    self->rotation.m[2][0] = upNorm.x;
    self->rotation.m[2][1] = upNorm.y;
    self->rotation.m[2][2] = upNorm.z;
}

// vfunc13 - inverse transform: out = R^T * (in - translation)
void Pademelon::vfunc13(Vec3f* in, Vec3f* out) {
    out->x = in->x * rotation.m[0][0];
    out->y = in->x * rotation.m[1][0];
    out->z = in->x * rotation.m[2][0];
    out->x += in->y * rotation.m[0][1];
    out->y += in->y * rotation.m[1][1];
    out->z += in->y * rotation.m[2][1];
    out->x += in->z * rotation.m[0][2];
    out->y += in->z * rotation.m[1][2];
    out->z += in->z * rotation.m[2][2];
    out->x -= rotation.m[0][0] * translation.x + rotation.m[0][1] * translation.y + rotation.m[0][2] * translation.z;
    out->y -= rotation.m[1][0] * translation.x + rotation.m[1][1] * translation.y + rotation.m[1][2] * translation.z;
    out->z -= rotation.m[2][0] * translation.x + rotation.m[2][1] * translation.y + rotation.m[2][2] * translation.z;
}

extern "C" void func_80021E88(Pademelon* self, Mat3f* arg1, f32 (*out)[3]) {
    Mat3f tmp;

    tmp.m[0][0] = self->rotation.m[0][0] * arg1->m[0][0] + self->rotation.m[1][0] * arg1->m[1][0] + self->rotation.m[2][0] * arg1->m[2][0];
    tmp.m[0][1] = self->rotation.m[0][0] * arg1->m[0][1] + self->rotation.m[1][0] * arg1->m[1][1] + self->rotation.m[2][0] * arg1->m[2][1];
    tmp.m[0][2] = self->rotation.m[0][0] * arg1->m[0][2] + self->rotation.m[1][0] * arg1->m[1][2] + self->rotation.m[2][0] * arg1->m[2][2];
    tmp.m[1][0] = self->rotation.m[0][1] * arg1->m[0][0] + self->rotation.m[1][1] * arg1->m[1][0] + self->rotation.m[2][1] * arg1->m[2][0];
    tmp.m[1][1] = self->rotation.m[0][1] * arg1->m[0][1] + self->rotation.m[1][1] * arg1->m[1][1] + self->rotation.m[2][1] * arg1->m[2][1];
    tmp.m[1][2] = self->rotation.m[0][1] * arg1->m[0][2] + self->rotation.m[1][1] * arg1->m[1][2] + self->rotation.m[2][1] * arg1->m[2][2];
    tmp.m[2][0] = self->rotation.m[0][2] * arg1->m[0][0] + self->rotation.m[1][2] * arg1->m[1][0] + self->rotation.m[2][2] * arg1->m[2][0];
    tmp.m[2][1] = self->rotation.m[0][2] * arg1->m[0][1] + self->rotation.m[1][2] * arg1->m[1][1] + self->rotation.m[2][2] * arg1->m[2][1];
    tmp.m[2][2] = self->rotation.m[0][2] * arg1->m[0][2] + self->rotation.m[1][2] * arg1->m[1][2] + self->rotation.m[2][2] * arg1->m[2][2];
    mat3_mul(tmp.m, self->rotation.m, out);
}

// Mirror Y axis of rotation and negate translation.y, then call vfunc16 on self
extern "C" void func_8002205C(Pademelon* self) {
    Vec3f row0;
    Vec3f row2;

    row0.x = self->rotation.m[0][0];
    row0.y = -self->rotation.m[0][1];
    row0.z = self->rotation.m[0][2];
    row2.x = self->rotation.m[2][0];
    row2.y = -self->rotation.m[2][1];
    row2.z = self->rotation.m[2][2];
    self->translation.y = -self->translation.y;

    self->vfunc16(&row0, &row2);
}

extern "C" void func_800220D0(Pademelon* self, Mat4f* out, f32 scale) {
    out->m[0][0] = self->rotation.m[0][0] * scale;
    out->m[0][1] = self->rotation.m[0][1] * scale;
    out->m[0][2] = self->rotation.m[0][2] * scale;
    out->m[1][0] = self->rotation.m[1][0] * scale;
    out->m[1][1] = self->rotation.m[1][1] * scale;
    out->m[1][2] = self->rotation.m[1][2] * scale;
    out->m[2][0] = self->rotation.m[2][0] * scale;
    out->m[2][1] = self->rotation.m[2][1] * scale;
    out->m[2][2] = self->rotation.m[2][2] * scale;
}

extern "C" void func_80022144(Dingo* self, f32 halfExt) {
    self->halfExtent = halfExt;
    self->minBound = self->pos.x - halfExt;
    self->maxBound = self->pos.x + halfExt;
}

extern "C" void func_80022168(Dingo* self, Vec3f* v) {
    self->pos = *v;
    self->minBound = self->pos.x - self->halfExtent;
    self->maxBound = self->pos.x + self->halfExtent;
}

INCLUDE_RODATA("asm/nonmatchings/padmelon", _vt.9Pademelon);

// vfunc3
void Pademelon::vfunc3(Vec3f* v) {
    translation.x = v->x;
    translation.y = v->y;
    translation.z = v->z;
    halfExtent = -1.0f;
}

#ifdef NON_MATCHING
// TODO: pending rodata reorganisation
void Pademelon::vfunc2(Vec3f* out) {
    out->x = translation.x;
    out->y = translation.y;
    out->z = translation.z;
}
#else
INCLUDE_ASM("asm/nonmatchings/padmelon", vfunc2__9PademelonP5Vec3f);
#endif

// vfunc12 - transform point: out = rotation * in + translation
void Pademelon::vfunc12(Vec3f* in, Vec3f* out) {
    out->x = in->x * rotation.m[0][0];
    out->y = in->x * rotation.m[0][1];
    out->z = in->x * rotation.m[0][2];
    out->x += in->y * rotation.m[1][0];
    out->y += in->y * rotation.m[1][1];
    out->z += in->y * rotation.m[1][2];
    out->x += in->z * rotation.m[2][0];
    out->y += in->z * rotation.m[2][1];
    out->z += in->z * rotation.m[2][2];
    out->x += translation.x;
    out->y += translation.y;
    out->z += translation.z;
}

// vfunc15 - transform with transposed rotation (no translation)
void Pademelon::vfunc15(Vec3f* in, Vec3f* out) {
    out->x = in->x * rotation.m[0][0];
    out->y = in->x * rotation.m[1][0];
    out->z = in->x * rotation.m[2][0];
    out->x += in->y * rotation.m[0][1];
    out->y += in->y * rotation.m[1][1];
    out->z += in->y * rotation.m[2][1];
    out->x += in->z * rotation.m[0][2];
    out->y += in->z * rotation.m[1][2];
    out->z += in->z * rotation.m[2][2];
}

// vfunc14 - transform direction (no translation)
void Pademelon::vfunc14(Vec3f* in, Vec3f* out) {
    out->x = in->x * rotation.m[0][0];
    out->y = in->x * rotation.m[0][1];
    out->z = in->x * rotation.m[0][2];
    out->x += in->y * rotation.m[1][0];
    out->y += in->y * rotation.m[1][1];
    out->z += in->y * rotation.m[1][2];
    out->x += in->z * rotation.m[2][0];
    out->y += in->z * rotation.m[2][1];
    out->z += in->z * rotation.m[2][2];
}

// vfunc19
void Pademelon::vfunc19(Mat3f* mat) {
    rotation.m[0][0] = mat->m[0][0];
    rotation.m[0][1] = mat->m[0][1];
    rotation.m[0][2] = mat->m[0][2];
    rotation.m[1][0] = mat->m[1][0];
    rotation.m[1][1] = mat->m[1][1];
    rotation.m[1][2] = mat->m[1][2];
    rotation.m[2][0] = mat->m[2][0];
    rotation.m[2][1] = mat->m[2][1];
    rotation.m[2][2] = mat->m[2][2];
}

// vfunc18
void Pademelon::vfunc18(Mat3f* out) {
    out->m[0][0] = rotation.m[0][0];
    out->m[0][1] = rotation.m[0][1];
    out->m[0][2] = rotation.m[0][2];
    out->m[1][0] = rotation.m[1][0];
    out->m[1][1] = rotation.m[1][1];
    out->m[1][2] = rotation.m[1][2];
    out->m[2][0] = rotation.m[2][0];
    out->m[2][1] = rotation.m[2][1];
    out->m[2][2] = rotation.m[2][2];
}

// Gets rotation into a 4x4 matrix (first 3 cols of each row)
extern "C" void func_800224FC(Pademelon* self, Mat4f* out) {
    out->m[0][0] = self->rotation.m[0][0];
    out->m[0][1] = self->rotation.m[0][1];
    out->m[0][2] = self->rotation.m[0][2];
    out->m[1][0] = self->rotation.m[1][0];
    out->m[1][1] = self->rotation.m[1][1];
    out->m[1][2] = self->rotation.m[1][2];
    out->m[2][0] = self->rotation.m[2][0];
    out->m[2][1] = self->rotation.m[2][1];
    out->m[2][2] = self->rotation.m[2][2];
}

// Resets to identity transform
extern "C" void func_80022548(Pademelon* self) {
    self->vel.x = 0;
    self->vel.y = 0;
    self->vel.z = 0;
    self->rotation.m[0][1] = 0;
    self->rotation.m[0][2] = 0;
    self->rotation.m[1][0] = 0;
    self->rotation.m[1][2] = 0;
    self->rotation.m[2][0] = 0;
    self->rotation.m[2][1] = 0;
    self->translation.x = 0;
    self->translation.y = 0;
    self->translation.z = 0;
    self->rotation.m[0][0] = 1.0f;
    self->rotation.m[1][1] = 1.0f;
    self->rotation.m[2][2] = 1.0f;
}

Pademelon::Pademelon() {
    func_80022548(this);
}

extern "C" void func_800225CC(Pademelon* self, Vec3f* row0, Vec3f* row1, Vec3f* row2) {
    row0->x = self->rotation.m[0][0];
    row0->y = self->rotation.m[0][1];
    row0->z = self->rotation.m[0][2];
    row1->x = self->rotation.m[1][0];
    row1->y = self->rotation.m[1][1];
    row1->z = self->rotation.m[1][2];
    row2->x = self->rotation.m[2][0];
    row2->y = self->rotation.m[2][1];
    row2->z = self->rotation.m[2][2];
}

// vfunc17
void Pademelon::vfunc17(Vec3f* a, Vec3f* b) {
    a->x = rotation.m[0][0];
    a->y = rotation.m[0][1];
    a->z = rotation.m[0][2];
    b->x = rotation.m[2][0];
    b->y = rotation.m[2][1];
    b->z = rotation.m[2][2];
}

extern "C" void func_8002264C(Pademelon* self, f32* quat) {
    quat_to_mat3(quat, (f32*)&self->rotation);
}

// Exports as column-major 4x3 (transposed rotation + translation)
extern "C" void func_80022670(Pademelon* self, f32* out) {
    out[0] = self->rotation.m[0][0];
    out[3] = self->rotation.m[1][0];
    out[6] = self->rotation.m[2][0];
    out[1] = self->rotation.m[0][1];
    out[4] = self->rotation.m[1][1];
    out[7] = self->rotation.m[2][1];
    out[2] = self->rotation.m[0][2];
    out[5] = self->rotation.m[1][2];
    out[8] = self->rotation.m[2][2];
    out[9] = self->translation.x;
    out[10] = self->translation.y;
    out[11] = self->translation.z;
}

extern "C" void func_800226D4(Pademelon* self, f32* quat) {
    mat3_to_quat(self->rotation.m, quat);
}

extern "C" void func_800226F0(Pademelon* self, Vec3f* out) {
    out->x = self->rotation.m[2][0];
    out->y = self->rotation.m[2][1];
    out->z = self->rotation.m[2][2];
}

extern "C" void func_8002270C(Pademelon* self, Vec3f* out) {
    out->x = self->rotation.m[1][0];
    out->y = self->rotation.m[1][1];
    out->z = self->rotation.m[1][2];
}

extern "C" void func_80022728(Pademelon* self, Vec3f* out) {
    out->x = self->rotation.m[0][0];
    out->y = self->rotation.m[0][1];
    out->z = self->rotation.m[0][2];
}

// Stores translation into row 3 of a 4x4 matrix
extern "C" void func_80022744(Pademelon* self, Mat4f* out) {
    out->m[3][0] = self->translation.x;
    out->m[3][1] = self->translation.y;
    out->m[3][2] = self->translation.z;
}

extern "C" void func_80022760(Pademelon* self, Pademelon* other) {
    self->rotation.m[0][0] = other->rotation.m[0][0];
    self->rotation.m[0][1] = other->rotation.m[0][1];
    self->rotation.m[0][2] = other->rotation.m[0][2];
    self->rotation.m[1][0] = other->rotation.m[1][0];
    self->rotation.m[1][1] = other->rotation.m[1][1];
    self->rotation.m[1][2] = other->rotation.m[1][2];
    self->rotation.m[2][0] = other->rotation.m[2][0];
    self->rotation.m[2][1] = other->rotation.m[2][1];
    self->rotation.m[2][2] = other->rotation.m[2][2];
    self->translation = other->translation;
    self->halfExtent = -1.0f;
}

extern "C" void func_800227D0(Pademelon* self, Pademelon* other) {
    self->rotation.m[0][0] = other->rotation.m[0][0];
    self->rotation.m[0][1] = other->rotation.m[0][1];
    self->rotation.m[0][2] = other->rotation.m[0][2];
    self->rotation.m[1][0] = other->rotation.m[1][0];
    self->rotation.m[1][1] = other->rotation.m[1][1];
    self->rotation.m[1][2] = other->rotation.m[1][2];
    self->rotation.m[2][0] = other->rotation.m[2][0];
    self->rotation.m[2][1] = other->rotation.m[2][1];
    self->rotation.m[2][2] = other->rotation.m[2][2];
}

extern "C" void func_8002281C(Pademelon* self, Pademelon* other) {
    self->translation = other->translation;
    self->halfExtent = -1.0f;
}

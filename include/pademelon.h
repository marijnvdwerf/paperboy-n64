#ifndef PADEMELON_H
#define PADEMELON_H

#include "common.h"
#include "dingo.h"
#include "vector.h"

struct Pademelon : public Dingo {
    /* 0x34 */ Mat3f rotation;
    /* 0x58 */ Vec3f translation;

    Pademelon();
    void vfunc16(Vec3f* dir, Vec3f* up) CXX_OVERRIDE;
    void vfunc13(Vec3f* in, Vec3f* out) CXX_OVERRIDE;
    void vfunc3(Vec3f* v) CXX_OVERRIDE;
    void vfunc2(Vec3f* out) CXX_OVERRIDE;
    void vfunc12(Vec3f* in, Vec3f* out) CXX_OVERRIDE;
    void vfunc15(Vec3f* in, Vec3f* out) CXX_OVERRIDE;
    void vfunc14(Vec3f* in, Vec3f* out) CXX_OVERRIDE;
    void vfunc19(Mat3f* mat) CXX_OVERRIDE;
    void vfunc18(Mat3f* out) CXX_OVERRIDE;
    void func_80021A74(Vec3f* dir, Vec3f* up);
    void func_80021BC8(Vec3f* dir, Vec3f* up);
    void func_80021E88(Mat3f* arg1, f32 (*out)[3]);
    void func_8002205C();
    void func_800220D0(Mat4f* out, f32 scale);
    void func_80022144(f32 halfExt);
    void func_80022168(Vec3f* v);
    void func_800224FC(Mat4f* out);
    void func_80022548();

    void func_8002281C(Pademelon* other) {
        translation = other->translation;
        halfExtent = -1.0f;
    }

    void func_800227D0(Pademelon* other) {
        rotation.m[0][0] = other->rotation.m[0][0];
        rotation.m[0][1] = other->rotation.m[0][1];
        rotation.m[0][2] = other->rotation.m[0][2];
        rotation.m[1][0] = other->rotation.m[1][0];
        rotation.m[1][1] = other->rotation.m[1][1];
        rotation.m[1][2] = other->rotation.m[1][2];
        rotation.m[2][0] = other->rotation.m[2][0];
        rotation.m[2][1] = other->rotation.m[2][1];
        rotation.m[2][2] = other->rotation.m[2][2];
    }

    void func_80022760(Pademelon* other) {
        rotation.m[0][0] = other->rotation.m[0][0];
        rotation.m[0][1] = other->rotation.m[0][1];
        rotation.m[0][2] = other->rotation.m[0][2];
        rotation.m[1][0] = other->rotation.m[1][0];
        rotation.m[1][1] = other->rotation.m[1][1];
        rotation.m[1][2] = other->rotation.m[1][2];
        rotation.m[2][0] = other->rotation.m[2][0];
        rotation.m[2][1] = other->rotation.m[2][1];
        rotation.m[2][2] = other->rotation.m[2][2];
        translation = other->translation;
        halfExtent = -1.0f;
    }

    void func_80022744(Mat4f* out) {
        out->m[3][0] = translation.x;
        out->m[3][1] = translation.y;
        out->m[3][2] = translation.z;
    }

    void func_80022728(Vec3f* out) {
        out->x = rotation.m[0][0];
        out->y = rotation.m[0][1];
        out->z = rotation.m[0][2];
    }

    void func_8002270C(Vec3f* out) {
        out->x = rotation.m[1][0];
        out->y = rotation.m[1][1];
        out->z = rotation.m[1][2];
    }

    void func_800226F0(Vec3f* out) {
        out->x = rotation.m[2][0];
        out->y = rotation.m[2][1];
        out->z = rotation.m[2][2];
    }

    void func_800226D4(f32* quat) {
        mat3_to_quat(rotation.m, quat);
    }

    void func_80022670(f32* out) {
        out[0] = rotation.m[0][0];
        out[3] = rotation.m[1][0];
        out[6] = rotation.m[2][0];
        out[1] = rotation.m[0][1];
        out[4] = rotation.m[1][1];
        out[7] = rotation.m[2][1];
        out[2] = rotation.m[0][2];
        out[5] = rotation.m[1][2];
        out[8] = rotation.m[2][2];
        out[9] = translation.x;
        out[10] = translation.y;
        out[11] = translation.z;
    }

    void func_8002264C(f32* quat) {
        quat_to_mat3(quat, (f32*)&rotation);
    }

    void vfunc17(Vec3f* a, Vec3f* b) CXX_OVERRIDE {
        a->x = rotation.m[0][0];
        a->y = rotation.m[0][1];
        a->z = rotation.m[0][2];
        b->x = rotation.m[2][0];
        b->y = rotation.m[2][1];
        b->z = rotation.m[2][2];
    }

    void func_800225CC(Vec3f* row0, Vec3f* row1, Vec3f* row2) {
        row0->x = rotation.m[0][0];
        row0->y = rotation.m[0][1];
        row0->z = rotation.m[0][2];
        row1->x = rotation.m[1][0];
        row1->y = rotation.m[1][1];
        row1->z = rotation.m[1][2];
        row2->x = rotation.m[2][0];
        row2->y = rotation.m[2][1];
        row2->z = rotation.m[2][2];
    }
};

#endif

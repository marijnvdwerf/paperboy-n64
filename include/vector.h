#ifndef VECTOR_H
#define VECTOR_H

#include "common.h"

struct Vec2f {
    f32 x;
    f32 y;
};

struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
};

struct Mat3f {
    f32 m[3][3];
};

struct Mat4f {
    f32 m[4][4];
};

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

void vec3_normalize(Vec3f* in, Vec3f* out);
void mat3_mul(f32 (*a)[3], f32 (*b)[3], f32 (*out)[3]);
void mat3_to_quat(f32 (*matrix)[3], f32* quaternion);
void quat_to_mat3(f32* quaternion, f32* matrix);

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif

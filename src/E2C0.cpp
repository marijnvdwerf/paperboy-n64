#include "common.h"

struct Vector2 {
    f32 x;
    f32 y;
};

struct Vector3 {
    f32 x;
    f32 y;
    f32 z;
};

extern "C" f32 sqrtf(f32);
extern "C" f32 __sinf(f32);
extern "C" f32 __cosf(f32);
extern "C" void* memcpy(void*, const void*, u32);

extern "C" f32 D_8006C5F0[0x400];

extern "C" void mat4_mul(f32 (*a)[4], f32 (*b)[4], f32 (*out)[4]) {
    out[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]) + (a[0][3] * b[3][0]);
    out[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]) + (a[0][3] * b[3][1]);
    out[0][2] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]) + (a[0][3] * b[3][2]);
    out[0][3] = (a[0][0] * b[0][3]) + (a[0][1] * b[1][3]) + (a[0][2] * b[2][3]) + (a[0][3] * b[3][3]);
    out[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]) + (a[1][3] * b[3][0]);
    out[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]) + (a[1][3] * b[3][1]);
    out[1][2] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]) + (a[1][3] * b[3][2]);
    out[1][3] = (a[1][0] * b[0][3]) + (a[1][1] * b[1][3]) + (a[1][2] * b[2][3]) + (a[1][3] * b[3][3]);
    out[2][0] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]) + (a[2][3] * b[3][0]);
    out[2][1] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]) + (a[2][3] * b[3][1]);
    out[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]) + (a[2][3] * b[3][2]);
    out[2][3] = (a[2][0] * b[0][3]) + (a[2][1] * b[1][3]) + (a[2][2] * b[2][3]) + (a[2][3] * b[3][3]);
    out[3][0] = (a[3][0] * b[0][0]) + (a[3][1] * b[1][0]) + (a[3][2] * b[2][0]) + (a[3][3] * b[3][0]);
    out[3][1] = (a[3][0] * b[0][1]) + (a[3][1] * b[1][1]) + (a[3][2] * b[2][1]) + (a[3][3] * b[3][1]);
    out[3][2] = (a[3][0] * b[0][2]) + (a[3][1] * b[1][2]) + (a[3][2] * b[2][2]) + (a[3][3] * b[3][2]);
    out[3][3] = (a[3][0] * b[0][3]) + (a[3][1] * b[1][3]) + (a[3][2] * b[2][3]) + (a[3][3] * b[3][3]);
}

extern "C" void mat4_mul_affine(f32 (*a)[4], f32 (*b)[4], f32 (*out)[4]) {
    out[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    out[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    out[0][2] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);

    out[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    out[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    out[1][2] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);

    out[2][0] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);
    out[2][1] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);
    out[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]);

    out[3][0] = (a[3][0] * b[0][0]) + (a[3][1] * b[1][0]) + (a[3][2] * b[2][0]) + b[3][0];
    out[3][1] = (a[3][0] * b[0][1]) + (a[3][1] * b[1][1]) + (a[3][2] * b[2][1]) + b[3][1];
    out[3][2] = (a[3][0] * b[0][2]) + (a[3][1] * b[1][2]) + (a[3][2] * b[2][2]) + b[3][2];
}

extern "C" void mat4x3_mul_affine(f32 (*a)[3], f32 (*b)[3], f32 (*out)[3]) {
    out[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    out[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    out[0][2] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);

    out[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    out[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    out[1][2] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);

    out[2][0] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);
    out[2][1] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);
    out[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]);

    out[3][0] = (a[3][0] * b[0][0]) + (a[3][1] * b[1][0]) + (a[3][2] * b[2][0]) + b[3][0];
    out[3][1] = (a[3][0] * b[0][1]) + (a[3][1] * b[1][1]) + (a[3][2] * b[2][1]) + b[3][1];
    out[3][2] = (a[3][0] * b[0][2]) + (a[3][1] * b[1][2]) + (a[3][2] * b[2][2]) + b[3][2];
}

extern "C" void mat3_mul(f32 (*a)[3], f32 (*b)[3], f32 (*out)[3]) {
    out[0][0] = (a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]);
    out[0][1] = (a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]);
    out[0][2] = (a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]);

    out[1][0] = (a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]);
    out[1][1] = (a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]);
    out[1][2] = (a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2]);

    out[2][0] = (a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]);
    out[2][1] = (a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]);
    out[2][2] = (a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2]);
}

extern "C" void mat3_to_quat(f32 (*matrix)[3], f32* quaternion) {
    f32 root;
    f32 trace;
    f32 m22;
    f32 m11;
    f32 m00;
    s32 biggestIndex;

    m00 = matrix[0][0];
    m11 = matrix[1][1];
    m22 = matrix[2][2];

    trace = m00 + m11 + m22;
    if (trace > 0.0f) {
        root = sqrtf(trace + 1.0f);
        quaternion[3] = root * 0.5f;
        root = 0.5f / root;
        quaternion[0] = (matrix[2][1] - matrix[1][2]) * root;
        quaternion[1] = (matrix[0][2] - matrix[2][0]) * root;
        quaternion[2] = (matrix[1][0] - matrix[0][1]) * root;
        return;
    }

    biggestIndex = 0;
    if (matrix[0][0] < matrix[1][1]) {
        biggestIndex = 1;
    }
    if (matrix[biggestIndex][biggestIndex] < m22) {
        biggestIndex = 2;
    }

    switch (biggestIndex) {
        case 0:
            root = sqrtf((m00 - (m11 + m22)) + 1.0f);
            quaternion[0] = root * 0.5f;
            root = 0.5f / root;
            quaternion[3] = (matrix[2][1] - matrix[1][2]) * root;
            quaternion[1] = (matrix[1][0] + matrix[0][1]) * root;
            quaternion[2] = (matrix[2][0] + matrix[0][2]) * root;
            return;
        case 1:
            root = sqrtf((m11 - (m22 + m00)) + 1.0f);
            quaternion[1] = root * 0.5f;
            root = 0.5f / root;
            quaternion[3] = (matrix[0][2] - matrix[2][0]) * root;
            quaternion[2] = (matrix[2][1] + matrix[1][2]) * root;
            quaternion[0] = (matrix[0][1] + matrix[1][0]) * root;
            return;
        case 2:
            root = sqrtf((m22 - (m00 + m11)) + 1.0f);
            quaternion[2] = root * 0.5f;
            root = 0.5f / root;
            quaternion[3] = (matrix[1][0] - matrix[0][1]) * root;
            quaternion[0] = (matrix[0][2] + matrix[2][0]) * root;
            quaternion[1] = (matrix[1][2] + matrix[2][1]) * root;
            return;
    }
}

extern "C" void mat4_to_quat(f32 (*matrix)[4], f32* quaternion) {
    f32 root;
    f32 trace;
    f32 m22;
    f32 m11;
    f32 m00;
    s32 biggestIndex;

    m00 = matrix[0][0];
    m11 = matrix[1][1];
    m22 = matrix[2][2];

    trace = m00 + m11 + m22;
    if (trace > 0.0f) {
        root = sqrtf(trace + 1.0f);
        quaternion[3] = root * 0.5f;
        root = 0.5f / root;
        quaternion[0] = (matrix[2][1] - matrix[1][2]) * root;
        quaternion[1] = (matrix[0][2] - matrix[2][0]) * root;
        quaternion[2] = (matrix[1][0] - matrix[0][1]) * root;
        return;
    }

    biggestIndex = 0;
    if (matrix[0][0] < matrix[1][1]) {
        biggestIndex = 1;
    }
    if (matrix[biggestIndex][biggestIndex] < m22) {
        biggestIndex = 2;
    }

    switch (biggestIndex) {
        case 0:
            root = sqrtf((m00 - (m11 + m22)) + 1.0f);
            quaternion[0] = root * 0.5f;
            root = 0.5f / root;
            quaternion[3] = (matrix[2][1] - matrix[1][2]) * root;
            quaternion[1] = (matrix[1][0] + matrix[0][1]) * root;
            quaternion[2] = (matrix[2][0] + matrix[0][2]) * root;
            return;
        case 1:
            root = sqrtf((m11 - (m22 + m00)) + 1.0f);
            quaternion[1] = root * 0.5f;
            root = 0.5f / root;
            quaternion[3] = (matrix[0][2] - matrix[2][0]) * root;
            quaternion[2] = (matrix[2][1] + matrix[1][2]) * root;
            quaternion[0] = (matrix[0][1] + matrix[1][0]) * root;
            return;
        case 2:
            root = sqrtf((m22 - (m00 + m11)) + 1.0f);
            quaternion[2] = root * 0.5f;
            root = 0.5f / root;
            quaternion[3] = (matrix[1][0] - matrix[0][1]) * root;
            quaternion[0] = (matrix[0][2] + matrix[2][0]) * root;
            quaternion[1] = (matrix[1][2] + matrix[2][1]) * root;
            return;
    }
}

extern "C" void quat_slerp(f32* arg0, f32* arg1, f32 arg2, f32* arg3) {
    f32 temp_f20;
    f32 temp_f22;
    f32 var_f12;
    f32 var_f20;
    f32 var_f4;
    s32 temp_f14;

    f32 temp[4];
    var_f4 = (arg0[0] * arg1[0]) + (arg0[1] * arg1[1]) + (arg0[2] * arg1[2]) + (arg0[3] * arg1[3]);
    if (var_f4 < 0.0f) {
        var_f4 = -var_f4;
        temp[0] = -arg1[0];
        temp[1] = -arg1[1];
        temp[2] = -arg1[2];
        temp[3] = -arg1[3];
    } else {
        memcpy(temp, arg1, sizeof(temp));
    }
    if ((1.0f - var_f4) > 0.004f) {
        temp_f14 = (s32)((var_f4 + 1.0f) * 511.5f);
        temp_f20 = D_8006C5F0[temp_f14 & 0x3FF];
        temp_f22 = 1.0f / __sinf(temp_f20);
        var_f20 = __sinf((1.0f - arg2) * temp_f20) * temp_f22;
        var_f12 = __sinf(arg2 * temp_f20) * temp_f22;
    } else {
        var_f12 = arg2;
        var_f20 = 1.0f - var_f12;
    }
    arg3[0] = (var_f20 * arg0[0]) + (var_f12 * arg1[0]);
    arg3[1] = (var_f20 * arg0[1]) + (var_f12 * arg1[1]);
    arg3[2] = (var_f20 * arg0[2]) + (var_f12 * arg1[2]);
    arg3[3] = (var_f20 * arg0[3]) + (var_f12 * arg1[3]);
}

// TODO: implement
INCLUDE_ASM("asm/nonmatchings/E2C0", vec3_rotate_axis_angle);

// TODO: implement
INCLUDE_ASM("asm/nonmatchings/E2C0", tri_contains_point);

// TODO: implement
INCLUDE_ASM("asm/nonmatchings/E2C0", vec3_move_toward);

extern "C" void mat3_mul_flat(f32* arg0, f32* arg1, f32* arg2) {
    arg2[0] = (arg0[0] * arg1[0]) + (arg0[1] * arg1[1]) + (arg0[2] * arg1[2]);
    arg2[1] = (arg0[0] * arg1[3]) + (arg0[1] * arg1[4]) + (arg0[2] * arg1[5]);
    arg2[2] = (arg0[0] * arg1[6]) + (arg0[1] * arg1[7]) + (arg0[2] * arg1[8]);
    arg2[3] = (arg0[3] * arg1[0]) + (arg0[4] * arg1[1]) + (arg0[5] * arg1[2]);
    arg2[4] = (arg0[3] * arg1[3]) + (arg0[4] * arg1[4]) + (arg0[5] * arg1[5]);
    arg2[5] = (arg0[3] * arg1[6]) + (arg0[4] * arg1[7]) + (arg0[5] * arg1[8]);
    arg2[6] = (arg0[6] * arg1[0]) + (arg0[7] * arg1[1]) + (arg0[8] * arg1[2]);
    arg2[7] = (arg0[6] * arg1[3]) + (arg0[7] * arg1[4]) + (arg0[8] * arg1[5]);
    arg2[8] = (arg0[6] * arg1[6]) + (arg0[7] * arg1[7]) + (arg0[8] * arg1[8]);
}

extern "C" void vec3_reflect_neg_w(f32* arg0, f32* arg1, f32* arg2) {
    f32 sp[3];
    f32 temp_fv1 = 2.0f * ((arg0[0] * arg1[0]) + (arg0[1] * arg1[1]) + (arg0[2] * arg1[2]));
    sp[0] = arg0[0] * temp_fv1;
    sp[1] = arg0[1] * temp_fv1;
    sp[2] = arg0[2] * temp_fv1;
    arg2[0] = arg1[0] - sp[0];
    arg2[1] = arg1[1] - sp[1];
    arg2[2] = arg1[2] - sp[2];
    arg2[3] = -arg1[3];
}

extern "C" void vec3_reflect(f32* arg0, f32* arg1, f32* arg2) {
    f32 sp[3];
    f32 temp_fv1 = 2.0f * ((arg0[0] * arg1[0]) + (arg0[1] * arg1[1]) + (arg0[2] * arg1[2]));
    sp[0] = arg0[0] * temp_fv1;
    sp[1] = arg0[1] * temp_fv1;
    sp[2] = arg0[2] * temp_fv1;
    arg2[0] = arg1[0] - sp[0];
    arg2[1] = arg1[1] - sp[1];
    arg2[2] = arg1[2] - sp[2];
}

extern "C" void vec3_reflect_yzswap_neg_w(Vector3* arg0, Vector3* arg1, Vector3* arg2) {
    f32 temp_f2 = 2.0f * ((arg1->x * arg0->x) + (arg1->y * arg0->z) + (arg1->z * arg0->y));
    arg2->x = arg1->x - (arg0->x * temp_f2);
    arg2->y = arg1->y - (arg0->z * temp_f2);
    arg2->z = arg1->z - (arg0->y * temp_f2);
    ((f32*)arg2)[3] = -((f32*)arg1)[3];
}

extern "C" void vec3_reflect_yzswap(Vector3* arg0, Vector3* arg1, Vector3* arg2) {
    f32 temp_f2 = 2.0f * ((arg1->x * arg0->x) + (arg1->y * arg0->z) + (arg1->z * arg0->y));
    arg2->x = arg1->x - (arg0->x * temp_f2);
    arg2->y = arg1->y - (arg0->z * temp_f2);
    arg2->z = arg1->z - (arg0->y * temp_f2);
}

extern "C" void plane_reflect_point(f32* arg0, f32* arg1, f32* arg2) {
    f32 sp[3];
    f32 temp_fv1 = 2.0f * ((arg0[0] * arg1[0]) + (arg0[1] * arg1[1]) + (arg0[2] * arg1[2]) + arg0[3]);
    sp[0] = arg0[0] * temp_fv1;
    sp[1] = arg0[1] * temp_fv1;
    sp[2] = arg0[2] * temp_fv1;
    arg2[0] = arg1[0] - sp[0];
    arg2[1] = arg1[1] - sp[1];
    arg2[2] = arg1[2] - sp[2];
}

extern "C" void vec3_mul_mat3(Vector3 v, f32* m, f32* out) {
    out[0] = (v.x * m[0]) + (v.y * m[1]) + (v.z * m[2]);
    out[1] = (v.x * m[3]) + (v.y * m[4]) + (v.z * m[5]);
    out[2] = (v.x * m[6]) + (v.y * m[7]) + (v.z * m[8]);
}

// TODO: implement
INCLUDE_ASM("asm/nonmatchings/E2C0", atanf);

extern "C" void quat_nlerp(f32* arg0, f32* arg1, f32 arg2, f32* arg3) {
    f32 dotProduct = (arg0[0] * arg1[0]) + (arg0[1] * arg1[1]) + (arg0[2] * arg1[2]) + (arg0[3] * arg1[3]);

    if (dotProduct > 0.0f) {
        arg3[0] = arg0[0] + arg2 * (arg1[0] - arg0[0]);
        arg3[1] = arg0[1] + arg2 * (arg1[1] - arg0[1]);
        arg3[2] = arg0[2] + arg2 * (arg1[2] - arg0[2]);
        arg3[3] = arg0[3] + arg2 * (arg1[3] - arg0[3]);
    } else {
        arg3[0] = arg0[0] - arg2 * (arg1[0] + arg0[0]);
        arg3[1] = arg0[1] - arg2 * (arg1[1] + arg0[1]);
        arg3[2] = arg0[2] - arg2 * (arg1[2] + arg0[2]);
        arg3[3] = arg0[3] - arg2 * (arg1[3] + arg0[3]);
    }
}

extern "C" void quat_to_mat4(f32* quaternion, f32* matrix) {
    f32 q1 = quaternion[0];
    f32 q2 = quaternion[1];
    f32 q3 = quaternion[2];
    f32 q4 = quaternion[3];

    f32 normalizationFactor = 2.0f / ((q1 * q1) + (q2 * q2) + (q3 * q3) + (q4 * q4));

    f32 nq1 = q1 * normalizationFactor;
    f32 nq2 = q2 * normalizationFactor;
    f32 nq3 = q3 * normalizationFactor;

    f32 q4nq1 = q4 * nq1;
    f32 q4nq2 = q4 * nq2;
    f32 q4nq3 = q4 * nq3;
    f32 nq1q1 = q1 * nq1;
    f32 nq1q2 = q1 * nq2;
    f32 nq1q3 = q1 * nq3;
    f32 nq2q2 = q2 * nq2;
    f32 nq2q3 = q2 * nq3;
    f32 nq3q3 = q3 * nq3;

    matrix[0] = 1.0f - (nq2q2 + nq3q3);
    matrix[1] = nq1q2 - q4nq3;
    matrix[2] = nq1q3 + q4nq2;
    matrix[3] = 0;
    matrix[4] = nq1q2 + q4nq3;
    matrix[5] = 1.0f - (nq1q1 + nq3q3);
    matrix[6] = nq2q3 - q4nq1;
    matrix[7] = 0;
    matrix[8] = nq1q3 - q4nq2;
    matrix[9] = nq2q3 + q4nq1;
    matrix[10] = 1.0f - (nq1q1 + nq2q2);
    matrix[11] = 0;
    matrix[15] = 1.0f;
}

extern "C" void quat_to_mat3(f32* arg0, f32* arg1) {
    f32 temp_f6 = arg0[0];
    f32 temp_f10 = arg0[1];
    f32 temp_f8 = arg0[2];
    f32 temp_f4 = arg0[3];
    f32 temp_f0 = 2.0f / ((temp_f6 * temp_f6) + (temp_f10 * temp_f10) + (temp_f8 * temp_f8) + (temp_f4 * temp_f4));
    f32 temp_f2 = temp_f6 * temp_f0;
    f32 temp_f14 = temp_f10 * temp_f0;
    f32 temp_f0_2 = temp_f8 * temp_f0;
    f32 temp_f24 = temp_f4 * temp_f2;
    f32 temp_f22 = temp_f4 * temp_f14;
    f32 temp_f4_2 = temp_f4 * temp_f0_2;
    f32 temp_f2_2 = temp_f6 * temp_f2;
    f32 temp_f12 = temp_f6 * temp_f14;
    f32 temp_f6_2 = temp_f6 * temp_f0_2;
    f32 temp_f14_2 = temp_f10 * temp_f14;
    f32 temp_f8_2 = temp_f8 * temp_f0_2;
    f32 temp_f10_2 = temp_f10 * temp_f0_2;

    arg1[0] = 1.0f - (temp_f14_2 + temp_f8_2);
    arg1[1] = temp_f12 - temp_f4_2;
    arg1[2] = temp_f6_2 + temp_f22;
    arg1[3] = temp_f12 + temp_f4_2;
    arg1[4] = 1.0f - (temp_f2_2 + temp_f8_2);
    arg1[5] = temp_f10_2 - temp_f24;
    arg1[6] = temp_f6_2 - temp_f22;
    arg1[7] = temp_f10_2 + temp_f24;
    arg1[8] = 1.0f - (temp_f2_2 + temp_f14_2);
}

extern "C" void quat_to_axis_angle(f32* arg0, f32* arg1, f32* arg2) {
    f32 temp_f20;
    f32 temp_f22_2;
    f32 temp_f2 = arg0[3];
    f32 var_f4 = temp_f2;
    if (temp_f2 < 0.0f) {
        var_f4 = -temp_f2;
    }
    if (var_f4 < 0.99999f) {
        s32 new_var = (s32)((temp_f2 + 1.0f) * 511.5f);
        temp_f20 = D_8006C5F0[new_var & 0x3FF];
        temp_f22_2 = 1.0f / __sinf(temp_f20);
        arg1[0] = arg0[0] * temp_f22_2;
        arg1[1] = arg0[1] * temp_f22_2;
        arg1[2] = arg0[2] * temp_f22_2;
        *arg2 = 2.0f * temp_f20;
        return;
    }
    *arg2 = 0.0f;
    arg1[0] = 1.0f;
    arg1[1] = 0.0f;
    arg1[2] = 0.0f;
}

extern "C" void quat_from_axis_angle(f32* arg0, f32 arg1, f32* arg2) {
    arg1 = arg1 * 0.5f;
    f32 temp_f0 = __sinf(arg1);
    arg2[0] = arg0[0] * temp_f0;
    arg2[1] = arg0[1] * temp_f0;
    arg2[2] = arg0[2] * temp_f0;
    arg2[3] = __cosf(arg1);
}

// TODO: implement
INCLUDE_ASM("asm/nonmatchings/E2C0", mat4_from_axis_angle);

// TODO: implement
INCLUDE_ASM("asm/nonmatchings/E2C0", mat4x3_from_axis_angle);

extern "C" void vec3_normalize(Vector3* arg0, Vector3* arg1) {
    f32 length_sq;
    f32 z;
    f32 y;
    f32 x;
    f32 length;
    f32 z_sq;
    f32 y_sq;

    x = arg0->x;
    length_sq = x * x;
    y = arg0->y;
    y_sq = y * y;
    z = arg0->z;
    z_sq = z * z;
    length_sq = length_sq + y_sq;
    length_sq = length_sq + z_sq;
    length = sqrtf(length_sq);
    if ((f64)length == 0.0) {
        arg1->x = 0.0f;
        arg1->y = 0.0f;
        arg1->z = 0.0f;
        return;
    }
    length = 1.0f / length;
    arg1->x = x * length;
    arg1->y = y * length;
    arg1->z = z * length;
}

// TODO: implement
INCLUDE_ASM("asm/nonmatchings/E2C0", vec2_normalize);

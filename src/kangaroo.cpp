#include "common.h"
#include "kangaroo.h"

extern "C" {
extern void* D_80072BB0;
void mat4_mul(f32 (*a)[4], f32 (*b)[4], f32 (*out)[4]);
void mat3_to_quat(f32 (*matrix)[3], f32* quaternion);
void vec3_normalize(void* in, void* out);
}

struct Bowerbird {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u32 unkC;
    /* 0x10 */ u32 unk10;
    /* 0x14 */ s32* unk14;

    Bowerbird();
    ~Bowerbird();
};

// vfunc3
void Kangaroo::vfunc3(f32* src, f32* dst) {
    dst[0] = src[0] * this->unk10;
    dst[1] = src[0] * this->unk20;
    dst[2] = src[0] * this->unk30;
    dst[0] += src[1] * this->unk14;
    dst[1] += src[1] * this->unk24;
    dst[2] += src[1] * this->unk34;
    dst[0] += src[2] * this->unk18;
    dst[1] += src[2] * this->unk28;
    dst[2] += src[2] * this->unk38;
    dst[0] -= this->unk10 * this->unk40 + this->unk14 * this->unk44 + this->unk18 * this->unk48;
    dst[1] -= this->unk20 * this->unk40 + this->unk24 * this->unk44 + this->unk28 * this->unk48;
    dst[2] -= this->unk30 * this->unk40 + this->unk34 * this->unk44 + this->unk38 * this->unk48;
}

// vfunc19
void Kangaroo::vfunc19(Joey* arg) {
    Kangaroo* src = (Kangaroo*)arg;
    this->unk10 = src->unk10;
    this->unk14 = src->unk14;
    this->unk18 = src->unk18;
    this->unk20 = src->unk20;
    this->unk24 = src->unk24;
    this->unk28 = src->unk28;
    this->unk30 = src->unk30;
    this->unk34 = src->unk34;
    this->unk38 = src->unk38;
    this->unk40 = src->unk40;
    this->unk44 = src->unk44;
    this->unk48 = src->unk48;
}

// vfunc6
void Kangaroo::vfunc6(f32* quat) {
    f32 mat[12];
    mat[0] = this->unk10;
    mat[1] = this->unk14;
    mat[2] = this->unk18;
    mat[3] = this->unk20;
    mat[4] = this->unk24;
    mat[5] = this->unk28;
    mat[6] = this->unk30;
    mat[7] = this->unk34;
    mat[8] = this->unk38;
    mat3_to_quat((f32(*)[3])mat, quat);
}

// vfunc18
void Kangaroo::vfunc18(f32* vec) {
    this->unk40 = vec[0];
    this->unk44 = vec[1];
    this->unk48 = vec[2];
}

// vfunc17
void Kangaroo::vfunc17(f32* vec) {
    vec[0] = this->unk40;
    vec[1] = this->unk44;
    vec[2] = this->unk48;
}

// vfunc16
f32 Kangaroo::vfunc16(f32* vec) {
    f32 dx = this->unk40 - vec[0];
    f32 dy = this->unk44 - vec[1];
    f32 dz = this->unk48 - vec[2];
    return dx * dx + dy * dy + dz * dz;
}

// vfunc15
void Kangaroo::vfunc15(f32* vec) {
    vec[0] = this->unk30;
    vec[1] = this->unk34;
    vec[2] = this->unk38;
}

// vfunc14
void Kangaroo::vfunc14(f32* vec) {
    vec[0] = this->unk20;
    vec[1] = this->unk24;
    vec[2] = this->unk28;
}

// vfunc13
void Kangaroo::vfunc13(f32* vec) {
    vec[0] = this->unk10;
    vec[1] = this->unk14;
    vec[2] = this->unk18;
}

// vfunc12
void Kangaroo::vfunc12(f32* quat) {
    f32 x = quat[0];
    f32 y = quat[1];
    f32 z = quat[2];
    f32 w = quat[3];
    f32 inv = 2.0f / (x * x + y * y + z * z + w * w);
    f32 xs = x * inv;
    f32 ys = y * inv;
    f32 zs = z * inv;
    f32 wx = w * xs;
    f32 wy = w * ys;
    f32 wz = w * zs;
    f32 xx = x * xs;
    f32 xy = x * ys;
    f32 xz = x * zs;
    f32 yy = y * ys;
    f32 yz = y * zs;
    f32 zz = z * zs;
    this->unk10 = 1.0f - (yy + zz);
    this->unk14 = xy - wz;
    this->unk18 = xz + wy;
    this->unk20 = xy + wz;
    this->unk24 = 1.0f - (xx + zz);
    this->unk28 = yz - wx;
    this->unk30 = xz - wy;
    this->unk34 = yz + wx;
    this->unk38 = 1.0f - (xx + yy);
}

void Kangaroo::func_8002B1D0(f32 scale) {
    this->unk50 = this->unk10 * scale;
    this->unk54 = this->unk14 * scale;
    this->unk58 = this->unk18 * scale;
    this->unk60 = this->unk20 * scale;
    this->unk64 = this->unk24 * scale;
    this->unk68 = this->unk28 * scale;
    this->unk70 = this->unk30 * scale;
    this->unk74 = this->unk34 * scale;
    this->unk78 = this->unk38 * scale;
    this->unk80 = this->unk40;
    this->unk84 = this->unk44;
    this->unk88 = this->unk48;
    Kangaroo* child = (Kangaroo*)this->child;
    while (child != NULL) {
        mat4_mul((f32(*)[4])&child->unk10, (f32(*)[4])&this->unk50, (f32(*)[4])&child->unk50);
        Joey* gc = child->child;
        if (gc != NULL) {
            do {
                ((Kangaroo*)gc)->func_8002B3B0(child);
                gc = gc->next;
            } while (gc != NULL);
        }
        child = (Kangaroo*)child->next;
    }
}

// vfunc1
void Kangaroo::vfunc1() {
    this->unk50 = this->unk10;
    this->unk54 = this->unk14;
    this->unk58 = this->unk18;
    this->unk60 = this->unk20;
    this->unk64 = this->unk24;
    this->unk68 = this->unk28;
    this->unk70 = this->unk30;
    this->unk74 = this->unk34;
    this->unk78 = this->unk38;
    this->unk80 = this->unk40;
    this->unk84 = this->unk44;
    this->unk88 = this->unk48;
    Kangaroo* child = (Kangaroo*)this->child;
    while (child != NULL) {
        mat4_mul((f32(*)[4])&child->unk10, (f32(*)[4])&this->unk50, (f32(*)[4])&child->unk50);
        Joey* gc = child->child;
        if (gc != NULL) {
            do {
                ((Kangaroo*)gc)->func_8002B3B0(child);
                gc = gc->next;
            } while (gc != NULL);
        }
        child = (Kangaroo*)child->next;
    }
}

void Kangaroo::func_8002B3B0(Kangaroo* parent) {
    mat4_mul((f32(*)[4])&this->unk10, (f32(*)[4])&parent->unk50, (f32(*)[4])&this->unk50);
    Joey* child = this->child;
    if (child != NULL) {
        do {
            ((Kangaroo*)child)->func_8002B3B0(this);
            child = child->next;
        } while (child != NULL);
    }
}

// vfunc5
void Kangaroo::vfunc5(f32* src, f32* dst) {
    dst[0] = src[0] * this->unk10;
    dst[1] = src[0] * this->unk20;
    dst[2] = src[0] * this->unk30;
    dst[0] += src[1] * this->unk14;
    dst[1] += src[1] * this->unk24;
    dst[2] += src[1] * this->unk34;
    dst[0] += src[2] * this->unk18;
    dst[1] += src[2] * this->unk28;
    dst[2] += src[2] * this->unk38;
}

// vfunc4
void Kangaroo::vfunc4(f32* src, f32* dst) {
    dst[0] = src[0] * this->unk10;
    dst[1] = src[0] * this->unk14;
    dst[2] = src[0] * this->unk18;
    dst[0] += src[1] * this->unk20;
    dst[1] += src[1] * this->unk24;
    dst[2] += src[1] * this->unk28;
    dst[0] += src[2] * this->unk30;
    dst[1] += src[2] * this->unk34;
    dst[2] += src[2] * this->unk38;
}

// vfunc2
void Kangaroo::vfunc2(f32* src, f32* dst) {
    dst[0] = src[0] * this->unk10;
    dst[1] = src[0] * this->unk14;
    dst[2] = src[0] * this->unk18;
    dst[0] += src[1] * this->unk20;
    dst[1] += src[1] * this->unk24;
    dst[2] += src[1] * this->unk28;
    dst[0] += src[2] * this->unk30;
    dst[1] += src[2] * this->unk34;
    dst[2] += src[2] * this->unk38;
    dst[0] += this->unk40;
    dst[1] += this->unk44;
    dst[2] += this->unk48;
}

// vfunc11
void Kangaroo::vfunc11(f32* a, f32* b) {
    f32 X[3];
    f32 Z[3];
    f32 Y[3];
    vec3_normalize(a, X);
    f32* Xp = X;
    f32 dot = b[0] * X[0] + b[1] * Xp[1] + b[2] * Xp[2];
    Y[0] = b[0] - dot * X[0];
    Y[1] = b[1] - dot * X[1];
    Y[2] = b[2] - dot * X[2];
    vec3_normalize(Y, Y);
    Z[0] = Y[1] * Xp[2] - Y[2] * Xp[1];
    Z[1] = Y[2] * X[0] - Y[0] * Xp[2];
    Z[2] = Y[0] * Xp[1] - Y[1] * X[0];
    this->unk10 = X[0];
    this->unk14 = X[1];
    this->unk18 = X[2];
    this->unk20 = Z[0];
    this->unk24 = Z[1];
    this->unk28 = Z[2];
    this->unk30 = Y[0];
    this->unk34 = Y[1];
    this->unk38 = Y[2];
}

// vfunc10
void Kangaroo::vfunc10(f32* a, f32* b) {
    f32 Z[3];
    f32 Y[3];
    f32 X[3];
    vec3_normalize(a, X);
    f32 dot = b[0] * X[0] + b[1] * X[1] + b[2] * X[2];
    Y[0] = b[0] - dot * X[0];
    Y[1] = b[1] - dot * X[1];
    Y[2] = b[2] - dot * X[2];
    vec3_normalize(Y, Y);
    Z[0] = Y[1] * X[2] - Y[2] * X[1];
    f32* Zp = Z;
    Zp[1] = Y[2] * X[0] - Y[0] * X[2];
    Zp[2] = Y[0] * X[1] - Y[1] * X[0];
    this->unk10 = Z[0];
    this->unk14 = Z[1];
    this->unk18 = Z[2];
    this->unk20 = Y[0];
    this->unk24 = Y[1];
    this->unk28 = Y[2];
    this->unk30 = X[0];
    this->unk34 = X[1];
    this->unk38 = X[2];
}

// vfunc9
void Kangaroo::vfunc9(f32* a, f32* b) {
    a[0] = this->unk10;
    a[1] = this->unk14;
    a[2] = this->unk18;
    b[0] = this->unk30;
    b[1] = this->unk34;
    b[2] = this->unk38;
}

// vfunc8
void Kangaroo::vfunc8(f32* a, f32* b) {
    b[0] = this->unk20;
    b[1] = this->unk24;
    b[2] = this->unk28;
    a[0] = this->unk30;
    a[1] = this->unk34;
    a[2] = this->unk38;
}

// vfunc7
void Kangaroo::vfunc7(f32* a, f32* b, f32* c) {
    c[0] = this->unk10;
    c[1] = this->unk14;
    c[2] = this->unk18;
    b[0] = this->unk20;
    b[1] = this->unk24;
    b[2] = this->unk28;
    a[0] = this->unk30;
    a[1] = this->unk34;
    a[2] = this->unk38;
}



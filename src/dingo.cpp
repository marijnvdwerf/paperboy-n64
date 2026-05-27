#include "dingo.h"

extern "C" s32 func_80029480(void* frustum, Vec3f* point, f32 radius);

#ifndef NON_MATCHING
// TODO: pending rodata migration
INCLUDE_RODATA("asm/nonmatchings/dingo", _vt.5Dingo);
#endif

s32 Dingo::vfunc9() {
    return 0;
}

void Dingo::vfunc11() {
}

void Dingo::vfunc10() {
}

void Dingo::vfunc19(Mat3f* mat) {
}

void Dingo::vfunc18(Mat3f* out) {
    out->m[0][1] = 0;
    out->m[0][2] = 0;
    out->m[1][0] = 0;
    out->m[1][2] = 0;
    out->m[2][0] = 0;
    out->m[2][1] = 0;
    out->m[0][0] = 1.0f;
    out->m[1][1] = 1.0f;
    out->m[2][2] = 1.0f;
}

void Dingo::vfunc17(Vec3f* a, Vec3f* b) {
    a->y = 0;
    a->z = 0;
    a->x = 1.0f;
    b->x = 0;
    b->y = 0;
    b->z = 1.0f;
}

void Dingo::vfunc16(Vec3f* dir, Vec3f* up) {
}

void Dingo::vfunc15(Vec3f* src, Vec3f* dst) {
    *dst = *src;
}

void Dingo::vfunc14(Vec3f* src, Vec3f* dst) {
    *dst = *src;
}

void Dingo::vfunc13(Vec3f* a, Vec3f* out) {
    out->x = a->x - pos.x;
    out->y = a->y - pos.y;
    out->z = a->z - pos.z;
}

void Dingo::vfunc12(Vec3f* a, Vec3f* out) {
    out->x = a->x + pos.x;
    out->y = a->y + pos.y;
    out->z = a->z + pos.z;
}

void Dingo::vfunc8() {
}

void Dingo::func_80122314(Vec3f* v) {
    target.x = v->x;
    target.y = v->y;
    target.z = v->z;
}

void Dingo::func_80122330(f32 halfExtent) {
    this->halfExtent = halfExtent;
    minBound = pos.x - halfExtent;
    maxBound = pos.x + halfExtent;
}

void Dingo::func_80122354(Vec3f* v) {
    pos.x = v->x;
    pos.y = v->y;
    pos.z = v->z;
    minBound = pos.x - halfExtent;
    maxBound = pos.x + halfExtent;
}

void Dingo::func_80122390(Vec3f* out) {
    out->x = target.x;
    out->y = target.y;
    out->z = target.z;
}

f32 Dingo::func_801223AC() {
    if (halfExtent < 0.0f) {
        vfunc1();
    }
    return halfExtent;
}

#ifdef NON_MATCHING
// TODO: pending rodata migration
void Dingo::vfunc1() {
    halfExtent = 0;
    f32 zero = halfExtent;
    pos.z = zero;
    pos.y = zero;
    pos.x = zero;
}
#else
INCLUDE_ASM("asm/nonmatchings/dingo", vfunc1__5Dingo);
#endif

void Dingo::func_80122414(Vec3f* out) {
    if (halfExtent < 0.0f) {
        vfunc1();
    }
    out->x = pos.x;
    out->y = pos.y;
    out->z = pos.z;
}

void Dingo::vfunc3(Vec3f* v) {
    func_80122354(v);
}

void Dingo::vfunc2(Vec3f* out) {
    out->x = pos.x;
    out->y = pos.y;
    out->z = pos.z;
}

s32 Dingo::vfunc7(Dingo* other) {
    if (halfExtent < 0.0f) {
        vfunc1();
    }
    if (other->halfExtent < 0.0f) {
        other->vfunc1();
    }
    f32 dx = other->pos.x - pos.x;
    f32 dy = other->pos.y - pos.y;
    f32 dz = other->pos.z - pos.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;
    if (distSq == 9.999999934e+36f) {
        return 0;
    }
    f32 radSum = halfExtent + other->halfExtent;
    if (distSq < radSum * radSum) {
        return 1;
    }
    return 0;
}

f32 Dingo::func_801225B0(Vec3f* point) {
    if (halfExtent < 0.0f) {
        vfunc1();
    }
    f32 dx = point->x - pos.x;
    f32 dy = point->y - pos.y;
    f32 dz = point->z - pos.z;
    return dx * dx + dy * dy + dz * dz;
}

void Dingo::vfunc6(void* frustum, DingoFrustumResult* out) {
    if (halfExtent < 0.0f) {
        vfunc1();
    }
    out->pad = 0;
    out->result = func_80029480(frustum, &pos, halfExtent);
}

void Dingo::vfunc5(f32 dt) {
    if (halfExtent < 0.0f) {
        vfunc1();
    }
    Vec3f* v = &vel;
    Vec3f delta;
    delta.x = v->x * dt;
    delta.y = v->y * dt;
    delta.z = v->z * dt;
    pos.x += delta.x;
    pos.y += delta.y;
    pos.z += delta.z;
    minBound = pos.x - halfExtent;
    maxBound = pos.x + halfExtent;
}

void Dingo::vfunc4(s32 steps) {
    if (halfExtent < 0.0f) {
        vfunc1();
    }
    Vec3f* v = &vel;
    f32 dt = (f32)steps;
    Vec3f delta;
    delta.x = v->x * dt;
    delta.y = v->y * dt;
    delta.z = v->z * dt;
    pos.x += delta.x;
    pos.y += delta.y;
    pos.z += delta.z;
    minBound = pos.x - halfExtent;
    maxBound = pos.x + halfExtent;
}

Dingo::Dingo() {
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    vel.x = 0;
    vel.y = 0;
    vel.z = 0;
    halfExtent = 0;
    minBound = 0;
    maxBound = 0;
    target.x = 0;
    target.y = 0;
    target.z = 0;
}

f32 Dingo::func_8012288C() {
    if (halfExtent < 0.0f) {
        vfunc1();
    }
    return maxBound;
}

f32 Dingo::func_801228DC() {
    if (halfExtent < 0.0f) {
        vfunc1();
    }
    return minBound;
}

void Dingo::func_8012292C(Vec3f* v) {
    vel.x = v->x;
    vel.y = v->y;
    vel.z = v->z;
}

void Dingo::func_80122948(Vec3f* out) {
    out->x = vel.x;
    out->y = vel.y;
    out->z = vel.z;
}

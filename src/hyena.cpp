#include "hyena.h"

void Hyena::func_80125610(f32 dt, Vec3f* accel) {
    Vec3f dv;
    dv.x = accel->x * dt;
    dv.y = accel->y * dt;
    dv.z = accel->z * dt;

    f32 half = dt * 0.5f;

    pos.x += vel.x * dt + half * dv.x;
    pos.y += vel.y * dt + half * dv.y;
    pos.z += vel.z * dt + half * dv.z;

    vel.x += dv.x;
    vel.y += dv.y;
    vel.z += dv.z;
}

Hyena::Hyena() {
    unk34 = 0;
    unk38 = 0;
    unk3C = 0;
    unk40 = 0;
}

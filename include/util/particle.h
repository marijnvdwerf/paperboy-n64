#ifndef UTIL_PARTICLE_H
#define UTIL_PARTICLE_H

#include "gol_world_entity.h"

struct Particle : GolWorldEntity {
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;

    Particle();
    void func_80125610(f32 dt, Vec3f* accel);
};

#endif

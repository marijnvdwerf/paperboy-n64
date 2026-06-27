#ifndef HYENA_H
#define HYENA_H

#include "dingo.h"

struct Hyena : Dingo {
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;

    Hyena();
    void func_80125610(f32 dt, Vec3f* accel);
};

#endif

#ifndef CURRAWONG_H
#define CURRAWONG_H

#include "bettong.h"

class Parrot;

struct Currawong : Bettong {
    /* 0x18 */ Vec3f* unk18;

    Currawong();
    ~Currawong(); // TODO: remove — auto-generated, not an override

    void vfunc2(Parrot* parrot) CXX_OVERRIDE;
    void vfunc3(s32 newCount) CXX_OVERRIDE;
    void vfunc4(void) CXX_OVERRIDE;
    void vfunc5(void) CXX_OVERRIDE;
    void vfunc8(s32 index, Vec3f* out) CXX_OVERRIDE;
    void vfunc12(s32 index, Vec3f* src) CXX_OVERRIDE;
};

#endif

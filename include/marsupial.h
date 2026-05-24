#ifndef MARSUPIAL_H
#define MARSUPIAL_H

#include "common.h"
#include "potoroo.h"

struct Marsupial : public Potoroo {
    /* 0x0C */ u8* unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8* unk14;

    Marsupial();
    ~Marsupial();

    void vfunc4(void) CXX_OVERRIDE;
    void vfunc14(PotorooTruffle* adj) CXX_OVERRIDE;
    void vfunc15(void) CXX_OVERRIDE;

    u8* func_8003B284(s32 index);
    u16 func_8003B294();
    s32 func_8003B2A0();

    static void func_8003B1B4(s32 arg0);
};

#endif // MARSUPIAL_H

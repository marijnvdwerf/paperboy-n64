#ifndef MARSUPIAL_H
#define MARSUPIAL_H

#include "common.h"
#include "potoroo.h"

struct MarsupialVertex {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s8 unkC;
    /* 0x0D */ s8 unkD;
    /* 0x0E */ s8 unkE;
    /* 0x0F */ u8 unkF;
};

struct Marsupial : public Potoroo {
    /* 0x0C */ MarsupialVertex* unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8* unk14;

    Marsupial();
    ~Marsupial();

    void vfunc4(void) CXX_OVERRIDE;
    void vfunc14(PotorooTruffle* adj) CXX_OVERRIDE;
    void vfunc15(void) CXX_OVERRIDE;

    MarsupialVertex* func_8003B284(s32 index);
    u16 func_8003B294();
    s32 func_8003B2A0();

    static void func_8003B1B4(s32 arg0);
};

#endif // MARSUPIAL_H

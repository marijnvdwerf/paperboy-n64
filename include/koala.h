#ifndef KOALA_H
#define KOALA_H

#include "common.h"

struct Koala {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x18 */ s32 pad18;
    /* 0x1C */ s32 unk1C;

    Koala();
    ~Koala();
};

#endif

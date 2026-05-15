#ifndef STREAMS_H
#define STREAMS_H

#include "common.h"

struct Sentry {
    /* 0x0 */ s32 state;
    /* 0x4 */ s32 flag;
    /* 0x8 */ /* vptr */
    Sentry();
    virtual ~Sentry();
    virtual s32 vfunc2() = 0;
    virtual s32 vfunc3() = 0;
    virtual s32 vfunc4() = 0;
    virtual s32 vfunc5() = 0;
    virtual s32 vfunc6();
    virtual s32 vfunc7();
    virtual s32 vfunc8();
    virtual s32 vfunc9();

    void clearFlag();
    s32 getFlag();
    s32 getState();
    s32 isInactive();
    s32 activate();
    s32 setMode15();
    s32 setMode19();
    s32 setMode14();
    s32 setMode13();
    s32 setMode12();
    void reset();
};

struct Stream {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ void* unk4;
    /* 0x8 */ u8 pad8[2];
    /* 0xA */ u16 unkA;
    /* 0xC */ s32 unkC;
};

struct Otter : public Sentry {
    /* 0xC  */ Stream* stream;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ char buf[0x68];

    Otter();
    virtual ~Otter();
};

#endif

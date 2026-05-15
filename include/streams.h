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

#endif

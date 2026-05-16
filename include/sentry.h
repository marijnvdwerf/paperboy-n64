#ifndef SENTRY_H
#define SENTRY_H

#include "common.h"

struct FileInfo {
    /* 0x00 */ char name[0x20];
    /* 0x20 */ s32 fileSize;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
};

struct Sentry {
    /* 0x0 */ s32 state;
    /* 0x4 */ s32 flag;
    /* 0x8 */ /* vptr */
    Sentry();
    virtual ~Sentry();
    virtual s32 vfunc2() = 0;
    virtual void vfunc3() = 0;
    virtual s32 vfunc4(const char* path) = 0;
    virtual s32 vfunc5(const char* name) = 0;
    virtual s32 vfunc6(FileInfo* info);
    virtual s32 vfunc7(s32* out1, s32* out2);
    virtual s32 vfunc8(s32* out);
    virtual s32 vfunc9(s32 idx, FileInfo* info);

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

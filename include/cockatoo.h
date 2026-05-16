#ifndef COCKATOO_H
#define COCKATOO_H

#include "parrot.h"

struct Cockatoo : public Parrot {
    /* 0x650 */ void* unk650;

    Cockatoo();
    virtual s32 close();
    virtual void selectDriver(const char* path);
    virtual char* getExtension();
    virtual void vfunc19(s32 arg1);
    virtual s32 vfunc20();
    virtual s32 vfunc21();

    void func_80040AF0();
    s32 func_80040910(s32 nbytes);
    void func_800409C8();
};

#endif

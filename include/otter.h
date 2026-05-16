#ifndef OTTER_H
#define OTTER_H

#include "sentry.h"

struct OtterStone {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ void* unk4;
    /* 0x8 */ u8 pad8[2];
    /* 0xA */ u16 unkA;
    /* 0xC */ s32 unkC;
};

struct Otter : public Sentry {
    /* 0xC  */ OtterStone* unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ char buf[0x68];

    s32 func_80049DB4() {
        return this->unkC != NULL;
    }

    char* func_80049DAC() {
        return this->buf;
    }

    OtterStone* func_80049DA0() {
        return this->unkC;
    }

    Otter() {
        unkC = NULL;
        unk10 = 0;
        memset(buf, 0, 0x68);
    }

    virtual ~Otter() {
        this->func_80049B50();
    }

    void func_80049CC4(OtterStone* arg0, s32 mode, s32 hasInit);

    virtual s32 vfunc2();

    virtual void vfunc3();

    void func_80049B58(s32 mode);

    void func_80049B50();

    virtual s32 vfunc4(const char* path);

    static void func_80049980(const char* path, char* name, char* ext);

    virtual s32 vfunc7(s32* out1, s32* out2);

    virtual s32 vfunc9(s32 arg1, FileInfo* info);

    virtual s32 vfunc8(s32* out);

    virtual s32 vfunc5(const char* name);

    virtual s32 vfunc6(FileInfo* arg1);
};

#endif

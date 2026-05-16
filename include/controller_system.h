#ifndef CONTROLLER_SYSTEM_H
#define CONTROLLER_SYSTEM_H

#include "input.h"

class ControllerSystem {
  public:
    /* 0x00 */ s32 active[4];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18[4];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 count;
    /* 0x38 */ ControllerDevice* unk38;
    /* 0x3C */ ControllerDevice* unk3C;
    /* 0x40 */ ControllerDevice* entries[4];
    /* 0x50 */ /* vptr */

    ControllerSystem();
    virtual void init();
    virtual ~ControllerSystem();
    virtual ControllerDevice* findByPort(s32 port);
    virtual ControllerDevice* findOrDefault(s32 port);
    virtual void vfunc5() = 0;
    virtual void vfunc6() = 0;
    virtual void vfunc7() = 0;
    virtual void vfunc8() = 0;

    void func_80041D40();
    void func_80041E1C();
    void func_80041FD4();
    void func_80042004();
    void func_8004210C(s32 i, s32 val);
    ControllerDevice* func_8004211C(s32 i);
    ControllerDevice* func_80042130();
    ControllerDevice* func_8004213C();
    s32 func_80042148(s32 i);
    s32 func_8004215C();
    s32 func_80042168();
    s32 func_80042174();
    s32 func_80042180();
};

#endif

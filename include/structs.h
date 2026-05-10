#ifndef STRUCTS_H
#define STRUCTS_H

#ifdef __cplusplus

#include "common.h"

class StructUU {
  public:
    /* 0x00 */ // vtable

    virtual ~StructUU();
    virtual void vfunc1();
    virtual void vfunc2();
};

class StructVV {
  public:
    /* 0x00 */ char pad0[0x4];
    /* 0x04 */ // vtable

    virtual ~StructVV();
    virtual void vfunc1(const char*);
    virtual void vfunc2();
    virtual void vfunc3();
    virtual StructUU* vfunc4(s32);
    virtual void vfunc5(StructUU*);
};

class StructWWBase {
  public:
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ // vtable

    StructWWBase();
    virtual ~StructWWBase();
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual StructVV* vfunc5();
    virtual void vfunc6(StructVV*);
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9();
    virtual void vfunc10();
    virtual void vfunc11();
    virtual void vfunc12();
    virtual void vfunc13();
};

class StructWW : public StructWWBase {
  public:
    /* 0x18 */ char pad18[0x5C];
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ StructVV* unk7C;
    /* 0x80 */ StructUU* unk80;

    StructWW();
    virtual ~StructWW();

    void func_80009210();
    void func_80009270(s32);
    void func_800092F8();
    void func_80009350(s32);
    static StructWW* func_800094A0();
};

#endif

#endif

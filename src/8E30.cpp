#include "common.h"

class StructTTBase {
  public:
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9();
    virtual void vfunc10();
    virtual void vfunc11();
    virtual void vfunc12();
    virtual void vfunc13() = 0;
    virtual void vfunc14() = 0;
};

class StructTT : public StructTTBase {
  public:
    StructTT();
    virtual ~StructTT();
    virtual void vfunc16() = 0;
    virtual void vfunc17() = 0;
    virtual void vfunc18() = 0;

    void func_80008260();
    void func_8000826C();
    void func_80008294();
    u32 func_8000829C();

    /* 0x4 */ u8 pad4[4];
    /* 0x8 */ u32 unk8;
    /* 0xC */ u8 unkC;
};

StructTT::~StructTT() {
}

void StructTT::func_80008260() {
    unk8 = 0;
    unkC = 0;
}

void StructTT::func_8000826C() {
    unk8 = 1;
    unkC = 0;
}

StructTT::StructTT() {
    unk8 = 0;
}

void StructTT::func_80008294() {
    unkC = 0;
}

u32 StructTT::func_8000829C() {
    return unk8;
}

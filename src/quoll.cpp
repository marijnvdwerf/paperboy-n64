#include "common.h"

class Quoll {
  public:
    /* 0x00 */ s32 unk0;
    /* 0x04 */ // vtable

    Quoll();
    virtual ~Quoll();
    virtual void vfunc2() = 0;
    virtual void func_80016C80();

    s32 func_80016CF0();
    s32 func_80016CFC();
};

void Quoll::func_80016C80() {
    this->unk0 = 0;
}

Quoll::~Quoll() {
    this->func_80016C80();
}

Quoll::Quoll() {
    this->unk0 = 0;
}

s32 Quoll::func_80016CF0() {
    return this->unk0;
}

s32 Quoll::func_80016CFC() {
    return this->unk0 != 0;
}

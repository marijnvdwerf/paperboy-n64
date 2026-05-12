#include "common.h"
#include "structs.h"

// TODO: unk10 type — probably a child of some abstract base. For now declare a
// local interface with the virtuals we call here.
class StructYYSubA8Unk10 {
  public:
    /* 0x00 */ char pad0[0x22];
    /* 0x22 */ u16 unk22;
    /* 0x24 */ char pad24[0x8];
    /* 0x2C */ // vtable

    virtual void vfunc1() = 0;
    virtual void vfunc2() = 0;
    virtual void vfunc3() = 0;
    virtual void vfunc4() = 0;
    virtual void vfunc5() = 0;
    virtual void vfunc6(s32) = 0;
    virtual void vfunc7() = 0;
    virtual void vfunc8() = 0;
    virtual void vfunc9() = 0;
    virtual void vfunc10() = 0;
    virtual void vfunc11() = 0;
    virtual void vfunc12() = 0;
    virtual void vfunc13() = 0;
    virtual s32 vfunc14(StructYYSubA8GrandBase*, s32, s32, s32) = 0;
    virtual void vfunc15() = 0;
};

void StructYYSubA8GrandBase::vfunc6() {
    StructYYSubA8Unk10* obj = (StructYYSubA8Unk10*)this->unk10;
    this->unkC &= ~1;
    if (obj != NULL && (obj->unk22 & 1)) {
        obj->vfunc15();
    }
}

void StructYYSubA8GrandBase::vfunc7() {
    ((StructYYSubA8Unk10*)this->unk10)->vfunc6(0);
}

void StructYYSubA8GrandBase::vfunc4() {
}

void StructYYSubA8GrandBase::vfunc3() {
}

s32 StructYYSubA8GrandBase::vfunc9(s32 a, s32 b, s32 c, s32 d) {
    if (a == 0) {
        a = 0x140;
    }
    if (b == 0) {
        b = 0xF0;
    }
    if (c == 0) {
        c = 0x10;
    }
    this->unk0 = a;
    this->unk4 = b;
    this->unk8 = c;
    this->unkC = d;
    s32 ret = this->vfunc1();
    if (ret) {
        return ret;
    }
    StructYYSubA8Unk10* obj = (StructYYSubA8Unk10*)this->unk10;
    obj->vfunc14(this, this->unk0, this->unk4, this->unk8);
    this->unkC = (this->unkC | 1) & ~2;
    return 0;
}

void StructYYSubA8GrandBase::vfunc8() {
    StructYYSubA8Unk10* obj = (StructYYSubA8Unk10*)this->unk10;
    this->unkC |= 2;
    if (obj != NULL && (obj->unk22 & 1)) {
        obj->vfunc15();
    }
}

s32 StructYYSubA8GrandBase::vfunc5(s32 a, s32 b, s32 c, s32 d) {
    if (this->unkC & 1) {
        this->vfunc6();
    }
    this->unkC |= 2;
    s32 ret = StructYYSubA8GrandBase::vfunc9(a, b, c, d);
    if (ret) {
        return ret;
    }
    return 0;
}

StructYYSubA8GrandBase::~StructYYSubA8GrandBase() {
}

StructYYSubA8GrandBase::StructYYSubA8GrandBase() {
    this->unk0 = 0;
    this->unk4 = 0;
    this->unk8 = 0;
    this->unkC = 0;
    this->unk10 = NULL;
}

s32 StructYYSubA8GrandBase::func_80016AB8() {
    return this->unkC & 0x80000;
}

s32 StructYYSubA8GrandBase::func_80016AC8() {
    return this->unkC & 0x40000;
}

s32 StructYYSubA8GrandBase::func_80016AD8() {
    return this->unkC & 0x10000;
}

s32 StructYYSubA8GrandBase::func_80016AE8() {
    return this->unkC & 0x400;
}

s32 StructYYSubA8GrandBase::func_80016AF4() {
    return this->unkC & 0x2000;
}

s32 StructYYSubA8GrandBase::func_80016B00() {
    return this->unkC & 0x200;
}

s32 StructYYSubA8GrandBase::func_80016B0C() {
    return this->unkC & 0x100;
}

s32 StructYYSubA8GrandBase::func_80016B18() {
    return this->unkC & 0x40;
}

s32 StructYYSubA8GrandBase::func_80016B24() {
    return this->unkC & 0x40;
}

s32 StructYYSubA8GrandBase::func_80016B30() {
    return this->unkC & 0x20;
}

s32 StructYYSubA8GrandBase::func_80016B3C() {
    return this->unkC & 0x10;
}

s32 StructYYSubA8GrandBase::func_80016B48() {
    return this->unkC & 0x8;
}

s32 StructYYSubA8GrandBase::func_80016B54() {
    return this->unkC & 0x4;
}

s32 StructYYSubA8GrandBase::func_80016B60() {
    return this->unkC & 0x2;
}

s32 StructYYSubA8GrandBase::func_80016B6C() {
    return this->unkC & 0x1;
}

s32 StructYYSubA8GrandBase::func_80016B78() {
    return this->unk8;
}

s32 StructYYSubA8GrandBase::func_80016B84() {
    return this->unk4;
}

s32 StructYYSubA8GrandBase::func_80016B90() {
    return this->unk0;
}

void* StructYYSubA8GrandBase::func_80016B9C() {
    return this->unk10;
}

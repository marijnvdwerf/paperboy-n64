#include "common.h"
#include "structs.h"

void StructYYBase::func_8000C6C0() {
    for (u32 i = 0; i < this->unk68; i++) {
        this->archives[i].func_80045288();
    }
}

void StructYYBase::vfunc8(s32 a, s32 b, s32 c, s32 d) {
    this->vfunc9(a, b, c, d);
}

void StructYYBase::func_8000C74C() {
    if (this->unk88 != NULL) {
        this->unk88->vfunc1();
    }
}

void StructYYBase::vfunc1() {
    this->unk70->vfunc6();
}

void StructYYBase::func_8000C7AC() {
    this->vfunc10();
}

StructYYBase::~StructYYBase() {
    this->unk6C = 0;
    this->unk70 = NULL;
    this->unk74 = 0;
    this->unk78 = 0;
    this->unk0 = 0;
    this->unk7C = 0;
    this->unk80 = 0;
    this->unk84 = 0;
    this->unk8C = 0;
    this->unk90 = 0;
    this->unk94 = 0x64;
    this->unk68 = 0;
    this->unk98 = 0;
    this->unk88 = NULL;
}

StructYYBase::StructYYBase() {
    this->unk6C = 0;
    this->unk70 = 0;
    this->unk74 = 0;
    this->unk78 = 0;
    this->unk0 = 0;
    this->unk7C = 0;
    this->unk80 = 0;
    this->unk84 = 0;
    this->unk8C = 0;
    this->unk90 = 0;
    this->unk94 = 0x64;
    this->unk68 = 0;
    this->unk98 = 0;
    this->unk88 = 0;
}

void StructYYBase::func_8000C9B8() {
    this->unk6C = 0;
    this->unk70 = NULL;
    this->unk74 = 0;
    this->unk78 = 0;
    this->unk0 = 0;
    this->unk7C = 0;
    this->unk80 = 0;
    this->unk84 = 0;
    this->unk8C = 0;
    this->unk90 = 0;
    this->unk94 = 0x64;
    this->unk68 = 0;
    this->unk98 = 0;
    this->unk88 = NULL;
}

s32 StructYYBase::func_8000C9F8() {
    return this->unk0;
}

JamArchive* StructYYBase::func_8000CA04(s32 idx) {
    return &this->archives[idx];
}

void StructYYBase::func_8000CA24() {
    this->unk0 &= ~0x4000;
}

void StructYYBase::func_8000CA38() {
    this->unk0 |= 0x4000;
}

s32 StructYYBase::func_8000CA48() {
    return this->unk0 & 0x4000;
}

s32 StructYYBase::func_8000CA54() {
    return this->unk68;
}

s32 StructYYBase::func_8000CA60() {
    return this->unk98;
}

s32 StructYYBase::func_8000CA6C() {
    return this->unk0 & 0x10;
}

s32 StructYYBase::func_8000CA78() {
    return this->unk0 & 0x100;
}

void StructYYBase::func_8000CA84(s32 v) {
    this->unk94 = v;
}

s32 StructYYBase::func_8000CA8C() {
    return this->unk94;
}

s32 StructYYBase::func_8000CA98() {
    return this->unk90;
}

s32 StructYYBase::func_8000CAA4() {
    return this->unk8C;
}

s32 StructYYBase::func_8000CAB0() {
    return this->unk78;
}

s32 StructYYBase::func_8000CABC() {
    return this->unk74;
}

StructYYHandler* StructYYBase::func_8000CAC8() {
    return this->unk70;
}

StructYYInner* StructYYBase::func_8000CAD4() {
    return this->unk6C;
}

s32 StructYYBase::func_8000CAE0() {
    return this->unk70->unkC & 0x2000;
}

s32 StructYYBase::func_8000CAF0() {
    return this->unk0 & 8;
}

s32 StructYYBase::func_8000CAFC() {
    return this->unk0 & 2;
}

s32 StructYYBase::func_8000CB08() {
    return this->unk0 & 1;
}

// automatic File destructor
// INCLUDE_ASM("asm/nonmatchings/D2C0", func_8000CB14);

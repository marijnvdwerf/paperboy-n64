#include "common.h"
#include "structs.h"

extern "C" {
extern void* D_8006AB20;
extern u8 D_80000800[];
extern char D_80000818[];
extern u8 D_80000820[];
extern u8 D_80076404[];
void func_80006730(u8*, u8*, s32);
}

void StructWW::func_80009210() {
    if (this->unk80 != NULL) {
        this->unk80->vfunc2();
        this->unk7C->vfunc5(this->unk80);
        this->unk80 = NULL;
    }
}

void StructWW::func_80009270(s32 arg1) {
    this->func_80009210();

    if (&D_80000800 != NULL) {
        func_80006730(D_80076404, D_80000800, 0x100);
        D_80076404[0xFF] = 0;
    } else {
        D_80076404[0] = 0;
    }

    this->unk80 = this->unk7C->vfunc4(arg1);
}

void StructWW::func_800092F8() {
    this->unk78 = 0;
    this->func_80009210();
    this->vfunc6(this->unk7C);
    this->unk7C = 0;
    this->vfunc2();
}

void StructWW::func_80009350(s32) {
    this->unk78 = 1;
    this->vfunc1();
    this->unk7C = this->vfunc5();

    if (&D_80000800 != NULL) {
        func_80006730(D_80076404, D_80000800, 0x100);
        D_80076404[0xFF] = 0;
    } else {
        D_80076404[0] = 0;
    }

    this->unk7C->vfunc1(D_80000818);

    if (&D_80000820 != NULL) {
        func_80006730(D_80076404, D_80000820, 0x100);
        D_80076404[0xFF] = 0;
    } else {
        D_80076404[0] = 0;
    }
}

StructWW::~StructWW() {
    D_8006AB20 = NULL;
}

StructWW::StructWW() {
    unk78 = 0;
    unk7C = 0;
    unk80 = 0;
    D_8006AB20 = this;
}

StructWW* StructWW::func_800094A0() {
    return (StructWW*)D_8006AB20;
}

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

extern "C" void func_80009210(StructWW* arg0) {
    if (arg0->unk80 != NULL) {
        arg0->unk80->vfunc2();
        arg0->unk7C->vfunc5(arg0->unk80);
        arg0->unk80 = NULL;
    }
}

INCLUDE_ASM("asm/nonmatchings/9E10", func_80009270);

extern "C" void func_800092F8(StructWW* arg0) {
    arg0->unk78 = 0;
    func_80009210(arg0);
    arg0->vfunc6(arg0->unk7C);
    arg0->unk7C = 0;
    arg0->vfunc2();
}

extern "C" void func_80009350(StructWW* arg0, s32) {
    arg0->unk78 = 1;
    arg0->vfunc1();
    arg0->unk7C = arg0->vfunc5();

    if (&D_80000800 != NULL) {
        func_80006730(D_80076404, D_80000800, 0x100);
        D_80076404[0xFF] = 0;
    } else {
        D_80076404[0] = 0;
    }

    arg0->unk7C->vfunc1(D_80000818);

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

INCLUDE_ASM("asm/nonmatchings/9E10", func_800094A0);

INCLUDE_ASM("asm/nonmatchings/9E10", func_800094B0);

INCLUDE_ASM("asm/nonmatchings/9E10", func_80009550);

INCLUDE_ASM("asm/nonmatchings/9E10", func_80009938);

INCLUDE_ASM("asm/nonmatchings/9E10", func_80009A64);

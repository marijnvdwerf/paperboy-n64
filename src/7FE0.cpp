#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

class StructYY {
public:
    /* 0x00 */ char pad0[0x70];
    /* 0x70 */ s32 unk70;
    /* 0x74 */ char pad74[0x28];

    virtual void vfunc0();
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9(s32, s32, s32, s32);
};

class StructWW {
public:
    /* 0x00 */ char pad0[0x74];
    /* 0x74 */ s32 unk74;
};

class StructXX {
public:
    /* 0x00 */ char pad0[0x4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ char pad10[0x38];
    /* 0x48 */ StructYY* unk48;
    /* 0x4C */ StructWW* unk4C;
    /* 0x50 */ s32 unk50;

    void func_80007DE0();
};

extern "C" {
void func_80006730(u8*, u8*, s32);
void func_80009350(void*, s32);
StructWW* func_80009458(s32);
s32 func_8004B414(s32);
#ifdef PAL
void func_8005FA60(s32);
#endif
extern u8 D_800005D8[];
extern u8 D_800005E8[];
extern u8 D_80076404[];
}

INCLUDE_ASM("asm/nonmatchings/7FE0", func_800073E0);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_8000752C);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007660);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007898);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_800078B8);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_800078DC);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007A60);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007CB8);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007D14);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007D70);

void StructXX::func_80007DE0() {
    StructYY* temp_a0;
    StructWW* temp_v0;

    temp_a0 = this->unk48;
    temp_a0->vfunc9(this->unk4, this->unk8, this->unkC, this->unk50);

#ifdef PAL
    func_8005FA60(1);
#endif
    if (&D_800005D8 != NULL) {
        func_80006730(D_80076404, D_800005D8, 0x100);
        D_80076404[0xFF] = 0;
    } else {
        D_80076404[0] = 0;
    }
    temp_v0 = func_80009458(func_8004B414(0x84));
    this->unk4C = temp_v0;
    temp_v0->unk74 = (s32)(this->unk48->unk70 + 0x20);
    func_80009350(this->unk4C, 0x14);
    if (&D_800005E8 != NULL) {
        func_80006730(D_80076404, D_800005E8, 0x100);
        D_80076404[0xFF] = 0;
        return;
    }
    D_80076404[0] = 0;
}

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007EC4);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007F10);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007F54);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007FE0);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80008040);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_800080F8);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80008100);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_8000812C);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80008138);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80008148);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80008158);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80008164);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80008174);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80008184);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80008194);

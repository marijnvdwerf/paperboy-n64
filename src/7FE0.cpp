#include "common.h"

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
void func_80007660();
void func_80007EC4(void*);
void func_80007F10(void*);
s32 func_80007F54(void*, s32, u8**);
int sprintf(char*, const char*, ...);
#ifdef PAL
void func_8005FA60(s32);
#endif
s32 func_8004ABD0(void*, s32, s32, ...);
void func_8002BECC(s32);
void func_8002BD30(s32);
void func_80036BF4(s32);
void func_80044458(s32);
void func_8004527C(s32);
void func_8003D76C(s32);
void func_8003B3A0(s32);
void func_8003DE3C(s32);
void func_8003E42C(s32);
void func_8004B3BC(s32);
extern u8 D_800005D8[];
extern u8 D_800005E8[];
extern u8 D_80076404[];
extern char D_80079360[];
extern u8 D_8006A480[];
extern s32 D_8006AAD0;
extern s32 D_8006AAD4;
extern s32 D_8006AAD8;
extern s32 D_8006AADC;
extern s32 D_8006AAE0;
extern s32 D_8006AAE4;
extern s32 D_8006AAE8;
extern s32 D_8006AAEC;
extern u32 D_8006AAF0;
extern u32 D_8006AAF4;
extern u32 D_8006AAF8;
extern u32 D_8006D5F0;
extern u8 D_800768F0;
}

INCLUDE_ASM("asm/nonmatchings/7FE0", func_800073E0);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_8000752C);

#ifdef NON_MATCHING
extern "C" void func_80007660(void) {
    u32 total_size = 0;

    total_size += ((D_8006AAF0 * D_8006AAF4 * D_8006AAF8) >> 2) + 0x100;
    D_8006AAD0 = func_8004ABD0(&D_800768F0, ((D_8006AAF0 * D_8006AAF4 * D_8006AAF8) >> 2) + 0x100, 6);

    total_size += 0x3C800;
    D_8006AAD4 = func_8004ABD0(&D_800768F0, 0x3C800, 6);

    total_size += 0x28000;
    D_8006AAD8 = func_8004ABD0(&D_800768F0, 0x28000, 4);

    total_size += 0x38000;
    D_8006AADC = func_8004ABD0(&D_800768F0, 0x38000, 6);

    total_size += 0x8800;
    D_8006AAE0 = func_8004ABD0(&D_800768F0, 0x8800, 3);

    total_size += 0x1C000;
    D_8006AAE4 = func_8004ABD0(&D_800768F0, 0x1C000, 3);

    {
        u32 temp_s0 = ((D_8006AAF0 * D_8006AAF4 * 0x10) >> 3) + 0x100;
        total_size += temp_s0;

        D_8006AAE8 = func_8004ABD0(&D_800768F0, (D_8006D5F0 - total_size) - 0x80, 3);
        D_8006AAEC = func_8004ABD0(&D_800768F0, temp_s0, 6);
    }

    func_8002BECC(D_8006AAD0); 
    func_8002BD30(D_8006AAD4);
    func_80036BF4(D_8006AAD4);
    func_80044458(D_8006AAD8);
    func_8004527C(D_8006AAD8);
    func_8003D76C(D_8006AADC);
    func_8003B3A0(D_8006AAEC);
    func_8003DE3C(D_8006AAE0);
    func_8003E42C(D_8006AAE0);
    func_8004B3BC(D_8006AAE8);
}
#else
INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007660);
#endif

INCLUDE_ASM("asm/nonmatchings/7FE0", func_80007898);

INCLUDE_ASM("asm/nonmatchings/7FE0", func_800078B8);

extern "C" void func_800078DC(s32 errCode, const char* file, s32 line, const char* prettyMsg) {
    const char* msg;
    const char* p = prettyMsg;

    // TODO: fix this in a natural way. This is just here to fix rodata ordering
    const char* DEFAULT_MESSAGE = "<NO PRETTYMSG>";
    const char* DEFAULT_FMT = "%s,\nfile %s, line %d, (%s)";

    switch (errCode) {
        case 0:
            msg = "Invalid error condition";
            break;
        case -1:
            msg = "General error occurred";
            break;
        case -2:
            msg = "Invalid parameter detected";
            break;
        case -3:
            msg = "Invalid value detected";
            break;
        case -4:
            msg = "Out of memory error occured";
            break;
        case -5:
            msg = "Critical resource busy";
            break;
        case -7:
            msg = "Assertion Failed";
            break;
        default:
            msg = "Unknown error condition occured";
            break;
    }
    if (p == NULL) {
        p = "<NO PRETTYMSG>";
    }
    sprintf(D_80079360, "%s,\nfile %s, line %d, (%s)", msg, file, line, p);
    __asm__("break 1, 7");
    while (1) {}
}

extern "C" void func_800079A8(const char* msg, const char* file, s32 line, const char* prettyMsg) {
    const char* p = prettyMsg;
    if (p == NULL) {
        p = "<NO PRETTYMSG>";
    }
    sprintf(D_80079360, "%s,\nfile %s, line %d, (%s)", msg, file, line, p);
    __asm__("break 1, 7");
    while (1) {}
}

extern "C" s32 func_800079F4(s32 argc, u8** argv) {
    func_80007660();
    if (func_80007F54(D_8006A480, argc, argv) != 0) {
        func_80007EC4(D_8006A480);
        func_80007F10(D_8006A480);
    }
    return 0;
}

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

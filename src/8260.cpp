#include "common.h"
#include "os_pi.h"
#include "os_thread.h"

class Actor;

struct GameContextBase {
    virtual void func1();
    virtual void func2();
    virtual void func3();
    virtual void func4();
    virtual void func5();
    virtual void func6();
    virtual void func7();
    virtual void func8();
    virtual void func9();
    virtual void func10();
    virtual void func11();
    virtual void func12();
};

struct GameSubContext {
    /* 0x000 */ char pad0[0xB0];
    /* 0x0B0 */ s32 unk0B0; // = 1
    /* 0x0B4 */ s32 unk0B4; // = 1
    /* 0x0B8 */ f32 unk0B8; // D_80000668 (80.0f)
    /* 0x0BC */ f32 unk0BC; // D_8000066C (4.0f)
    /* 0x0C0 */ f32 unk0C0; // D_8006AB00
    /* 0x0C4 */ char pad0C4[0x4];
    /* 0x0C8 */ s32 unk0C8; // = 1
    /* 0x0CC */ s32 unk0CC; // = 0
    /* 0x0D0 */ s32 unk0D0; // = 0
    /* 0x0D4 */ s32 unk0D4; // = 1
    /* 0x0D8 */ char pad0D8[0x524];
    /* 0x5FC */

    GameSubContext();
};

class StructYY;
class StructWW;

struct GameContext : GameContextBase {
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ OSMesgQueue unk14;
    /* 0x02C */ OSMesg unk2C;
    /* 0x030 */ OSIoMesg unk30;
    /* 0x048 */ StructYY* unk48;
    /* 0x04C */ StructWW* unk4C;
    /* 0x050 */ s32 unk50;
    /* 0x054 */ GameSubContext unk54;

    GameContext();
    virtual void func1();
    virtual void func13();
    virtual ~GameContext();

    void func_80007DE0();
};

class StructYY {
  public:
    /* 0x00 */ s32 unk0;
    /* 0x04 */ char pad4[0x6C];
    /* 0x70 */ s32 unk70;
    /* 0x74 */ char pad74[0x28];

    virtual ~StructYY();
    virtual void vfunc1();
    virtual void vfunc2(const char*, s32);
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9(s32, s32, s32, s32);
    virtual void vfunc10();
};

class StructWWBase {
  public:
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ // vtable
    virtual ~StructWWBase();
};

class StructWW : public StructWWBase {
  public:
    /* 0x18 */ char pad18[0x5C];
    /* 0x74 */ s32 unk74;
};

extern "C" {
// Functions
void func_80006310(void*, s32, u32);
void func_80006730(u8*, u8*, s32);
void func_8000812C();
void func_800085B8(Actor*);
void func_80008FA8(Actor*);
void func_800092F8(StructWW*);
void func_80009350(void*, s32);
StructWW* func_80009458(s32);
void func_80011500(s32);
void func_80019080(s32);
void func_80020DA4(s32);
void func_8002151C(s32);
void func_8002377C(s32);
void func_80024C10(s32);
void func_80026B50(s32);
void func_80026F10(s32);
void func_800284D8(s32);
void func_80029654(s32);
void func_8002C2CC(s32);
void func_8002DBBC(s32);
void func_8002DFF0(s32);
void func_8002E874(s32);
void func_800308D8(s32);
void func_80037C08(s32);
void func_80038C90(s32);
void func_80039A94(s32);
void func_8003A340(s32);
void func_8003B1B4(s32);
void func_8003F97C(s32);
s32 func_8004B414(s32);
void func_80007660();
void func_80007A60(GameContext*);
void func_80007CB8();
void func_80007D14();
void func_80007D70(GameContext*);
void func_80007EC4(GameContext*);
void func_80007F10(GameContext*);
s32 func_80007F54(GameContext*, s32, u8**);
void func_800BFF50(GameSubContext*);
void* func_8000D0B0(s32);
void func_8011EC68(s32);
void func_8012016C(s32);
void func_80124198(s32);
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

// Data
extern char D_800005CC[];
extern u8 D_800005D8[];
extern u8 D_800005E8[];
extern u8 D_80076404[];
extern char D_80079360[];
extern GameContext D_8006A480;
extern s32 D_8006AAFC;
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
extern void* D_8006AB04;
extern Actor* D_8006AB10;
extern u32 D_80000668;
extern u32 D_8000066C;
extern f32 D_8006AB00;
extern char first_ROM_START[];
extern char first_ROM_END[];
extern char first_VRAM[];
extern char first_VRAM_END[];
extern s32 D_80076170;
extern s32 D_80076630;
extern s32 D_801286D4;
}

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
INCLUDE_ASM("asm/nonmatchings/8260", func_80007660);
#endif

INCLUDE_ASM("asm/nonmatchings/8260", func_80007898); // global ctor

INCLUDE_ASM("asm/nonmatchings/8260", func_800078B8); // global dtor

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
    if (func_80007F54(&D_8006A480, argc, argv) != 0) {
        func_80007EC4(&D_8006A480);
        func_80007F10(&D_8006A480);
    }
    return 0;
}

extern "C" void func_80007A60(GameContext* arg0) {
    s32 size = (s32)(first_VRAM_END - first_VRAM);
    func_80006310(first_VRAM, 0, size);
    osInvalDCache(first_VRAM, size);
    osInvalICache(first_VRAM, size);
    osPiStartDma(
        &arg0->unk30,
        0,
        0,
        (u32)first_ROM_START,
        first_VRAM,
        (s32)(first_ROM_END - first_ROM_START),
        &arg0->unk14
    );
    osRecvMesg(&arg0->unk14, NULL, OS_MESG_BLOCK);

    func_8003A340(D_8006AAE8);
    func_800308D8(D_8006AAE8);
    func_8003A340(D_8006AAE8);
    func_800308D8(D_8006AAE8);
    func_80038C90(D_8006AAE8);
    func_80024C10(D_8006AAE8);
    func_8003B1B4(D_8006AAE8);
    func_80037C08(D_8006AAE8);
    func_80020DA4(D_8006AAE8);
    func_80011500(D_8006AAE8);
    func_800284D8(D_8006AAE8);
    func_8003B1B4(D_8006AAE8);
    func_8011EC68(D_8006AAE8);
    func_8012016C(D_8006AAE8);
    func_80039A94(D_8006AAE8);
    func_8002E874(D_8006AAE8);
    func_8002151C(D_8006AAE8);
    func_80124198(D_8006AAE8);
    func_8002DFF0(D_8006AAE8);
    func_8002C2CC(D_8006AAE8);
    func_8002DBBC(D_8006AAE8);
    func_80019080(D_8006AAE8);
    func_8002377C(D_8006AAE8);
    func_80029654(D_8006AAE8);
    func_8003F97C(D_8006AAE4);
    D_80076170 = D_8006AAE8;
    D_80076630 = D_8006AAE8;
    func_80026B50(D_8006AAE8);
    func_80026F10(D_8006AAE8);
    D_801286D4 = D_8006AAE8;
    func_80007D14();

    Actor* actor = D_8006AB10;
    func_800085B8(actor);
    while (arg0->unk54.unk0B4 != 0) {
        func_800BFF50(&arg0->unk54);
    }
    func_80008FA8(actor);
    func_80007CB8();
}

INCLUDE_ASM("asm/nonmatchings/8260", func_80007CB8);

INCLUDE_ASM("asm/nonmatchings/8260", func_80007D14);

extern "C" void func_80007D70(GameContext* arg0) {
    if (arg0->unk4C != NULL) {
        func_800092F8(arg0->unk4C);
        delete arg0->unk4C;
        arg0->unk4C = NULL;
    }
    arg0->unk48->vfunc10();
}

void GameContext::func_80007DE0() {
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

extern "C" void func_80007EC4(GameContext* arg0) {
    if (arg0->unk48->unk0 & 1) {
        arg0->func_80007DE0();
        func_80007A60(arg0);
        func_80007D70(arg0);
    }
}

extern "C" void func_80007F10(GameContext* arg0) {
    delete arg0->unk48;
    arg0->unk48 = NULL;
}

extern "C" s32 func_80007F54(GameContext* arg0, s32, u8**) {
    arg0->unk48 = (StructYY*)func_8000D0B0(func_8004B414(0x11AB0));
    if (arg0->unk48->unk0 & 1) {
        func_80007F10(arg0);
    }

    // TODO: migrate string
    arg0->unk48->vfunc2(D_800005CC, D_8006AAFC);
    osCreateMesgQueue(&arg0->unk14, &arg0->unk2C, 1);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/8260", func_80007FE0); // virt13

#ifdef NON_MATCHING
// Pending data migration

GameContext::GameContext() {
    D_8006AB04 = this;
    unk4 = D_8006AAF0;
    unk8 = D_8006AAF4;
    unkC = D_8006AAF8;
    unk48 = 0;
    this->unk54.unk0B0 = 1;
    unk50 = 0x58;
    this->unk54.unk0B4 = 1;
    this->unk54.unk0C8 = 1;
    this->unk54.unk0CC = 0;
    this->unk54.unk0D0 = 0;
    this->unk54.unk0D4 = 1;
    unk10 = 1;
    this->unk54.unk0B8 = 80; // D_80000668
    this->unk54.unk0BC = 4;  // D_8000066C
    this->unk54.unk0C0 = D_8006AB00;
}
#else
INCLUDE_ASM("asm/nonmatchings/8260", __11GameContext); // ctor
#endif

INCLUDE_ASM("asm/nonmatchings/8260", func_800080F8);

INCLUDE_ASM("asm/nonmatchings/8260", func_80008100);

INCLUDE_ASM("asm/nonmatchings/8260", func_8000812C);

INCLUDE_ASM("asm/nonmatchings/8260", func_80008138);

INCLUDE_ASM("asm/nonmatchings/8260", func_80008148);

INCLUDE_ASM("asm/nonmatchings/8260", func_80008158);

INCLUDE_ASM("asm/nonmatchings/8260", func_80008164);

INCLUDE_ASM("asm/nonmatchings/8260", func_80008174);

INCLUDE_ASM("asm/nonmatchings/8260", func_80008184);

INCLUDE_ASM("asm/nonmatchings/8260", func_80008194);

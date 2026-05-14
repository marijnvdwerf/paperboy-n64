#include "common.h"
#include "structs.h"
#include "game.h"

class Entry98 {
  public:
    Entry98();
    ~Entry98();
    /* 0x000 */ u8 pad0[0x13C];
    /* 0x13C */ u8 unk13C; // bit flags
    /* 0x13D */ u8 pad13D[3];
    /* 0x140 */ u32 unk140;
};

struct UnkD8620 {
    u8 pad0[0x240];
    u32 unk240;
};


class ObjA8 { // 0xC0
  public:
    ObjA8();
    ~ObjA8();
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ u32 unk20[0x1C];
    /* 0x90 */ u8 unk90[0x30];
};

class ObjA { // 0x284
  public:
    ObjA();
    ~ObjA();
    u8 _[0x284];
};

class ObjB { // 0xC24
  public:
    ObjB();
    ~ObjB();
    u8 _[0xC24];
};

class ObjC { // 0x100
  public:
    ObjC();
    ~ObjC();
    u8 _[0x100];
};

class ObjD { // 0x104
  public:
    ObjD();
    ~ObjD();
    u8 _[0x104];
};

class ObjE { // 0x658
  public:
    ObjE();
    ~ObjE();
    u8 _[0x658];
};

struct ObjF { // at D_801274A0; full size unknown
    /* 0x00 */ u8 pad0[0x1D];
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
};

// Base for D_80128FD8: 0x30 bytes of data, then a vptr at 0x30, then >= 54 virtuals.
// (Per SN cfront, vptr is placed after data members of the declaring class.)
class ObjDB8Base {
  public:
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ // vptr here

    virtual void vfunc1() = 0;
    virtual void vfunc2() = 0;
    virtual void vfunc3() = 0;
    virtual void vfunc4() = 0;
    virtual void vfunc5() = 0;
    virtual void vfunc6() = 0;
    virtual void vfunc7() = 0;
    virtual void vfunc8() = 0;
    virtual void vfunc9() = 0;
    virtual void vfunc10() = 0;
    virtual void vfunc11() = 0;
    virtual void vfunc12() = 0;
    virtual void vfunc13() = 0;
    virtual void vfunc14() = 0;
    virtual void vfunc15() = 0;
    virtual void vfunc16() = 0;
    virtual void vfunc17() = 0;
    virtual void vfunc18() = 0;
    virtual void vfunc19() = 0;
    virtual void vfunc20() = 0;
    virtual void vfunc21() = 0;
    virtual void vfunc22() = 0;
    virtual void vfunc23() = 0;
    virtual void vfunc24() = 0;
    virtual void vfunc25() = 0;
    virtual void vfunc26() = 0;
    virtual void vfunc27() = 0;
    virtual void vfunc28() = 0;
    virtual void vfunc29() = 0;
    virtual void vfunc30() = 0;
    virtual void vfunc31() = 0;
    virtual void vfunc32() = 0;
    virtual void vfunc33() = 0;
    virtual void vfunc34() = 0;
    virtual void vfunc35() = 0;
    virtual void vfunc36() = 0;
    virtual void vfunc37() = 0;
    virtual void vfunc38() = 0;
    virtual void vfunc39() = 0;
    virtual void vfunc40() = 0;
    virtual void vfunc41() = 0;
    virtual void vfunc42() = 0;
    virtual void vfunc43() = 0;
    virtual void vfunc44(s32) = 0; // slot 44 (vtable offset 0x160/0x164)
    virtual void vfunc45() = 0;
    virtual void vfunc46() = 0;
    virtual void vfunc47() = 0;
    virtual void vfunc48() = 0;
    virtual void vfunc49() = 0;
    virtual void vfunc50() = 0;
    virtual void vfunc51() = 0;
    virtual void vfunc52() = 0;
    virtual void vfunc53() = 0;
    virtual void vfunc54(ObjDB8Base*) = 0; // slot 54 (vtable offset 0x1B0/0x1B4)
};

class ObjDB8 : public ObjDB8Base {
  public:
    /* 0x034 */ u8 pad34[0x1D8 - 0x34];
    /* 0x1D8 */ u32 unk1D8;
};

struct Pkt12 {
    ObjDB8* a;
    ObjDB8* b;
    s32 c;
    Pkt12(ObjDB8* aa, ObjDB8* bb, s32 cc) : a(aa), b(bb), c(cc) {}
};

extern char D_80000730[];        // "PLAYER"
extern char D_80000738[];        // "\\GameData\\nothing.txt"
extern char D_80000750[];        // "GAMEDATA"
extern char D_8000075C[];        // "IHLT"
extern char D_80000764[];        // "GAMEEND"
extern s32 D_80076D20;
extern ObjF* D_801274A0;
extern s32 D_801274D0;
extern s32 D_80127304;
extern void* D_80127330;
extern s32 D_80127950;
extern s32 D_80127AD0;
extern UnkD8620* D_80128620;
extern ObjDB8* D_80128FD8;

extern "C" char* strcpy(char*, const char*);
extern "C" s32 func_800431D0(char*);
extern "C" void func_8004B848(s32, s32);
extern "C" void func_8004B878(s32, s32);
extern "C" void func_800CAFA0(Entry98*);
extern "C" void func_800CB8BC(ObjC*);
extern "C" void func_800CB8D8(ObjC*, char*);
extern "C" s32 func_800CB930(s32);
extern "C" void func_800CB950(s32, Pkt12, s32);
extern "C" s32 func_800CCD18(void*);
extern "C" void func_800CE410(ObjF*);
extern "C" void func_800CE81C(ObjF*, s32);
extern "C" void func_800CE8AC(ObjF*, s32);
extern "C" void func_800CE8E0(ObjF*, char*);
extern "C" void func_800CE944(u8*);
extern "C" void func_800CEA3C(ObjF*, s32, s32);
extern "C" void func_800CED50(s32);
extern "C" void func_800DA3E8(s32);
extern "C" void func_800DA6A8(ObjD*);
extern "C" void func_800DA720(ObjD*);
extern "C" s32 func_800DA834(s32);
extern "C" void func_800DBE1C(s32, s8, s8, char*);
extern "C" void func_800E1A08(ObjE*);
extern "C" void func_800E38A0(ObjE*);
extern "C" s32 func_800E3F40(s32);
extern "C" void func_800E686C(ObjB*);
extern "C" void func_800E68C4(ObjB*);
extern "C" s32 func_800E69AC(s32);
extern "C" void func_800E8438(ObjA*);
extern "C" void func_800E8440(ObjA*);
extern "C" s32 func_800E84B4(s32);
extern "C" s32 func_80106110(void*);

#if 0
GameSubContext::GameSubContext() {
    unk0 = 7;
    unk4 = 7;
    unk8 = 0;
    unkC = 0;
    unk10 = 0;
    unk14 = 0;
    unk1C = 0;
    unk20 = 0;
    unk94 = 0;
    unk98 = NULL;
    unk9C = 0;
    unkA0 = 0;
    unkA4 = 0;
    unkAC = 0;
    unkC4 = 0;
    unkD8 = 0;
    unkDC = 0;
    unkE0 = 0;
    unkE4 = 1;
    unkE8 = 1;
    unkEC = 1;
    unkF0 = 0;
    unkF4 = 0;
    unkF8 = 0;
    unk100 = 0;
    unk11C = 1;
    unk120 = 0;
    unk124 = 0;
    unk128 = 0;
    unk130 = 0;
    unk134 = 1;
    unk138 = 1;
    unk13C = 0;
    unk154 = 1;
    unk5B0 = 1;
    unk5D0 = -1;
    unk5EC = -1;
    D_8006AB10 = this;
    unkFF = 0x64;
    unk158 = 0;
    unk160 = 0;
    unk178 = 0;
    unk5B4 = 0;
    unk5B8 = 0;
    unk5BC = 0;
    unk5C0 = 0;
    unk5C4 = 0;
    unk5C8 = 0;
    unk5CC = 0;
    unk5D4 = 0;
    unk5D8 = 0;
    unk5DC = 0;
    unk5E0 = 0;
    unk5E4 = 0;
    unk5E8 = 0;
    unk5F8 = 0;
    unkFD = 3;
    func_80008EAC(this);
    func_80008EA0(this);
    unk188 = 0;
    unk194 = 0;
    unk190 = 0;
    unk18C = 0;
    unk1AC = 0;
    unk1A8 = 0;
    unk1A4 = 0;
    strcpy(unk1B0, "TITLES");
    strcpy(unk110, "NULL");
    unk140[0] = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/8EB0", __14GameSubContext);
#endif

void GameSubContext::func_8000843C() {
    delete[] unk98;
    unk98 = new Entry98[0x1C];
    // TODO: research to make this loop more natural
    u8 j = 0;
    while (1) {
        if (j >= 0x1C) break;
        unkA8->unk20[j] = 0;
        j++;
    }
    func_80008B88();
    func_80008938();
    func_800CE944(unkA8->unk90);
    ObjF* d = D_801274A0;
    func_800CE8AC(d, 0);
    func_800CE410(d);
    func_800CE81C(d, 0);
    func_800CE8E0(d, D_80000730);
    func_800CEA3C(d, 0, -1);
    d->unk1E = 0x2D;
    d->unk1D = 0x32;
    func_800DA3E8(D_80127950);
}

void GameSubContext::func_800085B8() {
    unk98 = new Entry98[0x1C];
    unk108 = 1;
    unk10C = 0;
    unkA8 = new ObjA8();
    char* filename = D_80000738;
    LocalIO2 sp10;
    if (func_800431D0(filename) != 8) {
        sp10.virt8(filename, 0x8A, 0x20);
        sp10.virt11(unk140, 0xC);
        sp10.virt9();
        for (s32 i = 0; i < 0xC; i++) {
            if (unk140[i] == 0) break;
            if (unk140[i] == '/') {
                unk140[i] = '-';
            }
        }
    }
    for (u8 j = 0; j < 0x1C; j++) {
        unk24[j] = 0;
    }
    unk24[0] = 1;
    unk24[1] = 1;
    unk24[3] = 1;
}

INCLUDE_ASM("asm/nonmatchings/8EB0", func_8000872C__14GameSubContextl);

void GameSubContext::func_8000884C(s32 arg1) {
    s32 skip = 0;
    if (func_80106110(D_80128FD8) != 0 || D_80128620->unk240 != 0) {
        skip = 1;
    }
    if (skip == 0 && D_80128FD8->unk1D8 == 0) {
        s32 cap = unkFF;
        if (cap < 0) {
            cap += 3;
        }
        if ((cap >> 2) >= unkFE) {
            func_8004B878(D_80076D20, 8);
        }
        if (unkFE > 0) {
            unkFE = unkFE - arg1;
            if (unkFE <= 0) {
                unkFE = 0;
                func_80008DB8();
            }
            func_800DBE1C(D_80127AD0, unkFE, unkFF, D_8000075C);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/8EB0", func_80008938__14GameSubContext);

void GameSubContext::func_80008B88() {
    s8 i;
    for (i = 0; i < 0x1C; i++) {
        unk24[i] = 0;
    }
    unk24[0] = 1;
    unk24[1] = 1;
    unk24[3] = 1;
}

void GameSubContext::func_80008BCC() {
    if (unkE4 == 0x1B && func_800CCD18(D_80127330) != 0 && unk5B4 == 0) {
        unk4 = 6;
        unk5B0 = 1;
        strcpy(unk1B0, D_80000764);
        unk178 = 4;
    } else {
        unk4 = 1;
    }
    unk100 = 1;
}

void GameSubContext::func_80008C58() {
    s32 tmp = unk4;
    unk4 = 0;
    unk0 = tmp;
}

s32 GameSubContext::func_80008C68() {
    return unk4;
}

s32 GameSubContext::func_80008C74() {
    return unk0;
}

void GameSubContext::func_80008C80(s32 val) {
    unk4 = val;
}

void GameSubContext::func_80008C88() {
    unk20 = 1;
}

void GameSubContext::func_80008C94() {
    unk10 = unk14;
}

void GameSubContext::func_80008CA0() {
    unk14 = 1;
    unk10 = 1;
    func_80008C88();
}

void GameSubContext::func_80008CC4() {
    unk10 = 1;
    func_80008C88();
}

void GameSubContext::func_80008CE4(s8 bit) {
    if (unk168 == 0 || unk164 == 0) {
        unk98[unkE4].unk13C |= (1 << bit);
    }
}

s32 GameSubContext::func_80008D3C(s8 bit) {
    return unk98[unkE4].unk13C & (1 << bit);
}

s32 GameSubContext::func_80008D78(s8 bit, s8 idx) {
    return unk98[idx].unk13C & (1 << bit);
}

void GameSubContext::func_80008DB8() {
    ObjDB8* obj = D_80128FD8;
    obj->vfunc44(0xCD);
    obj->vfunc54(obj);
    Pkt12 p(obj, obj, 0x2E);
    s32 arg1 = D_80127304;
    func_800CB950(arg1, p, 1);
    if (unk104 != 0) {
        func_800CED50(D_801274D0);
    }
}

void GameSubContext::func_80008E80(s32 delta) {
    unkFC = unkFC + delta;
}

void GameSubContext::func_80008E90() {
    unkFC = unkFC + 1;
}

void GameSubContext::func_80008EA0() {
    unkFC = unkFD;
}

void GameSubContext::func_80008EAC() {
    unkFE = unkFF;
}

void GameSubContext::func_80008EB8(s32 delta) {
    s32 cap = unkFF;
    unkFE = unkFE + delta;
    if (cap < 0) {
        cap += 3;
    }
    if (unkFE > (cap >> 2)) {
        func_8004B848(D_80076D20, 8);
    }
    if (unkFE > unkFF) {
        unkFE = unkFF;
    }
    func_800DBE1C(D_80127AD0, unkFE, unkFF, D_8000075C);
}

void GameSubContext::func_80008F48(s32 val) {
    unk158 = val;
}

u8 GameSubContext::func_80008F50(u8 idx) {
    u8 bits = unk98[idx].unk13C;
    u8 count = 0;
    for (u8 i = 0; i < 8; i++) {
        if ((bits >> i) & 1) {
            count++;
        }
    }
    return count;
}

void GameSubContext::func_80008FA8() {
    delete[] unk98;
    func_800E8438(unk9C);
    delete unk9C;
    func_800E686C(unkA0);
    delete unkA0;
    func_800DA6A8(unkA4);
    delete unkA4;
    delete unkA8;
    func_800E38A0(unkAC);
    delete unkAC;
}

void GameSubContext::func_800090BC() {
    if (unk9C == NULL) {
        unk9C = new ObjA();
        func_800E8440(unk9C);
    }
    if (unkA0 == NULL) {
        unkA0 = new ObjB();
        func_800E68C4(unkA0);
    }
    ObjC* tmp = new ObjC();
    func_800CB8D8(tmp, D_80000750);
    if (unkA4 == NULL) {
        unkA4 = new ObjD();
        func_800DA720(unkA4);
    }
    func_800CB8BC(tmp);
    delete tmp;
    if (unkAC == NULL) {
        unkAC = new ObjE();
        func_800E1A08(unkAC);
    }
}

GameSubContext::~GameSubContext() {
    D_8006AB10 = NULL;
}

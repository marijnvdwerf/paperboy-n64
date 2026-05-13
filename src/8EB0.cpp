#include "common.h"
#include "structs.h"

struct Entry98;

class GameSubContext {
  public:
    GameSubContext();
    ~GameSubContext();

    /* 0x000 */ u32 unk0;
    /* 0x004 */ u32 unk4;
    /* 0x008 */ u32 unk8;
    /* 0x00C */ u32 unkC;
    /* 0x010 */ u32 unk10;
    /* 0x014 */ u32 unk14;
    /* 0x018 */ u32 unk18;
    /* 0x01C */ u32 unk1C;
    /* 0x020 */ u32 unk20;
    /* 0x024 */ u32 unk24[0x1C]; // 0x24..0x94
    /* 0x094 */ u32 unk94;
    /* 0x098 */ Entry98* unk98;
    /* 0x09C */ class ObjA* unk9C;
    /* 0x0A0 */ class ObjB* unkA0;
    /* 0x0A4 */ class ObjD* unkA4;
    /* 0x0A8 */ class ObjA8* unkA8;
    /* 0x0AC */ class ObjE* unkAC;
    /* 0x0B0 */ u8 padB0[0x14];
    /* 0x0C4 */ u32 unkC4;
    /* 0x0C8 */ u8 padC8[0x10];
    /* 0x0D8 */ u32 unkD8;
    /* 0x0DC */ u32 unkDC;
    /* 0x0E0 */ u32 unkE0;
    /* 0x0E4 */ u32 unkE4;
    /* 0x0E8 */ u32 unkE8;
    /* 0x0EC */ u32 unkEC;
    /* 0x0F0 */ u32 unkF0;
    /* 0x0F4 */ u8 unkF4;
    /* 0x0F5 */ u8 padF5[3];
    /* 0x0F8 */ u32 unkF8;
    /* 0x0FC */ u8 unkFC;
    /* 0x0FD */ u8 unkFD;
    /* 0x0FE */ s8 unkFE;
    /* 0x0FF */ s8 unkFF;
    /* 0x100 */ u32 unk100;
    /* 0x104 */ u32 unk104;
    /* 0x108 */ u32 unk108;
    /* 0x10C */ u32 unk10C;
    /* 0x110 */ char unk110[5]; // "NULL"
    /* 0x115 */ u8 pad115[7];
    /* 0x11C */ u32 unk11C;
    /* 0x120 */ u32 unk120;
    /* 0x124 */ u32 unk124;
    /* 0x128 */ u32 unk128;
    /* 0x12C */ u32 pad12C;
    /* 0x130 */ u32 unk130;
    /* 0x134 */ u32 unk134;
    /* 0x138 */ u32 unk138;
    /* 0x13C */ u32 unk13C;
    /* 0x140 */ u8 unk140[20]; // 0x140..0x154 (12 byte file read + padding)
    /* 0x154 */ u32 unk154;
    /* 0x158 */ u32 unk158;
    /* 0x15C */ u32 pad15C;
    /* 0x160 */ u32 unk160;
    /* 0x164 */ u32 unk164;
    /* 0x168 */ u32 unk168;
    /* 0x16C */ u8 pad16C[12];
    /* 0x178 */ u32 unk178;
    /* 0x17C */ u8 pad17C[12];
    /* 0x188 */ u32 unk188;
    /* 0x18C */ u32 unk18C;
    /* 0x190 */ u32 unk190;
    /* 0x194 */ u32 unk194;
    /* 0x198 */ u8 pad198[12];
    /* 0x1A4 */ u32 unk1A4;
    /* 0x1A8 */ u32 unk1A8;
    /* 0x1AC */ u32 unk1AC;
    /* 0x1B0 */ char unk1B0[7]; // "TITLES"
    /* 0x1B7 */ u8 pad1B7;
    /* 0x1B8 */ u8 pad1B8[0x3F8];
    /* 0x5B0 */ u32 unk5B0;
    /* 0x5B4 */ u32 unk5B4;
    /* 0x5B8 */ u32 unk5B8;
    /* 0x5BC */ u32 unk5BC;
    /* 0x5C0 */ u32 unk5C0;
    /* 0x5C4 */ u32 unk5C4;
    /* 0x5C8 */ u32 unk5C8;
    /* 0x5CC */ u32 unk5CC;
    /* 0x5D0 */ s32 unk5D0;
    /* 0x5D4 */ u32 unk5D4;
    /* 0x5D8 */ u32 unk5D8;
    /* 0x5DC */ u32 unk5DC;
    /* 0x5E0 */ u32 unk5E0;
    /* 0x5E4 */ u32 unk5E4;
    /* 0x5E8 */ u32 unk5E8;
    /* 0x5EC */ s32 unk5EC;
    /* 0x5F0 */ u32 pad5F0;
    /* 0x5F4 */ u32 pad5F4;
    /* 0x5F8 */ u32 unk5F8;
};

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

// Data (sorted by address)
extern char D_80000730[];        // "PLAYER"
extern char D_80000738[];        // "\\GameData\\nothing.txt"
extern char D_80000750[];        // "GAMEDATA"
extern char D_8000075C[];        // "IHLT"
extern char D_80000764[];        // "GAMEEND"
extern s32 D_80076D20;
extern GameSubContext* D_8006AB10;
extern ObjF* D_801274A0;
extern s32 D_801274D0;
extern s32 D_80127304;
extern void* D_80127330;
extern s32 D_80127950;
extern s32 D_80127AD0;
extern UnkD8620* D_80128620;
extern ObjDB8* D_80128FD8;

// Runtime/intrinsics
extern "C" void* __builtin_new(u32);
extern "C" void* __builtin_vec_new(u32);
extern "C" void __builtin_vec_delete(void*);
extern "C" char* strcpy(char*, const char*);

// External functions (sorted by address)
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

// Forward declarations for functions defined in this TU (called before their definition)
extern "C" void func_80008938(GameSubContext*);
extern "C" void func_80008DB8(GameSubContext*);
extern "C" void func_80008EA0(GameSubContext*);
extern "C" void func_80008EAC(GameSubContext*);
extern "C" void func_80008B88(GameSubContext*);

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

extern "C" void func_8000843C(GameSubContext* sc) {
    delete[] sc->unk98;
    sc->unk98 = new Entry98[0x1C];
    // TODO: research to make this loop more natural
    u8 j = 0;
    while (1) {
        if (j >= 0x1C) break;
        sc->unkA8->unk20[j] = 0;
        j++;
    }
    func_80008B88(sc);
    func_80008938(sc);
    func_800CE944(sc->unkA8->unk90);
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

extern "C" void func_800085B8(GameSubContext* sc) {
    sc->unk98 = new Entry98[0x1C];
    sc->unk108 = 1;
    sc->unk10C = 0;
    sc->unkA8 = new ObjA8();
    char* filename = D_80000738;
    LocalIO2 sp10;
    if (func_800431D0(filename) != 8) {
        sp10.virt8(filename, 0x8A, 0x20);
        sp10.virt11(sc->unk140, 0xC);
        sp10.virt9();
        for (s32 i = 0; i < 0xC; i++) {
            if (sc->unk140[i] == 0) break;
            if (sc->unk140[i] == '/') {
                sc->unk140[i] = '-';
            }
        }
    }
    for (u8 j = 0; j < 0x1C; j++) {
        sc->unk24[j] = 0;
    }
    sc->unk24[0] = 1;
    sc->unk24[1] = 1;
    sc->unk24[3] = 1;
}

INCLUDE_ASM("asm/nonmatchings/8EB0", func_8000872C);

extern "C" void func_8000884C(GameSubContext* sc, s32 arg1) {
    s32 skip = 0;
    if (func_80106110(D_80128FD8) != 0 || D_80128620->unk240 != 0) {
        skip = 1;
    }
    if (skip == 0 && D_80128FD8->unk1D8 == 0) {
        s32 cap = sc->unkFF;
        if (cap < 0) {
            cap += 3;
        }
        if ((cap >> 2) >= sc->unkFE) {
            func_8004B878(D_80076D20, 8);
        }
        if (sc->unkFE > 0) {
            sc->unkFE = sc->unkFE - arg1;
            if (sc->unkFE <= 0) {
                sc->unkFE = 0;
                func_80008DB8(sc);
            }
            func_800DBE1C(D_80127AD0, sc->unkFE, sc->unkFF, D_8000075C);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/8EB0", func_80008938);

extern "C" void func_80008B88(GameSubContext* sc) {
    s8 i;
    for (i = 0; i < 0x1C; i++) {
        sc->unk24[i] = 0;
    }
    sc->unk24[0] = 1;
    sc->unk24[1] = 1;
    sc->unk24[3] = 1;
}

extern "C" void func_80008BCC(GameSubContext* sc) {
    if (sc->unkE4 == 0x1B && func_800CCD18(D_80127330) != 0 && sc->unk5B4 == 0) {
        sc->unk4 = 6;
        sc->unk5B0 = 1;
        strcpy(sc->unk1B0, D_80000764);
        sc->unk178 = 4;
    } else {
        sc->unk4 = 1;
    }
    sc->unk100 = 1;
}

extern "C" void func_80008C58(GameSubContext* sc) {
    s32 tmp = sc->unk4;
    sc->unk4 = 0;
    sc->unk0 = tmp;
}

extern "C" s32 func_80008C68(GameSubContext* sc) {
    return sc->unk4;
}

extern "C" s32 func_80008C74(s32* p) {
    return *p;
}

extern "C" void func_80008C80(GameSubContext* sc, s32 val) {
    sc->unk4 = val;
}

extern "C" void func_80008C88(GameSubContext* sc) {
    sc->unk20 = 1;
}

extern "C" void func_80008C94(GameSubContext* sc) {
    sc->unk10 = sc->unk14;
}

extern "C" void func_80008CA0(GameSubContext* sc) {
    sc->unk14 = 1;
    sc->unk10 = 1;
    func_80008C88(sc);
}

extern "C" void func_80008CC4(GameSubContext* sc) {
    sc->unk10 = 1;
    func_80008C88(sc);
}

extern "C" void func_80008CE4(GameSubContext* sc, s8 bit) {
    if (sc->unk168 == 0 || sc->unk164 == 0) {
        sc->unk98[sc->unkE4].unk13C |= (1 << bit);
    }
}

extern "C" s32 func_80008D3C(GameSubContext* sc, s8 bit) {
    return sc->unk98[sc->unkE4].unk13C & (1 << bit);
}

extern "C" s32 func_80008D78(GameSubContext* sc, s8 bit, s8 idx) {
    return sc->unk98[idx].unk13C & (1 << bit);
}

extern "C" void func_80008DB8(GameSubContext* sc) {
    ObjDB8* obj = D_80128FD8;
    obj->vfunc44(0xCD);
    obj->vfunc54(obj);
    Pkt12 p(obj, obj, 0x2E);
    s32 arg1 = D_80127304;
    func_800CB950(arg1, p, 1);
    if (sc->unk104 != 0) {
        func_800CED50(D_801274D0);
    }
}

extern "C" void func_80008E80(GameSubContext* sc, s32 delta) {
    sc->unkFC = sc->unkFC + delta;
}

extern "C" void func_80008E90(GameSubContext* sc) {
    sc->unkFC = sc->unkFC + 1;
}

extern "C" void func_80008EA0(GameSubContext* sc) {
    sc->unkFC = sc->unkFD;
}

extern "C" void func_80008EAC(GameSubContext* sc) {
    sc->unkFE = sc->unkFF;
}

extern "C" void func_80008EB8(GameSubContext* sc, s32 delta) {
    s32 cap = sc->unkFF;
    sc->unkFE = sc->unkFE + delta;
    if (cap < 0) {
        cap += 3;
    }
    if (sc->unkFE > (cap >> 2)) {
        func_8004B848(D_80076D20, 8);
    }
    if (sc->unkFE > sc->unkFF) {
        sc->unkFE = sc->unkFF;
    }
    func_800DBE1C(D_80127AD0, sc->unkFE, sc->unkFF, D_8000075C);
}

extern "C" void func_80008F48(GameSubContext* sc, s32 val) {
    sc->unk158 = val;
}

extern "C" u8 func_80008F50(GameSubContext* sc, u8 idx) {
    u8 bits = sc->unk98[idx].unk13C;
    u8 count = 0;
    for (u8 i = 0; i < 8; i++) {
        if ((bits >> i) & 1) {
            count++;
        }
    }
    return count;
}

extern "C" void func_80008FA8(GameSubContext* sc) {
    delete[] sc->unk98;
    func_800E8438(sc->unk9C);
    delete sc->unk9C;
    func_800E686C(sc->unkA0);
    delete sc->unkA0;
    func_800DA6A8(sc->unkA4);
    delete sc->unkA4;
    delete sc->unkA8;
    func_800E38A0(sc->unkAC);
    delete sc->unkAC;
}

extern "C" void func_800090BC(GameSubContext* sc) {
    if (sc->unk9C == NULL) {
        sc->unk9C = new ObjA();
        func_800E8440(sc->unk9C);
    }
    if (sc->unkA0 == NULL) {
        sc->unkA0 = new ObjB();
        func_800E68C4(sc->unkA0);
    }
    ObjC* tmp = new ObjC();
    func_800CB8D8(tmp, D_80000750);
    if (sc->unkA4 == NULL) {
        sc->unkA4 = new ObjD();
        func_800DA720(sc->unkA4);
    }
    func_800CB8BC(tmp);
    delete tmp;
    if (sc->unkAC == NULL) {
        sc->unkAC = new ObjE();
        func_800E1A08(sc->unkAC);
    }
}

GameSubContext::~GameSubContext() {
    D_8006AB10 = NULL;
}

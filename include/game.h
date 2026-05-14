#ifndef GAME_H
#define GAME_H

#include "common.h"

// Forward declarations of file-local types used by GameSubContext as pointers
class Entry98;
class ObjA;
class ObjB;
class ObjD;
class ObjA8;
class ObjE;

class GameSubContext {
  public:
    GameSubContext();
    ~GameSubContext();

    void func_8000843C();
    void func_800085B8();
    u32 func_8000872C(s32);
    void func_8000884C(s32);
    void func_80008938();
    void func_80008B88();
    void func_80008BCC();
    void func_80008C58();
    s32 func_80008C68();
    s32 func_80008C74();
    void func_80008C80(s32);
    void func_80008C88();
    void func_80008C94();
    void func_80008CA0();
    void func_80008CC4();
    void func_80008CE4(s8);
    s32 func_80008D3C(s8);
    s32 func_80008D78(s8, s8);
    void func_80008DB8();
    void func_80008E80(s32);
    void func_80008E90();
    void func_80008EA0();
    void func_80008EAC();
    void func_80008EB8(s32);
    void func_80008F48(s32);
    u8 func_80008F50(u8);
    void func_80008FA8();
    void func_800090BC();

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
    /* 0x09C */ ObjA* unk9C;
    /* 0x0A0 */ ObjB* unkA0;
    /* 0x0A4 */ ObjD* unkA4;
    /* 0x0A8 */ ObjA8* unkA8;
    /* 0x0AC */ ObjE* unkAC;
    /* 0x0B0 */ s32 unkB0;
    /* 0x0B4 */ s32 unkB4;
    /* 0x0B8 */ f32 unkB8;
    /* 0x0BC */ f32 unkBC;
    /* 0x0C0 */ f32 unkC0;
    /* 0x0C4 */ u32 unkC4;
    /* 0x0C8 */ s32 unkC8;
    /* 0x0CC */ s32 unkCC;
    /* 0x0D0 */ s32 unkD0;
    /* 0x0D4 */ s32 unkD4;
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

extern GameSubContext* D_8006AB10;

#endif

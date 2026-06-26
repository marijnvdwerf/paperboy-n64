#ifndef PEACOCK_H
#define PEACOCK_H

#include "common.h"

struct Color4 {
    char c[4];
};

struct PeacockLinked {
    /* 0x00 */ char pad[0x34];
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u16 unk36;
};

struct PeacockExport {
    /* 0x00 */ u32 flags;
    /* 0x04 */ PeacockLinked* linked;
    /* 0x08 */ Color4 color2;
    /* 0x0C */ Color4 color1;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
};

struct PeacockContext {
    /* 0x00 */ u32 contextFlags;
};

class Peacock {
  public:
    /* 0x00 */ PeacockLinked* linked;
    /* 0x04 */ u32 flags;
    /* 0x08 */ Color4 color1;
    /* 0x0C */ Color4 color2;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ char unk14[8];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    // vptr at 0x20

    Peacock();
    virtual ~Peacock();

    void func_80020F40(PeacockExport* dst);
    void func_80020FA4(PeacockContext* ctx, PeacockExport* src);
    u32 func_80021148();
    u32 func_80021158();
    u32 func_80021168();
    void func_80021180(PeacockLinked* linked);
    PeacockLinked* func_80021188();
    u32 func_80021194();
    s32 func_800211A4();
    void func_800211B0(s32 val);
    u32 func_800211C8();
    void func_800211D4(u8* src);
    u32 func_800211E0();
    void func_800211EC(char* dst);
    void func_80021214(char* src);
    u32 func_8002123C();
    u32 func_80021248();
    u32 func_80021254();
    u32 func_80021260();
    u32 func_8002126C();
    u32 func_80021278();
    u32 func_80021284();
    u32 func_80021290();
    u32 func_8002129C();
    u32 func_800212AC();
    u32 func_800212BC();
    u32 func_800212C8();
    u32 func_800212D4();
    u32 func_800212E0();
    u32 func_800212EC();
    u32 func_800212F8();
    u32 func_80021304();
    u32 func_80021310();
    u32 func_80021320();
    u32 func_8002132C();
    void func_80021338(char* dst);
    void func_80021350(char* dst);
    u32 func_80021368();
    u32 func_80021374();
};

#endif

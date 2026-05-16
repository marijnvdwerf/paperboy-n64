#ifndef PARROT_H
#define PARROT_H

#include "common.h"
#include "structs.h"

struct ParrotDriver {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ u32 dataStart;
    /* 0x1C */ u32 dataEnd;
    /* 0x20 */ u8* data;
};

struct Parrot : public RomFile {
    /* 0x030 */ s32 pushedBack;
    /* 0x034 */ s32 currentType;
    /* 0x038 */ s32 intValue;
    /* 0x03C */ f32 unk3C;
    /* 0x040 */ f32 floatValue;
    /* 0x044 */ u8 unk44[0x40];
    /* 0x084 */ u8 unk84[0x20];
    /* 0x0A4 */ char pathBuf[0x100];
    /* 0x1A4 */ char* unk1A4;
    /* 0x1A8 */ char inlineBuf[0x40];
    /* 0x1E8 */ s32 cursor;
    /* 0x1EC */ char extension[5];
    /* 0x1F4 */ s32 repeatCount;
    /* 0x1F8 */ s32 repeatType;
    /* 0x1FC */ s32 inArray;
    /* 0x200 */ s32 inStruct;
    /* 0x204 */ s32 structId;
    /* 0x208 */ u32 structPos;
    /* 0x20C */ u32 structDefs[16][16];
    /* 0x60C */ u32 structLengths[0x10];
    /* 0x64C */ ParrotDriver* driver;

    Parrot();
    virtual ~Parrot();
    virtual s32 close();
    virtual void selectDriver(const char* path);
    virtual char* getExtension();
    virtual void vfunc19(s32 arg1) = 0;
    virtual s32 vfunc20();
    virtual s32 vfunc21();

    void init();
    void expectToken(s32 arg1);
    s32 func_80046570();
    s32 func_800465C4();
    void setExtension(const char* src);
    void func_80046700(s32 arg1);
    const char* func_800467B8(s32 idx);
    f32 readFloat();
    s32 readInt();
    u8* func_8004698C();
    s32 getInt();
    f32 getFloat();
    f32 func_800469AC();
    s32 pushBack();
    s32 getCurrentType();
    char* func_800469D0();
    u8* func_800469DC();
    u8* func_80046A14();
    void expectCloseBrace();
    void expectOpenBrace();
    void expectCloseBracket();
    void expectOpenBracket();
    void readToken(s32 arg1);
};

#endif

#ifndef PARROT_H
#define PARROT_H

#include "common.h"
#include "file.h"

enum ParrotToken {
    TOKEN_STRING = 0x02,
    TOKEN_FLOAT = 0x03,
    TOKEN_INT = 0x04,
    TOKEN_OPEN_BRACE = 0x05,
    TOKEN_CLOSE_BRACE = 0x06,
    TOKEN_OPEN_BRACKET = 0x07,
    TOKEN_CLOSE_BRACKET = 0x08,
    TOKEN_SBYTE = 0x0B,
    TOKEN_BYTE = 0x0C,
    TOKEN_SHORT = 0x0D,
    TOKEN_USHORT = 0x0E,
    TOKEN_FIXED_4096 = 0x0F, // s16 * (1/4096)
    TOKEN_FIXED_32 = 0x10, // s16 * (1/32)
    TOKEN_SHORT_F = 0x11, // s16 as float
    TOKEN_NORM_BYTE = 0x12, // byte * (1/127)
    TOKEN_EXT = 0x13, // 16-bit extended token follows
    TOKEN_REPEAT = 0x14,
    TOKEN_ARRAY = 0x15,
    TOKEN_STRUCT_DEF = 0x16,
    TOKEN_STRUCT_BASE = 0x17 // user struct IDs are 0x17..0x26
};

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
    /* 0x044 */ u8 stringValue[0x40];
    /* 0x084 */ u8 unk84[0x20];
    /* 0x0A4 */ u8 readBuf[0x100];
    /* 0x1A4 */ char* path;
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
    virtual void parseError(s32 code) = 0;
    virtual s32 nextToken();
    virtual s32 isStreaming();

    void init();
    void expectToken(s32 arg1);
    s32 beginArray();
    s32 nextRepeatedToken();
    void setExtension(const char* src);
    void ioError(s32 err);
    const char* errorMessage(s32 code);
    f32 readFloat();
    s32 readInt();
    u8* func_8004698C();
    s32 getInt();
    f32 getFloat();
    f32 func_800469AC();
    s32 pushBack();
    s32 getCurrentType();
    char* getPath();
    u8* func_800469DC();
    u8* func_80046A14();
    void expectCloseBrace();
    void expectOpenBrace();
    void expectCloseBracket();
    void expectOpenBracket();
    void readToken(s32 arg1);
};

#endif

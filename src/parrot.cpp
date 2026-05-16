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
    virtual void selectDriver();
    virtual char* getExtension();
    virtual void vfunc19() = 0;
    virtual s32 func_8004667C();
    virtual s32 func_800465BC();

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

extern "C" {
extern char* strncpy(char*, const char*, s32);
extern unsigned strlen(const char*);
extern char* strcat(char*, const char*);
extern int sprintf(char*, const char*, ...);
extern char D_80004C10[];
extern char D_80004C14[];
extern void* D_800763F8;
extern char* D_80076640;
extern const char* D_80076644[];
extern s32 func_800079A8(const char*, s32, s32, s32);
}

enum Token {
    TOKEN_FLOAT = 3,
    TOKEN_INT = 4,
    TOKEN_OPEN_BRACE = 5,
    TOKEN_CLOSE_BRACE = 6,
    TOKEN_OPEN_BRACKET = 7,
    TOKEN_CLOSE_BRACKET = 8,
    TOKEN_SBYTE = 11,
    TOKEN_BYTE = 12,
    TOKEN_SHORT = 13,
    TOKEN_USHORT = 14,
    TOKEN_FIXED_4096 = 15, // s16 * (1/4096)
    TOKEN_FIXED_32 = 16, // s16 * (1/32)
    TOKEN_SHORT_F = 17, // s16 as float
    TOKEN_NORM_BYTE = 18, // byte * (1/127)
};

const u32 D_80004D50[] = { TOKEN_OPEN_BRACKET, TOKEN_INT, TOKEN_CLOSE_BRACKET, TOKEN_CLOSE_BRACKET };

INCLUDE_RODATA("asm/nonmatchings/parrot", D_80004A80);

INCLUDE_RODATA("asm/nonmatchings/parrot", D_80004C10);

INCLUDE_RODATA("asm/nonmatchings/parrot", D_80004C14);

static inline s32 nextRepeatedToken(Parrot* self) {
    s32 ret;
    if (self->inStruct) {
        u32 idx = self->structPos;
        ret = self->structDefs[self->structId][idx];
        idx++;
        self->structPos = idx;
        if (idx < self->structLengths[self->structId]) {
            return ret;
        }
        self->structPos = 0;
    } else if (self->inArray) {
        u32 idx = self->structPos;
        ret = D_80004D50[idx];
        idx++;
        self->structPos = idx;
        if (idx < 4u) {
            return ret;
        }
        self->structPos = 0;
    } else {
        ret = self->repeatType;
    }
    self->repeatCount--;
    if (self->repeatCount == 0) {
        self->inStruct = 0;
        self->inArray = 0;
    }
    return ret;
}

#if 1
f32 Parrot::readFloat() {
    if (this->pushedBack != 0) {
        this->pushedBack = 0;
        return this->floatValue;
    }
    s32 cur1E8 = this->cursor;
    ParrotDriver* driver = this->driver;
    u32 total = cur1E8 + this->fileOffset;
    u32 base = driver->dataStart;
    s32 raw;
    if (total >= base && driver->dataEnd >= total + 5) {
        s32 pos = total - base;
        s32 op;
        if (this->repeatCount != 0) {
            op = nextRepeatedToken(this);
        } else {
            op = this->driver->data[pos];
            pos++;
            this->cursor = cur1E8 + 1;
        }
        this->currentType = TOKEN_FLOAT;
        switch (op) {
            case TOKEN_FLOAT: {
                raw = this->driver->data[pos] + (this->driver->data[pos + 1] << 8) +
                      (this->driver->data[pos + 2] << 16) + (this->driver->data[pos + 3] << 24);
                f32 fval = *(f32*)&raw;
                this->cursor += 4;
                this->floatValue = fval;
                return fval;
            }
            case TOKEN_FIXED_4096: {
                f32 fval =
                    (f32)(s16)(this->driver->data[pos] + (this->driver->data[pos + 1] << 8)) * 0.000244140625f;
                this->cursor += 2;
                this->floatValue = fval;
                return fval;
            }
            case TOKEN_FIXED_32: {
                f32 fval =
                    (f32)(s16)(this->driver->data[pos] + (this->driver->data[pos + 1] << 8)) * 0.03125f;
                this->cursor += 2;
                this->floatValue = fval;
                return fval;
            }
            case TOKEN_SHORT_F: {
                f32 fval = (f32)(s16)(this->driver->data[pos] + (this->driver->data[pos + 1] << 8));
                this->cursor += 2;
                this->floatValue = fval;
                return fval;
            }
            case TOKEN_NORM_BYTE: {
                raw = this->driver->data[pos];
                f32 fval = (f32)raw * 0.007874015719f;
                this->cursor += 1;
                this->floatValue = fval;
                return fval;
            }
        }
        this->cursor = cur1E8;
    }
    this->func_8004667C();
    return this->floatValue;
}
#else
INCLUDE_ASM("asm/nonmatchings/parrot", readFloat__6Parrot);
#endif

// case 4 accumulator goes to v0 instead of target's a0 (avoids `move v0, a0` at end)
#ifdef NON_MATCHING
s32 Parrot::readInt() {
    if (this->pushedBack != 0) {
        this->pushedBack = 0;
        return this->intValue;
    }
    s32 cur1E8 = this->cursor;
    ParrotDriver* driver = this->driver;
    u32 total = cur1E8 + this->fileOffset;
    u32 base = driver->dataStart;
    if (total >= base && driver->dataEnd >= total + 5) {
        s32 pos = total - base;
        s32 op;
        if (this->repeatCount != 0) {
            op = nextRepeatedToken(this);
        } else {
            op = this->driver->data[pos];
            pos++;
            this->cursor = cur1E8 + 1;
        }
        this->currentType = TOKEN_INT;
        switch (op) {
            case TOKEN_INT: {
                s32 oldUnk1E8 = this->cursor;
                u8* p = this->driver->data + pos;
                u32 b0 = p[0];
                u32 b1 = p[1];
                u32 b2 = p[2];
                u32 b3 = p[3];
                this->cursor = oldUnk1E8 + 4;
                return this->intValue = b0 + (b1 << 8) + (b2 << 16) + (b3 << 24);
            }
            case TOKEN_SBYTE:
            case TOKEN_BYTE: {
                this->cursor++;
                return this->intValue = this->driver->data[pos];
            }
            case TOKEN_SHORT: {
                u8* p = this->driver->data + pos;
                u32 b0 = p[0];
                u32 b1 = p[1];
                this->cursor += 2;
                return this->intValue = (s16)(b0 + (b1 << 8));
            }
            case TOKEN_USHORT: {
                u8* p = this->driver->data + pos;
                u32 b0 = p[0];
                u32 b1 = p[1];
                this->cursor += 2;
                return this->intValue = b0 | (b1 << 8);
            }
        }
        this->cursor = cur1E8;
    }
    this->func_8004667C();
    return this->intValue;
}
#else
INCLUDE_ASM("asm/nonmatchings/parrot", readInt__6Parrot);
#endif

void Parrot::expectToken(s32 arg1) {
    if (this->pushedBack != 0) {
        this->pushedBack = 0;
        return;
    }
    u32 base = this->cursor;
    ParrotDriver* driver = this->driver;
    u32 total = base + this->fileOffset;
    if (total < driver->dataStart || driver->dataEnd < total + 1) {
        this->func_8004667C();
        return;
    }
    this->currentType = arg1;
    if (this->repeatCount != 0) {
        if (this->inStruct) {
            if (++this->structPos < this->structLengths[this->structId])
                return;
            this->structPos = 0;
        } else if (this->inArray) {
            if (++this->structPos < 4)
                return;
            this->structPos = 0;
        }
        if (--this->repeatCount == 0) {
            this->inStruct = 0;
            this->inArray = 0;
        }
        return;
    }
    ParrotDriver* d2 = this->driver;
    if (d2->data[total - d2->dataStart] == arg1) {
        this->cursor++;
        return;
    }
    this->func_8004667C();
}

s32 Parrot::func_80046570() {
    this->expectToken(TOKEN_OPEN_BRACKET);
    this->readInt();
    this->expectToken(TOKEN_CLOSE_BRACKET);
    this->expectToken(TOKEN_OPEN_BRACE);
    return this->intValue;
}

s32 Parrot::func_800465BC() {
    return 0;
}

s32 Parrot::func_800465C4() {
    s32 ret;
    if (this->inStruct) {
        u32 idx = this->structPos;
        ret = this->structDefs[this->structId][idx];
        idx++;
        this->structPos = idx;
        if (idx < this->structLengths[this->structId]) {
            return ret;
        }
        this->structPos = 0;
    } else if (this->inArray) {
        u32 idx = this->structPos;
        ret = D_80004D50[idx];
        idx++;
        this->structPos = idx;
        if (idx < 4u) {
            return ret;
        }
        this->structPos = 0;
    } else {
        ret = this->repeatType;
    }
    this->repeatCount--;
    if (this->repeatCount == 0) {
        this->inStruct = 0;
        this->inArray = 0;
    }
    return ret;
}

s32 Parrot::func_8004667C() {
    return 0;
}

void Parrot::setExtension(const char* src) {
    strncpy(this->extension, src, 5);
    this->extension[4] = 0;
}

char* Parrot::getExtension() {
    if (this->extension[0]) {
        return this->extension;
    }
    return D_80004C14;
}

void Parrot::selectDriver() {
    s32 idx = this->archiveIndex * 13;
    this->driver = ((ParrotDriver**)D_800763F8)[idx];
}

void Parrot::func_80046700(s32 arg1) {
    if (this->unk1A4 != NULL) {
        s32 total = strlen(this->unk1A4) + strlen(D_80076640) + strlen(AbstractFile::getDeviceParam(arg1));
        this->pathBuf[0] = 0;
        if (total < 0xFF) {
            sprintf(this->pathBuf, D_80076640, this->unk1A4);
        }
        strcat(this->pathBuf, AbstractFile::getDeviceParam(arg1));
    }
    func_800079A8(D_80004C10, 0, 0, 0);
}

const char* Parrot::func_800467B8(s32 idx) {
    if (idx >= 0x14) {
        func_800079A8(D_80004C10, 0, 0, 0);
    }
    return D_80076644[idx];
}

s32 Parrot::close() {
    s32 ret = this->AbstractFile::close();
    if (this->unk1A4 != NULL && this->unk1A4 != this->inlineBuf) {
        delete[] this->unk1A4;
        this->unk1A4 = NULL;
    }
    return ret;
}

void Parrot::init() {
    this->pushedBack = 0;
    this->currentType = 0;
    this->unk44[0] = 0;
    this->unk84[0] = 0;
    this->unk1A4 = NULL;
    memset(this->extension, 0, sizeof(this->extension));
    this->repeatCount = 0;
    this->repeatType = 0;
    this->inArray = 0;
    this->inStruct = 0;
    this->structId = 0;
    this->structPos = 0;
    memset(this->structDefs, 0, sizeof(this->structDefs));
    memset(this->structLengths, 0, sizeof(this->structLengths));
    this->driver = NULL;
}

Parrot::~Parrot() {
    if (this->unk1A4 != NULL && this->unk1A4 != this->inlineBuf) {
        delete[] this->unk1A4;
    }
}

Parrot::Parrot() {
    this->init();
}

u8* Parrot::func_8004698C() {
    return this->unk44;
}

s32 Parrot::getInt() {
    return this->intValue;
}

f32 Parrot::getFloat() {
    return this->floatValue;
}

f32 Parrot::func_800469AC() {
    return this->unk3C;
}

s32 Parrot::pushBack() {
    this->pushedBack = 1;
    return 1;
}

s32 Parrot::getCurrentType() {
    return this->currentType;
}

char* Parrot::func_800469D0() {
    return this->unk1A4;
}

u8* Parrot::func_800469DC() {
    this->func_8004667C();
    return this->unk44;
}

u8* Parrot::func_80046A14() {
    this->func_8004667C();
    return this->unk44;
}

void Parrot::expectCloseBrace() {
    this->expectToken(TOKEN_CLOSE_BRACE);
}

void Parrot::expectOpenBrace() {
    this->expectToken(TOKEN_OPEN_BRACE);
}

void Parrot::expectCloseBracket() {
    this->expectToken(TOKEN_CLOSE_BRACKET);
}

void Parrot::expectOpenBracket() {
    this->expectToken(TOKEN_OPEN_BRACKET);
}

void Parrot::readToken(s32 arg1) {
    this->expectToken(arg1);
}

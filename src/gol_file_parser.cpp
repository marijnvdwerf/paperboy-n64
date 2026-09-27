#include "gol_file_parser.h"

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
}

const u32 D_80004D50[] = { TOKEN_OPEN_BRACKET, TOKEN_INT, TOKEN_CLOSE_BRACKET, TOKEN_CLOSE_BRACKET };

INCLUDE_RODATA("asm/nonmatchings/gol_file_parser", D_80004A80);

INCLUDE_RODATA("asm/nonmatchings/gol_file_parser", D_80004C10);

INCLUDE_RODATA("asm/nonmatchings/gol_file_parser", D_80004C14);

static inline s32 nextRepeatedTokenInline(GolFileParser* self) {
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

f32 GolFileParser::readFloat() {
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
            op = nextRepeatedTokenInline(this);
        } else {
            op = this->driver->data[pos];
            pos++;
            this->cursor = cur1E8 + 1;
        }
        this->currentType = TOKEN_FLOAT;
        switch (op) {
            case TOKEN_FLOAT: {
                raw = this->driver->data[pos] + (this->driver->data[pos + 1] << 8) + (this->driver->data[pos + 2] << 16) + (this->driver->data[pos + 3] << 24);
                f32 fval = *(f32*)&raw;
                this->cursor += 4;
                this->floatValue = fval;
                return fval;
            }
            case TOKEN_FIXED_4096: {
                f32 fval = (f32)(s16)(this->driver->data[pos] + (this->driver->data[pos + 1] << 8)) * 0.000244140625f;
                this->cursor += 2;
                this->floatValue = fval;
                return fval;
            }
            case TOKEN_FIXED_32: {
                f32 fval = (f32)(s16)(this->driver->data[pos] + (this->driver->data[pos + 1] << 8)) * 0.03125f;
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
    this->nextToken();
    return this->floatValue;
}

#ifdef NON_MATCHING
s32 GolFileParser::readInt() {
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
            op = nextRepeatedTokenInline(this);
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
    this->nextToken();
    return this->intValue;
}
#else
INCLUDE_ASM("asm/nonmatchings/gol_file_parser", readInt__13GolFileParser);
#endif

void GolFileParser::expectToken(s32 arg1) {
    if (this->pushedBack != 0) {
        this->pushedBack = 0;
        return;
    }
    u32 base = this->cursor;
    ParrotDriver* driver = this->driver;
    u32 total = base + this->fileOffset;
    if (total < driver->dataStart || driver->dataEnd < total + 1) {
        this->nextToken();
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
    this->nextToken();
}

s32 GolFileParser::beginArray() {
    this->expectToken(TOKEN_OPEN_BRACKET);
    this->readInt();
    this->expectToken(TOKEN_CLOSE_BRACKET);
    this->expectToken(TOKEN_OPEN_BRACE);
    return this->intValue;
}

s32 GolFileParser::vfunc21(char*, s32) {
    return 0;
}

s32 GolFileParser::nextRepeatedToken() {
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

s32 GolFileParser::nextToken() {
    return 0;
}

void GolFileParser::setExtension(const char* src) {
    strncpy(this->extension, src, 5);
    this->extension[4] = 0;
}

char* GolFileParser::getExtension() {
    if (this->extension[0]) {
        return this->extension;
    }
    return D_80004C14;
}

void GolFileParser::selectDriver(const char*) {
    s32 idx = this->archiveIndex * 13;
    this->driver = ((ParrotDriver**)D_800763F8)[idx];
}

void GolFileParser::ioError(s32 err) {
    if (this->path != NULL) {
        s32 total = strlen(this->path) + strlen(D_80076640) + strlen(GolStream::errorMessage(err));
        this->readBuf[0] = 0;
        if (total < 0xFF) {
            sprintf((char*)this->readBuf, D_80076640, this->path);
        }
        strcat((char*)this->readBuf, GolStream::errorMessage(err));
    }
    __assert(D_80004C10, 0, 0, 0);
}

const char* GolFileParser::errorMessage(s32 code) {
    if (code >= 0x14) {
        __assert(D_80004C10, 0, 0, 0);
    }
    return D_80076644[code];
}

s32 GolFileParser::close() {
    s32 ret = this->GolStream::close();
    if (this->path != NULL && this->path != this->inlineBuf) {
        delete[] this->path;
        this->path = NULL;
    }
    return ret;
}

void GolFileParser::init() {
    this->pushedBack = 0;
    this->currentType = 0;
    this->stringValue[0] = 0;
    this->unk84[0] = 0;
    this->path = NULL;
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

GolFileParser::~GolFileParser() {
    if (this->path != NULL && this->path != this->inlineBuf) {
        delete[] this->path;
    }
}

GolFileParser::GolFileParser() {
    this->init();
}

u8* GolFileParser::func_8004698C() {
    return this->stringValue;
}

s32 GolFileParser::getInt() {
    return this->intValue;
}

f32 GolFileParser::getFloat() {
    return this->floatValue;
}

f32 GolFileParser::func_800469AC() {
    return this->unk3C;
}

s32 GolFileParser::pushBack() {
    this->pushedBack = 1;
    return 1;
}

s32 GolFileParser::getCurrentType() {
    return this->currentType;
}

char* GolFileParser::getPath() {
    return this->path;
}

u8* GolFileParser::func_800469DC() {
    this->nextToken();
    return this->stringValue;
}

u8* GolFileParser::func_80046A14() {
    this->nextToken();
    return this->stringValue;
}

void GolFileParser::expectCloseBrace() {
    this->expectToken(TOKEN_CLOSE_BRACE);
}

void GolFileParser::expectOpenBrace() {
    this->expectToken(TOKEN_OPEN_BRACE);
}

void GolFileParser::expectCloseBracket() {
    this->expectToken(TOKEN_CLOSE_BRACKET);
}

void GolFileParser::expectOpenBracket() {
    this->expectToken(TOKEN_OPEN_BRACKET);
}

void GolFileParser::readToken(s32 arg1) {
    this->expectToken(arg1);
}

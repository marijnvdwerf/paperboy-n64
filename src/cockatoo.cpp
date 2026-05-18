#include "cockatoo.h"

extern "C" {
extern char* strcpy(char*, const char*);
extern char* strcat(char*, const char*);
extern unsigned strlen(const char*);
extern int sprintf(char*, const char*, ...);

extern char* D_80076160;
extern char D_80004320[];
extern char D_8000432C[];
extern char D_80004330[];
}

#if 1
void Cockatoo::selectDriver(const char* path) {
    Cockatoo* self = this;
    if (self->state & 1) {
        self->close();
    }
    const char* ext;
    s32 dotIdx = -1;
    s32 pathLen = 0;
    if (*path != 0) {
        s32 dot = '.';
        const char* p = path;
        do {
            if (*p == dot) {
                dotIdx = pathLen;
            }
            p++;
            pathLen++;
        } while (*p != 0);
    }
    if (dotIdx < 0) {
        ext = self->getExtension();
        pathLen += strlen(ext);
        self->path = pathLen < 0x40 ? self->inlineBuf : new char[pathLen + 1];
        if (self->path == NULL) {
            __assert(D_8000432C, 0, 0, 0);
        }
        strcpy(self->path, path);
        strcat(self->path, ext);
    } else if (self->extension[0] != 0) {
        ext = self->getExtension();
        pathLen += strlen(ext);
        self->path = pathLen < 0x40 ? self->inlineBuf : new char[pathLen + 1];
        if (self->path == NULL) {
            __assert(D_8000432C, 0, 0, 0);
        }
        strcpy(self->path, path);
        strcpy(self->path + dotIdx, ext);
    } else {
        self->path = pathLen < 0x40 ? self->inlineBuf : new char[pathLen + 1];
        if (self->path == NULL) {
            __assert(D_8000432C, 0, 0, 0);
        }
        strcpy(self->path, path);
    }
    s32 ret = self->AbstractFile::open(self->path, 2, 0x1000);
    if (ret != 0) {
        self->ioError(ret);
    }
    self->cursor = 0;
    self->unk650 = NULL;
    self->currentType = 0;
    self->pushedBack = 0;
    self->Parrot::selectDriver(self->path);
}
#else
INCLUDE_ASM("asm/nonmatchings/cockatoo", selectDriver__8CockatooPCc);
#endif

INCLUDE_RODATA("asm/nonmatchings/cockatoo", D_80004320);

INCLUDE_RODATA("asm/nonmatchings/cockatoo", D_8000432C);

INCLUDE_RODATA("asm/nonmatchings/cockatoo", D_80004330);

#ifdef NON_MATCHING
s32 Cockatoo::nextToken() {
    Cockatoo* self = this;
    if (self->pushedBack != 0) {
        self->pushedBack = 0;
        return self->currentType;
    }
    s32 token;
    if (self->repeatCount != 0) {
        token = self->nextRepeatedToken();
    } else {
        if (self->readBytes(1) == 0) {
            return 0;
        }
        token = self->readBuf[0];
    }
    s32 ret;
    s32 idx;
    switch (token) {
        case TOKEN_STRING: {
            s32 actualRead;
            s32 r = self->readAt(self->cursor, self->readBuf, 0x3F, &actualRead);
            if (r != 0) {
                if (r == 0x10) {
                    if (actualRead == 0) {
                        self->currentType = 0;
                        return 0;
                    }
                } else {
                    self->ioError(r);
                }
            }
            u32 i = 0;
            if (actualRead != 0) {
                while (1) {
                    if (self->readBuf[i] == 0) {
                        break;
                    }
                    self->stringValue[i] = self->readBuf[i];
                    i++;
                    if (i >= (u32)actualRead) {
                        break;
                    }
                }
            }
            self->stringValue[i] = 0;
            self->currentType = token;
            self->cursor += i + 1;
            return token;
        }
        case TOKEN_FLOAT: {
            if (self->readBytes(4) == 0) {
                return 0;
            }
            u32 raw = self->readBuf[0] + (self->readBuf[1] << 8) +
                      (self->readBuf[2] << 16) + (self->readBuf[3] << 24);
            self->floatValue = *(f32*)&raw;
            self->currentType = token;
            return token;
        }
        case TOKEN_INT: {
            if (self->readBytes(4) == 0) {
                return self->currentType = 0;
            }
            self->intValue = self->readBuf[0] + (self->readBuf[1] << 8) +
                             (self->readBuf[2] << 16) + (self->readBuf[3] << 24);
            self->currentType = token;
            return token;
        }
        case TOKEN_SBYTE:
        case TOKEN_BYTE: {
            if (self->readBytes(1) == 0) {
                return self->currentType = 0;
            }
            self->intValue = self->readBuf[0];
            self->currentType = TOKEN_INT;
            return TOKEN_INT;
        }
        case TOKEN_SHORT: {
            if (self->readBytes(2) == 0) {
                return self->currentType = 0;
            }
            self->intValue = (s16)(self->readBuf[0] + (self->readBuf[1] << 8));
            self->currentType = TOKEN_INT;
            return TOKEN_INT;
        }
        case TOKEN_USHORT: {
            if (self->readBytes(2) == 0) {
                return self->currentType = 0;
            }
            self->intValue = self->readBuf[0] | (self->readBuf[1] << 8); // ushort uses or
            self->currentType = TOKEN_INT;
            return TOKEN_INT;
        }
        case TOKEN_FIXED_4096: {
            if (self->readBytes(2) == 0) {
                return 0;
            }
            self->floatValue =
                (f32)(s16)(self->readBuf[0] + (self->readBuf[1] << 8)) * 0.000244140625f;
            self->currentType = TOKEN_FLOAT;
            return TOKEN_FLOAT;
        }
        case TOKEN_FIXED_32: {
            if (self->readBytes(2) == 0) {
                return 0;
            }
            self->floatValue =
                (f32)(s16)(self->readBuf[0] + (self->readBuf[1] << 8)) * 0.03125f;
            self->currentType = TOKEN_FLOAT;
            return TOKEN_FLOAT;
        }
        case TOKEN_SHORT_F: {
            if (self->readBytes(2) == 0) {
                return self->currentType = 0;
            }
            self->floatValue = (f32)(s16)(self->readBuf[0] + (self->readBuf[1] << 8));
            self->currentType = TOKEN_FLOAT;
            return TOKEN_FLOAT;
        }
        case TOKEN_NORM_BYTE: {
            if (self->readBytes(1) == 0) {
                return self->currentType = 0;
            }
            self->floatValue = (f32)self->readBuf[0] * 0.007874015719f;
            self->currentType = TOKEN_FLOAT;
            return TOKEN_FLOAT;
        }
        case TOKEN_EXT: {
            if (self->readBytes(2) == 0) {
                return 0;
            }
            ret = self->readBuf[0] + (self->readBuf[1] << 8);
            self->currentType = ret;
            return ret;
        }
        case TOKEN_REPEAT: {
            if (self->readBytes(2) == 0) {
                return self->currentType = 0;
            }
            self->repeatCount = self->readBuf[0] + (self->readBuf[1] << 8);
            if (self->readBytes(1) == 0) {
                return self->currentType = 0;
            }
            self->repeatType = self->readBuf[0];
            if (self->repeatType == TOKEN_EXT) {
                if (self->readBytes(2) == 0) {
                    return 0;
                }
                self->repeatType = self->readBuf[0] + (self->readBuf[1] << 8);
            }
            idx = self->repeatType - TOKEN_STRUCT_BASE;
            self->inStruct = 0;
            if ((u32)idx < 0x10) {
                self->structId = idx;
                if (&self->structDefs[idx][0] != NULL) {
                    self->parseError(0);
                }
                self->structPos = 0;
                self->inStruct = 1;
            } else if (self->repeatType == TOKEN_ARRAY) {
                self->inArray = 1;
                self->structPos = 0;
            }
            goto recurse;
        }
        case TOKEN_ARRAY: {
            self->repeatCount = 1;
            self->inArray = 1;
            self->structPos = 0;
            self->inStruct = 0;
            goto recurse;
        }
        case TOKEN_STRUCT_DEF: {
            self->readStructDef();
            if (self->currentType != 0) {
                goto recurse;
            }
            return 0;
        }
        default: {
            idx = token - TOKEN_STRUCT_BASE;
            if ((u32)idx < 0x10) {
                self->structId = idx;
                if (&self->structDefs[idx][0] != NULL) {
                    self->parseError(0);
                }
                self->inStruct = 1;
                self->structPos = 0;
                self->repeatCount = 1;
                goto recurse;
            }
            self->currentType = token;
            return token;
        }
    }
recurse:
    return self->nextToken();
}
#else
INCLUDE_ASM("asm/nonmatchings/cockatoo", nextToken__8Cockatoo);
#endif

void Cockatoo::parseError(s32 code) {
    if (this->path != NULL) {
        s32 total = strlen(this->path) + strlen(D_80076160) +
                    strlen(this->errorMessage(code));
        this->readBuf[0] = 0;
        if (total < 0xFF) {
            sprintf((char*)this->readBuf, D_80076160, this->path);
        }
        strcat((char*)this->readBuf, this->errorMessage(code));
    }
    __assert(D_8000432C, 0, 0, 0);
}

s32 Cockatoo::vfunc21(char*, s32) {
    return 1;
}

s32 Cockatoo::readBytes(s32 nbytes) {
    s32 actualRead;
    s32 ret = this->readAt(this->cursor, this->readBuf, nbytes, &actualRead);
    if (ret != 0) {
        if (ret == 0x10) {
            if (actualRead == 0) {
                this->currentType = 0;
                return 0;
            }
        } else {
            this->ioError(ret);
        }
    }
    if (actualRead != nbytes) {
        this->parseError(0);
    }
    this->cursor += nbytes;
    return 1;
}

#ifdef NON_MATCHING
void Cockatoo::readStructDef() {
    if (this->readBytes(2) == 0) {
        return;
    }
    u32 idx = this->readBuf[0];
    u32 count = this->readBuf[1];
    u32 i = 0;
    this->structLengths[idx - TOKEN_STRUCT_BASE] = count;
    if (count == 0) {
        return;
    }
    do {
        if (this->readBytes(1) == 0) {
            return;
        }
        u32 val = this->readBuf[0];
        if (val == TOKEN_EXT) {
            if (this->readBytes(2) == 0) {
                return;
            }
            val = this->readBuf[0] + (this->readBuf[1] << 8);
        }
        this->structDefs[idx - TOKEN_STRUCT_BASE][i] = val;
        i++;
    } while (i < count);
}
#else
INCLUDE_ASM("asm/nonmatchings/cockatoo", readStructDef__8Cockatoo);
#endif

char* Cockatoo::getExtension() {
    if (this->extension[0] != 0) {
        return this->extension;
    }
    return D_80004330;
}

s32 Cockatoo::close() {
    s32 ret = this->Parrot::close();
    this->cursor = 0;
    this->unk650 = NULL;
    this->init();
    return ret;
}

void Cockatoo::resetStream() {
    this->cursor = 0;
    this->unk650 = NULL;
    this->init();
}

Cockatoo::Cockatoo() {
    this->resetStream();
}

#include "cockatoo.h"

extern "C" {
extern char* strcpy(char*, const char*);
extern char* strcat(char*, const char*);
extern unsigned strlen(const char*);
extern int sprintf(char*, const char*, ...);

extern char* D_80076160;
extern const char D_80004320[] = "file %s\n";
}

void Cockatoo::selectDriver(const char* path) {
    if (this->state & 1) {
        this->close();
    }
    const char* ext;
    s32 dotIdx = -1;
    s32 pathLen = 0;
    if (*path != 0) {
        s32 dot = '.';
        do {
            if (path[pathLen] == dot) {
                dotIdx = pathLen;
            }
            pathLen++;
        } while (path[pathLen] != 0);
    }
    if (dotIdx < 0) {
        ext = this->getExtension();
        pathLen += strlen(ext);
        this->path = pathLen < 0x40 ? this->inlineBuf : new char[pathLen + 1];
        if (this->path == NULL) {
            __assert("", 0, 0, 0);
        }
        strcpy(this->path, path);
        strcat(this->path, ext);
    } else if (this->extension[0] != 0) {
        ext = this->getExtension();
        pathLen += strlen(ext);
        this->path = pathLen < 0x40 ? this->inlineBuf : new char[pathLen + 1];
        if (this->path == NULL) {
            __assert("", 0, 0, 0);
        }
        strcpy(this->path, path);
        strcpy(this->path + dotIdx, ext);
    } else {
        this->path = pathLen < 0x40 ? this->inlineBuf : new char[pathLen + 1];
        if (this->path == NULL) {
            __assert("", 0, 0, 0);
        }
        strcpy(this->path, path);
    }
    s32 ret = this->AbstractFile::open(this->path, 2, 0x1000);
    if (ret != 0) {
        this->ioError(ret);
    }
    this->cursor = 0;
    this->unk650 = NULL;
    this->currentType = 0;
    this->pushedBack = 0;
    this->Parrot::selectDriver(this->path);
}

static inline char* defaultExtension() {
    return ".bin";
}

s32 Cockatoo::nextToken() {
    if (this->pushedBack != 0) {
        this->pushedBack = 0;
        return this->currentType;
    }

    union TokenValue {
        s32 integer;
        f32 real;
    } value;

    s32 token;
    while (1) {
        if (this->repeatCount != 0) {
            token = this->nextRepeatedToken();
        } else {
            if (this->readBytes(1) == 0) {
                return 0;
            }
            token = this->readBuf[0];
        }
        s32 repeatedType;
        u32 idx;
        switch (token) {
            case TOKEN_STRING: {
                s32 actualRead;
                s32 r = this->readAt(this->cursor, this->readBuf, 0x3F, &actualRead);
                if (r != 0) {
                    if (r == 0x10) {
                        if (actualRead == 0) {
                            this->currentType = 0;
                            return 0;
                        }
                    } else {
                        this->ioError(r);
                    }
                }
                u32 i = 0;
                if (actualRead != 0) {
                    while (1) {
                        if (this->readBuf[i] == 0) {
                            break;
                        }
                        this->stringValue[i] = this->readBuf[i];
                        i++;
                        if (i >= actualRead) {
                            break;
                        }
                    }
                }
                this->stringValue[i] = 0;
                this->currentType = token;
                this->cursor += i + 1;
                return token;
            }
            case TOKEN_FLOAT: {
                if (this->readBytes(4) == 0) {
                    return 0;
                }
                value.integer = this->readBuf[0] + (this->readBuf[1] << 8) + (this->readBuf[2] << 16) + (this->readBuf[3] << 24);
                this->floatValue = (&value)->real;
                this->currentType = token;
                return token;
            }
            case TOKEN_INT: {
                if (this->readBytes(4) == 0) {
                    return 0;
                }
                this->currentType = token;
                value.integer = this->readBuf[0] + (this->readBuf[1] << 8) + (this->readBuf[2] << 16) + (this->readBuf[3] << 24);
                this->intValue = value.integer;
                return token;
            }
            case TOKEN_SBYTE:
            case TOKEN_BYTE: {
                if (this->readBytes(1) == 0) {
                    return 0;
                }
                this->intValue = this->readBuf[0];
                this->currentType = TOKEN_INT;
                return TOKEN_INT;
            }
            case TOKEN_SHORT: {
                if (this->readBytes(2) == 0) {
                    return 0;
                }
                this->intValue = (s16)(this->readBuf[0] + (this->readBuf[1] << 8));
                this->currentType = TOKEN_INT;
                return TOKEN_INT;
            }
            case TOKEN_USHORT: {
                if (this->readBytes(2) == 0) {
                    return 0;
                }
                this->intValue = this->readBuf[0] | (this->readBuf[1] << 8);
                this->currentType = TOKEN_INT;
                return TOKEN_INT;
            }
            case TOKEN_FIXED_4096: {
                if (this->readBytes(2) == 0) {
                    return 0;
                }
                this->floatValue = (s16)(this->readBuf[0] + (this->readBuf[1] << 8)) * 0.000244140625f;
                this->currentType = TOKEN_FLOAT;
                return TOKEN_FLOAT;
            }
            case TOKEN_FIXED_32: {
                if (this->readBytes(2) == 0) {
                    return 0;
                }
                this->floatValue = (s16)(this->readBuf[0] + (this->readBuf[1] << 8)) * 0.03125f;
                this->currentType = TOKEN_FLOAT;
                return TOKEN_FLOAT;
            }
            case TOKEN_SHORT_F: {
                if (this->readBytes(2) == 0) {
                    return 0;
                }
                this->floatValue = (s16)(this->readBuf[0] + (this->readBuf[1] << 8));
                this->currentType = TOKEN_FLOAT;
                return TOKEN_FLOAT;
            }
            case TOKEN_NORM_BYTE: {
                if (this->readBytes(1) == 0) {
                    return 0;
                }
                this->currentType = TOKEN_FLOAT;
                value.integer = this->readBuf[0];
                this->floatValue = value.integer * 0.007874015719f;
                return TOKEN_FLOAT;
            }
            case TOKEN_EXT: {
                if (this->readBytes(2) == 0) {
                    return 0;
                }
                token = this->readBuf[0] + (this->readBuf[1] << 8);
                this->currentType = token;
                return token;
            }
            case TOKEN_REPEAT: {
                if (this->readBytes(2) == 0) {
                    return 0;
                }
                this->repeatCount = this->readBuf[0] + (this->readBuf[1] << 8);
                if (this->readBytes(1) == 0) {
                    return 0;
                }
                this->repeatType = this->readBuf[0];
                if (this->repeatType == TOKEN_EXT) {
                    if (this->readBytes(2) == 0) {
                        return 0;
                    }
                    this->repeatType = this->readBuf[0] + (this->readBuf[1] << 8);
                }
                repeatedType = this->repeatType;
                idx = repeatedType - TOKEN_STRUCT_BASE;
                this->inStruct = 0;
                if (idx < 0x10) {
                    this->structId = idx;
                    if (&this->structDefs[idx][0] == NULL) {
                        this->parseError(0);
                    }
                    this->structPos = 0;
                    this->inStruct = 1;
                } else if (repeatedType == TOKEN_ARRAY) {
                    this->inArray = 1;
                    this->structPos = 0;
                }
                break;
            }
            case TOKEN_ARRAY: {
                this->repeatCount = 1;
                this->inArray = 1;
                this->structPos = 0;
                this->inStruct = 0;
                break;
            }
            case TOKEN_STRUCT_DEF: {
                this->readStructDef();
                if (this->currentType != 0) {
                    break;
                }
                return 0;
            }
            default: {
                idx = token - TOKEN_STRUCT_BASE;
                if (idx < 0x10) {
                    this->structId = idx;
                    if (&this->structDefs[idx][0] == NULL) {
                        this->parseError(0);
                    }
                    this->structPos = 0;
                    this->inStruct = 1;
                    this->repeatCount = 1;
                    break;
                }
                this->currentType = token;
                return token;
            }
        }
    }
}

void Cockatoo::parseError(s32 code) {
    if (this->path != NULL) {
        s32 total = strlen(this->path) + strlen(D_80076160) + strlen(this->errorMessage(code));
        this->readBuf[0] = 0;
        if (total < 0xFF) {
            sprintf((char*)this->readBuf, D_80076160, this->path);
        }
        strcat((char*)this->readBuf, this->errorMessage(code));
    }
    __assert("", 0, 0, 0);
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

void Cockatoo::readStructDef() {
    if (this->readBytes(2) == 0) {
        return;
    }
    u32 idx = this->readBuf[0] - TOKEN_STRUCT_BASE;
    u32 count = this->readBuf[1];
    u32 i;
    u32 val;
    this->structLengths[idx] = count;
    for (i = 0; i < count; i++) {
        if (this->readBytes(1) == 0) {
            return;
        }
        val = this->readBuf[0];
        if (val == TOKEN_EXT) {
            if (this->readBytes(2) == 0) {
                return;
            }
            val = this->readBuf[0] + (this->readBuf[1] << 8);
        }
        this->structDefs[idx][i] = val;
    }
}

char* Cockatoo::getExtension() {
    if (this->extension[0] != 0) {
        return this->extension;
    }
    return defaultExtension();
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

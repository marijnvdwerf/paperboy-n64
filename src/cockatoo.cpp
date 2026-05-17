#include "cockatoo.h"

extern "C" {
extern char* strcpy(char*, const char*);
extern char* strcat(char*, const char*);
extern unsigned strlen(const char*);
extern int sprintf(char*, const char*, ...);
extern s32 func_800079A8(const char*, s32, s32, s32);

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
        self->unk1A4 = pathLen < 0x40 ? self->inlineBuf : new char[pathLen + 1];
        if (self->unk1A4 == NULL) {
            func_800079A8(D_8000432C, 0, 0, 0);
        }
        strcpy(self->unk1A4, path);
        strcat(self->unk1A4, ext);
    } else if (self->extension[0] != 0) {
        ext = self->getExtension();
        pathLen += strlen(ext);
        self->unk1A4 = pathLen < 0x40 ? self->inlineBuf : new char[pathLen + 1];
        if (self->unk1A4 == NULL) {
            func_800079A8(D_8000432C, 0, 0, 0);
        }
        strcpy(self->unk1A4, path);
        strcpy(self->unk1A4 + dotIdx, ext);
    } else {
        self->unk1A4 = pathLen < 0x40 ? self->inlineBuf : new char[pathLen + 1];
        if (self->unk1A4 == NULL) {
            func_800079A8(D_8000432C, 0, 0, 0);
        }
        strcpy(self->unk1A4, path);
    }
    s32 ret = self->AbstractFile::open(self->unk1A4, 2, 0x1000);
    if (ret != 0) {
        self->func_80046700(ret);
    }
    self->cursor = 0;
    self->unk650 = NULL;
    self->currentType = 0;
    self->pushedBack = 0;
    self->Parrot::selectDriver(self->unk1A4);
}
#else
INCLUDE_ASM("asm/nonmatchings/cockatoo", selectDriver__8CockatooPCc);
#endif

INCLUDE_RODATA("asm/nonmatchings/cockatoo", D_80004320);

INCLUDE_RODATA("asm/nonmatchings/cockatoo", D_8000432C);

INCLUDE_RODATA("asm/nonmatchings/cockatoo", D_80004330);

#ifdef NON_MATCHING
s32 Cockatoo::vfunc20() {
    Cockatoo* self = this;
    if (self->pushedBack != 0) {
        self->pushedBack = 0;
        return self->currentType;
    }
    s32 token;
    if (self->repeatCount != 0) {
        token = self->func_800465C4();
    } else {
        if (self->func_80040910(1) == 0) {
            return 0;
        }
        token = (u8)self->pathBuf[0];
    }
    s32 ret;
    s32 idx;
    switch (token) {
        case 2: { // string
            s32 actualRead;
            s32 r = self->readAt(self->cursor, self->pathBuf, 0x3F, &actualRead);
            if (r != 0) {
                if (r == 0x10) {
                    if (actualRead == 0) {
                        self->currentType = 0;
                        return 0;
                    }
                } else {
                    self->func_80046700(r);
                }
            }
            u32 i = 0;
            if (actualRead != 0) {
                while (1) {
                    if (self->pathBuf[i] == 0) {
                        break;
                    }
                    self->unk44[i] = self->pathBuf[i];
                    i++;
                    if (i >= (u32)actualRead) {
                        break;
                    }
                }
            }
            self->unk44[i] = 0;
            self->currentType = token;
            self->cursor += i + 1;
            return token;
        }
        case 3: { // float
            if (self->func_80040910(4) == 0) {
                return 0;
            }
            u32 raw = (u8)self->pathBuf[0] + ((u8)self->pathBuf[1] << 8) +
                      ((u8)self->pathBuf[2] << 16) + ((u8)self->pathBuf[3] << 24);
            self->floatValue = *(f32*)&raw;
            self->currentType = token;
            return token;
        }
        case 4: { // int
            if (self->func_80040910(4) == 0) {
                return self->currentType = 0;
            }
            self->intValue = (u8)self->pathBuf[0] + ((u8)self->pathBuf[1] << 8) +
                             ((u8)self->pathBuf[2] << 16) + ((u8)self->pathBuf[3] << 24);
            self->currentType = token;
            return token;
        }
        case 11:
        case 12: { // sbyte / byte
            if (self->func_80040910(1) == 0) {
                return self->currentType = 0;
            }
            self->intValue = (u8)self->pathBuf[0];
            self->currentType = 4;
            return 4;
        }
        case 13: { // short
            if (self->func_80040910(2) == 0) {
                return self->currentType = 0;
            }
            self->intValue = (s16)((u8)self->pathBuf[0] + ((u8)self->pathBuf[1] << 8));
            self->currentType = 4;
            return 4;
        }
        case 14: { // ushort
            if (self->func_80040910(2) == 0) {
                return self->currentType = 0;
            }
            self->intValue = (u8)self->pathBuf[0] | ((u8)self->pathBuf[1] << 8); // case 14 uses or
            self->currentType = 4;
            return 4;
        }
        case 15: { // fixed-4096
            if (self->func_80040910(2) == 0) {
                return 0;
            }
            self->floatValue =
                (f32)(s16)((u8)self->pathBuf[0] + ((u8)self->pathBuf[1] << 8)) * 0.000244140625f;
            self->currentType = 3;
            return 3;
        }
        case 16: { // fixed-32
            if (self->func_80040910(2) == 0) {
                return 0;
            }
            self->floatValue =
                (f32)(s16)((u8)self->pathBuf[0] + ((u8)self->pathBuf[1] << 8)) * 0.03125f;
            self->currentType = 3;
            return 3;
        }
        case 17: { // short-as-float
            if (self->func_80040910(2) == 0) {
                return self->currentType = 0;
            }
            self->floatValue = (f32)(s16)((u8)self->pathBuf[0] + ((u8)self->pathBuf[1] << 8));
            self->currentType = 3;
            return 3;
        }
        case 18: { // norm-byte
            if (self->func_80040910(1) == 0) {
                return self->currentType = 0;
            }
            self->floatValue = (f32)(u8)self->pathBuf[0] * 0.007874015719f;
            self->currentType = 3;
            return 3;
        }
        case 19: {
            if (self->func_80040910(2) == 0) {
                return 0;
            }
            ret = (u8)self->pathBuf[0] + ((u8)self->pathBuf[1] << 8);
            self->currentType = ret;
            return ret;
        }
        case 20: { // repeat
            if (self->func_80040910(2) == 0) {
                return self->currentType = 0;
            }
            self->repeatCount = (u8)self->pathBuf[0] + ((u8)self->pathBuf[1] << 8);
            if (self->func_80040910(1) == 0) {
                return self->currentType = 0;
            }
            self->repeatType = (u8)self->pathBuf[0];
            if (self->repeatType == 0x13) {
                if (self->func_80040910(2) == 0) {
                    return 0;
                }
                self->repeatType = (u8)self->pathBuf[0] + ((u8)self->pathBuf[1] << 8);
            }
            idx = self->repeatType - 0x17;
            self->inStruct = 0;
            if ((u32)idx < 0x10) {
                self->structId = idx;
                if (&self->structDefs[idx][0] != NULL) {
                    self->vfunc19(0);
                }
                self->structPos = 0;
                self->inStruct = 1;
            } else if (self->repeatType == 0x15) {
                self->inArray = 1;
                self->structPos = 0;
            }
            goto recurse;
        }
        case 21: { // array start
            self->repeatCount = 1;
            self->inArray = 1;
            self->structPos = 0;
            self->inStruct = 0;
            goto recurse;
        }
        case 22: { // struct definition
            self->func_800409C8();
            if (self->currentType != 0) {
                goto recurse;
            }
            return 0;
        }
        default: {
            idx = token - 0x17;
            if ((u32)idx < 0x10) {
                self->structId = idx;
                if (&self->structDefs[idx][0] != NULL) {
                    self->vfunc19(0);
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
    return self->vfunc20();
}
#else
INCLUDE_ASM("asm/nonmatchings/cockatoo", vfunc20__8Cockatoo);
#endif

void Cockatoo::vfunc19(s32 arg1) {
    if (this->unk1A4 != NULL) {
        s32 total = strlen(this->unk1A4) + strlen(D_80076160) +
                    strlen(this->func_800467B8(arg1));
        this->pathBuf[0] = 0;
        if (total < 0xFF) {
            sprintf(this->pathBuf, D_80076160, this->unk1A4);
        }
        strcat(this->pathBuf, this->func_800467B8(arg1));
    }
    func_800079A8(D_8000432C, 0, 0, 0);
}

s32 Cockatoo::vfunc21() {
    return 1;
}

s32 Cockatoo::func_80040910(s32 nbytes) {
    s32 actualRead;
    s32 ret = this->readAt(this->cursor, this->pathBuf, nbytes, &actualRead);
    if (ret != 0) {
        if (ret == 0x10) {
            if (actualRead == 0) {
                this->currentType = 0;
                return 0;
            }
        } else {
            this->func_80046700(ret);
        }
    }
    if (actualRead != nbytes) {
        this->vfunc19(0);
    }
    this->cursor += nbytes;
    return 1;
}

#ifdef NON_MATCHING
void Cockatoo::func_800409C8() {
    if (this->func_80040910(2) == 0) {
        return;
    }
    u32 idx = (u8)this->pathBuf[0];
    u32 count = (u8)this->pathBuf[1];
    u32 i = 0;
    this->structLengths[idx - 0x17] = count;
    if (count == 0) {
        return;
    }
    do {
        if (this->func_80040910(1) == 0) {
            return;
        }
        u32 val = (u8)this->pathBuf[0];
        if (val == 0x13) {
            if (this->func_80040910(2) == 0) {
                return;
            }
            val = (u8)this->pathBuf[0] + ((u8)this->pathBuf[1] << 8);
        }
        this->structDefs[idx - 0x17][i] = val;
        i++;
    } while (i < count);
}
#else
INCLUDE_ASM("asm/nonmatchings/cockatoo", func_800409C8__8Cockatoo);
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

void Cockatoo::func_80040AF0() {
    this->cursor = 0;
    this->unk650 = NULL;
    this->init();
}

Cockatoo::Cockatoo() {
    this->func_80040AF0();
}

#include "common.h"
#include "structs.h"

struct ParrotDriver {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ u32 unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u8* unk20;
};

struct Parrot : public RomFile {
    /* 0x030 */ s32 unk30;
    /* 0x034 */ s32 unk34;
    /* 0x038 */ s32 unk38;
    /* 0x03C */ f32 unk3C;
    /* 0x040 */ f32 unk40;
    /* 0x044 */ u8 unk44[0x40];
    /* 0x084 */ u8 unk84[0x120];
    /* 0x1A4 */ u8* unk1A4;
    /* 0x1A8 */ u8 inlineBuf[0x40];
    /* 0x1E8 */ s32 unk1E8;
    /* 0x1EC */ char unk1EC[4];
    /* 0x1F0 */ u8 unk1F0[4];
    /* 0x1F4 */ s32 unk1F4;
    /* 0x1F8 */ s32 unk1F8;
    /* 0x1FC */ s32 unk1FC;
    /* 0x200 */ s32 unk200;
    /* 0x204 */ s32 unk204;
    /* 0x208 */ s32 unk208;
    /* 0x20C */ u32 unk20C[16][16];
    /* 0x60C */ u32 unk60C[0x10];
    /* 0x64C */ ParrotDriver* unk64C;

    Parrot();
    virtual ~Parrot();
    virtual s32 close();
    virtual void func_800466D0();
    virtual char* func_800466B4();
    virtual void vfunc19() = 0;
    virtual s32 func_8004667C();
    virtual s32 func_800465BC();

    void func_80046868();
    void func_80046450(s32 arg1);
    s32 func_80046570();
    s32 func_800465C4();
    void func_80046684(const char* src);
    void func_80046700(s32 arg1);
    s32 func_800467B8(s32 idx);
    f32 func_80045F40();
    s32 func_800461F0();
    u8* func_8004698C();
    s32 func_80046994();
    f32 func_800469A0();
    f32 func_800469AC();
    s32 func_800469B8();
    s32 func_800469C4();
    u8* func_800469D0();
    u8* func_800469DC();
    u8* func_80046A14();
    void func_80046A4C();
    void func_80046A68();
    void func_80046A84();
    void func_80046AA0();
    void func_80046ABC(s32 arg1);
};

extern "C" {
extern char* strncpy(char*, const char*, s32);
extern unsigned strlen(const char*);
extern char* strcat(char*, const char*);
extern int sprintf(char*, const char*, ...);
extern u8 D_80004C10;
extern u8* D_80004C14;
extern void* D_800763F8;
extern char* D_80076640;
extern u32 D_80076644;
extern s32 func_800079A8(const char*, s32, s32, s32);
}

const u32 D_80004D50[] = { 7, 4, 8, 8 };

INCLUDE_RODATA("asm/nonmatchings/parrot", D_80004A80);

INCLUDE_RODATA("asm/nonmatchings/parrot", D_80004C10);

INCLUDE_RODATA("asm/nonmatchings/parrot", D_80004C14);

// reg-alloc differences in the unk1FC state-machine read
#ifdef NON_MATCHING
f32 Parrot::func_80045F40() {
    if (this->unk30 != 0) {
        this->unk30 = 0;
        return this->unk40;
    }
    s32 cur1E8 = this->unk1E8;
    ParrotDriver* driver = this->unk64C;
    u32 total = cur1E8 + this->fileOffset;
    u32 base = driver->unk18;
    s32 raw;
    if (total >= base && driver->unk1C >= total + 5) {
        s32 pos = total - base;
        s32 op;
        if (this->unk1F4 != 0) {
            do {
                if (this->unk200) {
                    op = this->unk20C[this->unk204][this->unk208];
                    s32 v = this->unk208 + 1;
                    this->unk208 = v;
                    if ((u32)v < this->unk60C[this->unk204])
                        break;
                    this->unk208 = 0;
                } else if (this->unk1FC) {
                    op = D_80004D50[this->unk208];
                    s32 v = this->unk208 + 1;
                    this->unk208 = v;
                    if ((u32)v < 4)
                        break;
                    this->unk208 = 0;
                } else {
                    op = this->unk1F8;
                }
                if (--this->unk1F4 == 0) {
                    this->unk200 = 0;
                    this->unk1FC = 0;
                }
            } while (0);
        } else {
            op = this->unk64C->unk20[pos];
            pos++;
            this->unk1E8 = cur1E8 + 1;
        }
        this->unk34 = 3;
        switch (op) {
            case 3: {
                u8* p = this->unk64C->unk20 + pos;
                u32 b0 = p[0];
                u32 b1 = p[1];
                u32 b2 = p[2];
                u32 b3 = p[3];
                raw = b0 + (b1 << 8) + (b2 << 16) + (b3 << 24);
                s32 oldUnk1E8 = this->unk1E8;
                f32 fval = *(f32*)&raw;
                this->unk1E8 = oldUnk1E8 + 4;
                this->unk40 = fval;
                return fval;
            }
            case 15: {
                u8* p = this->unk64C->unk20 + pos;
                u32 b0 = p[0];
                u32 b1 = p[1];
                s16 v = (s16)(b0 + (b1 << 8));
                f32 fval = (f32)v * 0.000244140625f;
                this->unk1E8 += 2;
                this->unk40 = fval;
                return fval;
            }
            case 16: {
                u8* p = this->unk64C->unk20 + pos;
                u32 b0 = p[0];
                u32 b1 = p[1];
                s16 v = (s16)(b0 + (b1 << 8));
                f32 fval = (f32)v * 0.03125f;
                this->unk1E8 += 2;
                this->unk40 = fval;
                return fval;
            }
            case 17: {
                u8* p = this->unk64C->unk20 + pos;
                u32 b0 = p[0];
                u32 b1 = p[1];
                s16 v = (s16)(b0 + (b1 << 8));
                f32 fval = (f32)v;
                this->unk1E8 += 2;
                this->unk40 = fval;
                return fval;
            }
            case 18: {
                raw = this->unk64C->unk20[pos];
                f32 fval = (f32)raw * 0.007874015719f;
                this->unk1E8 += 1;
                this->unk40 = fval;
                return fval;
            }
        }
        this->unk1E8 = cur1E8;
    }
    this->func_8004667C();
    return this->unk40;
}
#else
INCLUDE_ASM("asm/nonmatchings/parrot", func_80045F40__6Parrot);
#endif

// case 4 accumulator goes to v0 instead of target's a0 (avoids `move v0, a0` at end)
#ifdef NON_MATCHING
s32 Parrot::func_800461F0() {
    if (this->unk30 != 0) {
        this->unk30 = 0;
        return this->unk38;
    }
    s32 cur1E8 = this->unk1E8;
    ParrotDriver* driver = this->unk64C;
    u32 total = cur1E8 + this->fileOffset;
    u32 base = driver->unk18;
    if (total >= base && driver->unk1C >= total + 5) {
        s32 pos = total - base;
        s32 op;
        if (this->unk1F4 != 0) {
            do {
                if (this->unk200) {
                    op = this->unk20C[this->unk204][this->unk208];
                    s32 v = this->unk208 + 1;
                    this->unk208 = v;
                    if ((u32)v < this->unk60C[this->unk204])
                        break;
                    this->unk208 = 0;
                } else if (this->unk1FC) {
                    op = D_80004D50[this->unk208];
                    s32 v = this->unk208 + 1;
                    this->unk208 = v;
                    if ((u32)v < 4)
                        break;
                    this->unk208 = 0;
                } else {
                    op = this->unk1F8;
                }
                if (--this->unk1F4 == 0) {
                    this->unk200 = 0;
                    this->unk1FC = 0;
                }
            } while (0);
        } else {
            op = this->unk64C->unk20[pos];
            pos++;
            this->unk1E8 = cur1E8 + 1;
        }
        this->unk34 = 4;
        switch (op) {
            case 4: {
                s32 oldUnk1E8 = this->unk1E8;
                u8* p = this->unk64C->unk20 + pos;
                u32 b0 = p[0];
                u32 b1 = p[1];
                u32 b2 = p[2];
                u32 b3 = p[3];
                this->unk1E8 = oldUnk1E8 + 4;
                return this->unk38 = b0 + (b1 << 8) + (b2 << 16) + (b3 << 24);
            }
            case 11:
            case 12: {
                this->unk1E8++;
                return this->unk38 = this->unk64C->unk20[pos];
            }
            case 13: {
                u8* p = this->unk64C->unk20 + pos;
                u32 b0 = p[0];
                u32 b1 = p[1];
                this->unk1E8 += 2;
                return this->unk38 = (s16)(b0 + (b1 << 8));
            }
            case 14: {
                u8* p = this->unk64C->unk20 + pos;
                u32 b0 = p[0];
                u32 b1 = p[1];
                this->unk1E8 += 2;
                return this->unk38 = b0 | (b1 << 8);
            }
        }
        this->unk1E8 = cur1E8;
    }
    this->func_8004667C();
    return this->unk38;
}
#else
INCLUDE_ASM("asm/nonmatchings/parrot", func_800461F0__6Parrot);
#endif

void Parrot::func_80046450(s32 arg1) {
    if (this->unk30 != 0) {
        this->unk30 = 0;
        return;
    }
    u32 base = this->unk1E8;
    ParrotDriver* driver = this->unk64C;
    u32 total = base + this->fileOffset;
    if (total < driver->unk18 || driver->unk1C < total + 1) {
        this->func_8004667C();
        return;
    }
    this->unk34 = arg1;
    if (this->unk1F4 != 0) {
        if (this->unk200) {
            s32 v = ++this->unk208;
            if ((u32)v < (u32)this->unk60C[this->unk204])
                return;
            this->unk208 = 0;
        } else if (this->unk1FC) {
            s32 v = ++this->unk208;
            if ((u32)v < 4)
                return;
            this->unk208 = 0;
        }
        if (--this->unk1F4 == 0) {
            this->unk200 = 0;
            this->unk1FC = 0;
        }
        return;
    }
    ParrotDriver* d2 = this->unk64C;
    if (d2->unk20[total - d2->unk18] == arg1) {
        this->unk1E8++;
        return;
    }
    this->func_8004667C();
}

s32 Parrot::func_80046570() {
    this->func_80046450(7);
    this->func_800461F0();
    this->func_80046450(8);
    this->func_80046450(5);
    return this->unk38;
}

s32 Parrot::func_800465BC() {
    return 0;
}

s32 Parrot::func_800465C4() {
    s32 ret;
    if (this->unk200) {
        s32 idx = this->unk208;
        ret = this->unk20C[this->unk204][idx];
        idx++;
        this->unk208 = idx;
        if ((u32)idx < (u32)this->unk60C[this->unk204]) {
            return ret;
        }
        this->unk208 = 0;
    } else if (this->unk1FC) {
        s32 idx = this->unk208;
        ret = D_80004D50[idx];
        idx++;
        this->unk208 = idx;
        if ((u32)idx < 4u) {
            return ret;
        }
        this->unk208 = 0;
    } else {
        ret = this->unk1F8;
    }
    this->unk1F4--;
    if (this->unk1F4 == 0) {
        this->unk200 = 0;
        this->unk1FC = 0;
    }
    return ret;
}

s32 Parrot::func_8004667C() {
    return 0;
}

void Parrot::func_80046684(const char* src) {
    strncpy(this->unk1EC, src, 5);
    this->unk1F0[0] = 0;
}

char* Parrot::func_800466B4() {
    if (this->unk1EC[0]) {
        return this->unk1EC;
    }
    return (char*)&D_80004C14;
}

void Parrot::func_800466D0() {
    s32 idx = this->archiveIndex * 13;
    this->unk64C = ((ParrotDriver**)D_800763F8)[idx];
}

void Parrot::func_80046700(s32 arg1) {
    if (this->unk1A4 != NULL) {
        s32 total = strlen((char*)this->unk1A4) + strlen(D_80076640) + strlen((char*)AbstractFile::getDeviceParam(arg1));
        ((char*)this)[0xA4] = 0;
        if (total < 0xFF) {
            sprintf((char*)this + 0xA4, D_80076640, this->unk1A4);
        }
        strcat((char*)this + 0xA4, (char*)AbstractFile::getDeviceParam(arg1));
    }
    func_800079A8((const char*)&D_80004C10, 0, 0, 0);
}

s32 Parrot::func_800467B8(s32 idx) {
    if (idx >= 0x14) {
        func_800079A8((const char*)&D_80004C10, 0, 0, 0);
    }
    return ((s32*)&D_80076644)[idx];
}

s32 Parrot::close() {
    s32 ret = this->AbstractFile::close();
    if (this->unk1A4 != NULL && this->unk1A4 != this->inlineBuf) {
        delete[] this->unk1A4;
        this->unk1A4 = NULL;
    }
    return ret;
}

void Parrot::func_80046868() {
    this->unk30 = 0;
    this->unk34 = 0;
    this->unk44[0] = 0;
    this->unk84[0] = 0;
    this->unk1A4 = NULL;
    memset(this->unk1EC, 0, 5);
    this->unk1F4 = 0;
    this->unk1F8 = 0;
    this->unk1FC = 0;
    this->unk200 = 0;
    this->unk204 = 0;
    this->unk208 = 0;
    memset(&this->unk20C[0][0], 0, 0x400);
    memset(this->unk60C, 0, 0x40);
    this->unk64C = NULL;
}

Parrot::~Parrot() {
    if (this->unk1A4 != NULL && this->unk1A4 != this->inlineBuf) {
        delete[] this->unk1A4;
    }
}

Parrot::Parrot() {
    this->func_80046868();
}

u8* Parrot::func_8004698C() {
    return this->unk44;
}

s32 Parrot::func_80046994() {
    return this->unk38;
}

f32 Parrot::func_800469A0() {
    return this->unk40;
}

f32 Parrot::func_800469AC() {
    return this->unk3C;
}

s32 Parrot::func_800469B8() {
    this->unk30 = 1;
    return 1;
}

s32 Parrot::func_800469C4() {
    return this->unk34;
}

u8* Parrot::func_800469D0() {
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

void Parrot::func_80046A4C() {
    this->func_80046450(6);
}

void Parrot::func_80046A68() {
    this->func_80046450(5);
}

void Parrot::func_80046A84() {
    this->func_80046450(8);
}

void Parrot::func_80046AA0() {
    this->func_80046450(7);
}

void Parrot::func_80046ABC(s32 arg1) {
    this->func_80046450(arg1);
}

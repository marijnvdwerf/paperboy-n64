#include "common.h"
#include "structs.h"

struct Parrot : public RomFile {
    /* 0x030 */ s32 unk30;
    /* 0x034 */ s32 unk34;
    /* 0x038 */ s32 unk38;
    /* 0x03C */ f32 unk3C;
    /* 0x040 */ f32 unk40;
    /* 0x044 */ u8 unk44[0x40];
    /* 0x084 */ u8 unk84[0x120];
    /* 0x1A4 */ u8* unk1A4;
    /* 0x1A8 */ u8 inlineBuf[0x44];
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
    /* 0x64C */ struct ParrotDriver* unk64C;
};

struct ParrotDriver {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ u32 unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u8* unk20;
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
extern void func_80046450(Parrot*, s32);
extern void func_800461F0(Parrot*);
extern void func_80046868(Parrot*);
}

INCLUDE_RODATA("asm/nonmatchings/parrot", D_80004A80);

INCLUDE_RODATA("asm/nonmatchings/parrot", D_80004C10);

INCLUDE_RODATA("asm/nonmatchings/parrot", D_80004C14);

INCLUDE_ASM("asm/nonmatchings/parrot", func_80045F40);

INCLUDE_ASM("asm/nonmatchings/parrot", func_800461F0);

INCLUDE_ASM("asm/nonmatchings/parrot", func_80046450);

extern "C" s32 func_80046570(Parrot* self) {
    func_80046450(self, 7);
    func_800461F0(self);
    func_80046450(self, 8);
    func_80046450(self, 5);
    return self->unk38;
}

extern "C" s32 func_800465BC(Parrot* self) {
    return 0;
}

extern u32 D_80004D50[];

#ifdef NON_MATCHING
extern "C" s32 func_800465C4(Parrot* self) {
    s32 ret;
    if (self->unk200) {
        s32 idx = self->unk208;
        ret = self->unk20C[self->unk204][idx];
        idx++;
        self->unk208 = idx;
        if ((u32)idx < (u32)self->unk60C[self->unk204]) {
            return ret;
        }
        self->unk208 = 0;
    } else if (self->unk1FC) {
        s32 idx = self->unk208;
        ret = D_80004D50[idx];
        idx++;
        self->unk208 = idx;
        if ((u32)idx < 4u) {
            return ret;
        }
        self->unk208 = 0;
    } else {
        ret = self->unk1F8;
    }
    self->unk1F4--;
    if (self->unk1F4 == 0) {
        self->unk200 = 0;
        self->unk1FC = 0;
    }
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/parrot", func_800465C4);
#endif

extern "C" s32 func_8004667C(Parrot* self) {
    return 0;
}

extern "C" void func_80046684(Parrot* self, const char* src) {
    strncpy(self->unk1EC, src, 5);
    self->unk1F0[0] = 0;
}

extern "C" char* func_800466B4(Parrot* self) {
    if (self->unk1EC[0]) {
        return self->unk1EC;
    }
    return (char*)&D_80004C14;
}

#ifdef NON_MATCHING
extern "C" void func_800466D0(Parrot* self) {
    self->unk64C = (ParrotDriver*)((void**)D_800763F8)[self->archiveIndex * 13];
}
#else
INCLUDE_ASM("asm/nonmatchings/parrot", func_800466D0);
#endif

extern "C" void func_80046700(Parrot* self, s32 arg1) {
    if (self->unk1A4 != NULL) {
        s32 total = strlen((char*)self->unk1A4) + strlen(D_80076640) + strlen((char*)AbstractFile::getDeviceParam(arg1));
        ((char*)self)[0xA4] = 0;
        if (total < 0xFF) {
            sprintf((char*)self + 0xA4, D_80076640, self->unk1A4);
        }
        strcat((char*)self + 0xA4, (char*)AbstractFile::getDeviceParam(arg1));
    }
    func_800079A8((const char*)&D_80004C10, 0, 0, 0);
}

extern "C" s32 func_800467B8(Parrot* self, s32 idx) {
    if (idx >= 0x14) {
        func_800079A8((const char*)&D_80004C10, 0, 0, 0);
    }
    return ((s32*)&D_80076644)[idx];
}

#ifdef NON_MATCHING
extern "C" s32 func_8004680C(Parrot* self) {
    s32 ret = (s32)self->AbstractFile::close();
    if (self->unk1A4 != NULL && self->unk1A4 != self->inlineBuf) {
        delete[] self->unk1A4;
        self->unk1A4 = NULL;
    }
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/parrot", func_8004680C);
#endif

extern "C" void func_80046868(Parrot* self) {
    self->unk30 = 0;
    self->unk34 = 0;
    self->unk44[0] = 0;
    self->unk84[0] = 0;
    self->unk1A4 = NULL;
    memset(self->unk1EC, 0, 5);
    self->unk1F4 = 0;
    self->unk1F8 = 0;
    self->unk1FC = 0;
    self->unk200 = 0;
    self->unk204 = 0;
    self->unk208 = 0;
    memset(&self->unk20C[0][0], 0, 0x400);
    memset(self->unk60C, 0, 0x40);
    self->unk64C = NULL;
}

INCLUDE_ASM("asm/nonmatchings/parrot", _._6Parrot);

INCLUDE_ASM("asm/nonmatchings/parrot", __6Parrot);

extern "C" u8* func_8004698C(Parrot* self) {
    return self->unk44;
}

extern "C" s32 func_80046994(Parrot* self) {
    return self->unk38;
}

extern "C" f32 func_800469A0(Parrot* self) {
    return self->unk40;
}

extern "C" f32 func_800469AC(Parrot* self) {
    return self->unk3C;
}

extern "C" s32 func_800469B8(Parrot* self) {
    self->unk30 = 1;
    return 1;
}

extern "C" s32 func_800469C4(Parrot* self) {
    return self->unk34;
}

extern "C" u8* func_800469D0(Parrot* self) {
    return self->unk1A4;
}

INCLUDE_ASM("asm/nonmatchings/parrot", func_800469DC);

INCLUDE_ASM("asm/nonmatchings/parrot", func_80046A14);

extern "C" void func_80046A4C(Parrot* self) {
    func_80046450(self, 6);
}

extern "C" void func_80046A68(Parrot* self) {
    func_80046450(self, 5);
}

extern "C" void func_80046A84(Parrot* self) {
    func_80046450(self, 8);
}

extern "C" void func_80046AA0(Parrot* self) {
    func_80046450(self, 7);
}

extern "C" void func_80046ABC(Parrot* self, s32 arg1) {
    func_80046450(self, arg1);
}

INCLUDE_RODATA("asm/nonmatchings/parrot", _vt.6Parrot);

INCLUDE_RODATA("asm/nonmatchings/parrot", D_80004D50);

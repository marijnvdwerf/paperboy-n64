#include "structs.h"

extern "C" {
#include <PR/os_vi.h>
extern s32 D_80072BC0;
extern void func_800079A8(const char*, s32, s32, s32);
extern void func_8004B390(void);
extern void func_8004B3BC(s32);
void* memcpy(void* dst, const void* src, unsigned n);
void* memset(void* p, s32 v, unsigned n);
}

struct StructYYSubA8Ctx {
    /* 0x0000 */ char pad[0xD03C];
    /* 0xD03C */ s32 unkD03C;
};

void StructYYSubA8Inner1::func_8002BDA0(u32 color) {
    u32* start = (u32*)this->unk18;
    u32* end = start + ((this->unk26 * this->unk28) >> 1);
    u32 doubleColor = (color << 16) | (color & 0xFFFF);
    while (start < end) {
        *start = doubleColor;
        start++;
    }
    start = (u32*)this->unk38;
    end = start + ((this->unk26 * this->unk28) >> 1);
    while (start < end) {
        *start = doubleColor;
        start++;
    }
}

void StructYYSubA8Inner1::vfunc13(void*, s16, s16, s32) {
    memcpy((void*)this->unk38, (void*)this->unk18, this->unk20 * this->unk28);
}

void StructYYSubA8Inner1::vfunc2(s32* outAddr, s32* outPitch, s32 mode) {
    *outPitch = this->unk20;
    *outAddr = this->unk38;
    u16 v = this->unk22;
    this->unk22 = v | 2;
    if (mode & 1) {
        this->unk22 = v | 0xA;
    }
    if (mode & 2) {
        this->unk22 = this->unk22 | 0x10;
    }
}

void StructYYSubA8Inner1::vfunc6() {
    s32 t = this->unk18;
    this->unk18 = this->unk38;
    this->unk38 = t;
}

extern "C" void func_8002BECC(s32 v) {
    D_80072BC0 = v;
}

void StructYYSubA8Inner1::vfunc15() {
    if (this->unk38) {
        delete[] (u8*)this->unk38;
        this->unk38 = 0;
    }
    if (this->unk18) {
        delete[] (u8*)this->unk18;
        this->unk18 = 0;
    }
    this->unk26 = 0;
    this->unk28 = 0;
    this->unk20 = 0;
    this->Surface177B0::vfunc15();
}

void StructYYSubA8Inner1::vfunc14(void* buf, u16 w, u16 h, u32 bpp) {
    StructYYSubA8Ctx* ctx = (StructYYSubA8Ctx*)buf;
    if (this->unk22 & 1) {
        this->vfunc15();
    }
    this->unk26 = w;
    this->unk22 = 1;
    this->unk30 = buf;
    this->unk28 = h;
    this->unk38 = 0;
    this->hdr.unk0 = 0xF800;
    this->hdr.unk4 = 0x7C0;
    this->hdr.unk8 = 0x3E;
    this->unk20 = this->unk26 * bpp >> 3;
    this->hdr.unkC = 1;
    this->hdr.unk16 = 0x10;
    u32 size = this->unk20 * this->unk28;
    func_8004B3BC(D_80072BC0);
    this->unk18 = (u32) new u8[size];
    func_8004B390();
    if (!this->unk18) {
        func_800079A8("", 0, 0, 0);
    }
    memset((void*)this->unk18, 0, size);
    if (ctx->unkD03C != 0) {
        osViBlack(0);
        ctx->unkD03C = 0;
    }
    osViSwapBuffer((void*)this->unk18);
    func_8004B3BC(D_80072BC0);
    this->unk38 = (s32) new u8[size];
    func_8004B390();
    if (!this->unk38) {
        func_800079A8("", 0, 0, 0);
    }
}

StructYYSubA8Inner1::~StructYYSubA8Inner1() {
    this->vfunc15();
}

StructYYSubA8Inner1::StructYYSubA8Inner1() {
    this->unk38 = 0;
}

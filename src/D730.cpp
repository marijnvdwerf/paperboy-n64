#include "common.h"
#include "structs.h"

// TODO: figure out the real type of StructYYHandler::unk1C — fingerprint:
// has a vtable at +0x12C, slot 6 returns s32.
class StructYYHandlerInner {
  public:
    /* 0x000 */ char pad[0x12C];
    /* 0x12C */ // vtable

    virtual void vfunc1() = 0;
    virtual void vfunc2() = 0;
    virtual void vfunc3() = 0;
    virtual void vfunc4() = 0;
    virtual void vfunc5() = 0;
    virtual s32 vfunc6() = 0;
};

extern "C" {
extern s32 D_80000300;
StructYYHandler* func_80039420(StructYYInner*);
void func_80047B94(StructYYSubD0E8*);
void func_80046BC0(StructYYSubD0E8*);
void func_80047A84(StructYYSubD0E8*, u32);
void func_800079A8(void*, s32, s32, s32);
extern u8 D_1106A0[];
extern u8 D_BDE740[];
extern s32 D_80000B70;
extern JamArchive* D_800763F8;
extern s32 D_800763FC;
}

INCLUDE_ASM("asm/nonmatchings/D730", func_8000CB30);

INCLUDE_ASM("asm/nonmatchings/D730", func_8000CB60);

s32 StructYY::vfunc14(s32 flags) {
    s32 v = 0x80;
    if (flags & 0x10) {
        v = 0x1080;
    }
    if (flags & 0x40) {
        v |= 0x8;
    }
    if (D_80000300 == 0) {
        v |= 0x10;
    } else if (D_80000300 == 2) {
        v |= 0x100000;
    }
    return v;
}

StructYYSubD0E8* StructYY::vfunc13() {
    return &this->subD0E8;
}

s32 StructYY::vfunc12() {
    OSTime t = osGetTime();
    u32 lo = t;
    u64 dt = lo - this->unk11AA8;
    this->unk90 = dt / 0xB71B;
    if (this->unk94 < this->unk90) {
        this->unk90 = this->unk94;
    }
    this->unk11AA8 = lo;
    this->unk8C = lo / 1000000;
    func_80047A84(&this->subD0E8, this->unk90);
    return 1;
}

void StructYY::vfunc11() {
}

s32 StructYY::vfunc9(s32 a, s32 b, s32 c, s32 d) {
    if (this->unk0 & 2) {
        this->vfunc7();
        this->vfunc10();
    }
    this->unk7C = a;
    this->unk80 = b;
    this->unk84 = c;
    this->unk0 = (this->unk0 & ~0x7A) | (d & 0x78);
    s32 r = this->vfunc14(this->unk0);
    s32 result = this->unk70->vfunc4(a, b, c, r);
    StructYYHandlerInner* inner = *(StructYYHandlerInner**)((u8*)this->unk70 + 0x1C);
    this->unk78 = (s32)inner;
    this->unk74 = inner->vfunc6();
    this->unk0 |= 2;
    this->vfunc6();
    return result;
}

void StructYY::func_8000CDD8() {
    LocalIO2* io = &this->io[0];
    if (io->func_80048A40((s32)D_1106A0, D_BDE740 - D_1106A0, 0x82, 0x8000)) {
        func_800079A8(&D_80000B70, 0, 0, 0);
    }
    JamArchive* archive = &this->archives[0];
    this->unk68 = 1;
    archive->open(io);
    D_800763F8 = archive;
    D_800763FC = 1;
}

void StructYY::vfunc5() {
    this->unk6C = 0;
    this->unk70 = NULL;
}

void StructYY::vfunc4() {
    this->unk6C = &this->inner;
    this->unk70 = func_80039420(&this->inner);
}

void StructYY::vfunc7() {
    func_80047B94(&this->subD0E8);
}

void StructYY::vfunc6() {
    StructYYSubD0E8* sub = &this->subD0E8;
    sub->unk47AC = (u8*)this->unk70 + 0x20;
    func_80046BC0(sub);
}

void StructYY::vfunc3() {
    this->vfunc10();
    this->unk0 = 0;
}

void StructYY::vfunc10() {
    StructYYHandler* h = this->unk70;
    this->unk74 = 0;
    this->unk78 = 0;
    if (h != NULL && (h->unkC & 1)) {
        h->vfunc5();
    }
    this->unk0 &= ~2;
}

void StructYY::vfunc2(const char* name, s32 doInit) {
    if (this->unk0 & 1) {
        this->vfunc3();
    }
    if (doInit) {
        this->func_8000CDD8();
    }
    this->vfunc4();
    this->unk0 |= 1;
    u32 t = osGetTime();
    this->unk11AA8 = t;
    this->unk8C = t / 1000000;
}

INCLUDE_ASM("asm/nonmatchings/D730", _._8StructYY);

StructYY::StructYY() {
    this->unk11AA8 = 0;
}

INCLUDE_ASM("asm/nonmatchings/D730", _._17StructYYInnerBase);

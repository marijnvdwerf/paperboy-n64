#include "common.h"
#include "peacock.h"

void Peacock::func_80020F40(PeacockExport* dst) {
    dst->flags = this->flags;
    dst->linked = this->linked;
    char* d = (char*)dst;
    memcpy(d + 8, this->color2.c, 4);
    memcpy(d + 12, this->color1.c, 4);
    dst->unk10 = this->unk1C;
    dst->unk11 = this->unk1D;
    dst->unk12 = this->unk1E;
    dst->unk12 = this->unk1E;
    dst->unk13 = this->unk1F;
    dst->unk13 = this->unk1F;
}

void Peacock::func_80020FA4(PeacockContext* ctx, PeacockExport* src) {
    s32 cond;
    this->color2 = src->color2;
    this->color1 = src->color1;
    this->flags = src->flags;
    this->unk1C = src->unk10;
    this->unk1D = src->unk11;
    this->unk1E = src->unk12;
    this->unk1E = src->unk12;
    this->unk1F = src->unk13;
    this->unk1F = src->unk13;

    if (this->flags & 0x8) {
        this->linked = src->linked;
        cond = 0;
        if (this->linked->unk36 & 0x20) {
            u32 masked = ctx->contextFlags & 0x180;
            cond = cond < masked;
        }
        if (cond) {
            cond = 0;
            if (ctx->contextFlags & 0x80) {
                u32 tmp = this->flags & 0x1000;
                cond = tmp < 1;
            }
            if (cond) {
                this->unk1E = 6;
                this->unk1F = 8;
                this->flags = (this->flags | 0x112) & ~0x224;
            }
            if (ctx->contextFlags & 0x100) {
                this->unk1C = 2;
                this->unk1D = 0;
                this->flags = (this->flags | 0x40) & ~0x80;
            }
        }
    } else {
        this->linked = NULL;
    }
}

Peacock::~Peacock() {
}

Peacock::Peacock() {
    this->color1.c[0] = (char)0xFF;
    this->color1.c[1] = (char)0xFF;
    this->color1.c[2] = (char)0xFF;
    this->color1.c[3] = (char)0xFF;
    this->color2.c[0] = (char)0xFF;
    this->color2.c[1] = (char)0xFF;
    this->color2.c[2] = (char)0xFF;
    this->color2.c[3] = (char)0xFF;
    this->unk10 = 0;
    this->flags = 0x50A694;
    this->linked = NULL;
    this->unk1D = 0;
    this->unk1C = 0;
    this->unk1F = 0;
    this->unk1E = 1;
    this->unk1F = 0;
}

u32 Peacock::func_80021148() {
    return this->flags & 0x200000;
}

u32 Peacock::func_80021158() {
    return this->flags & 0x80000;
}

u32 Peacock::func_80021168() {
    if (this->linked == NULL) {
        return 1;
    }
    return this->linked->unk34;
}

void Peacock::func_80021180(PeacockLinked* linked) {
    this->linked = linked;
}

PeacockLinked* Peacock::func_80021188() {
    return this->linked;
}

u32 Peacock::func_80021194() {
    return this->flags & 0x40000;
}

s32 Peacock::func_800211A4() {
    return this->unk10;
}

void Peacock::func_800211B0(s32 val) {
    this->unk10 = val;
    this->flags |= 0x40000;
}

u32 Peacock::func_800211C8() {
    return this->unk1E;
}

void Peacock::func_800211D4(u8* src) {
    this->unk1F = *src;
}

u32 Peacock::func_800211E0() {
    return this->unk1F;
}

void Peacock::func_800211EC(char* dst) {
    memcpy(dst, this->unk14, 8);
}

void Peacock::func_80021214(char* src) {
    memcpy(this->unk14, src, 8);
}

u32 Peacock::func_8002123C() {
    return this->unk1F;
}

u32 Peacock::func_80021248() {
    return this->unk1E;
}

u32 Peacock::func_80021254() {
    return this->flags & 0x4000;
}

u32 Peacock::func_80021260() {
    return this->flags & 0x1100;
}

u32 Peacock::func_8002126C() {
    return this->flags & 0x100;
}

u32 Peacock::func_80021278() {
    return this->flags & 0x40;
}

u32 Peacock::func_80021284() {
    return this->flags & 0x1000;
}

u32 Peacock::func_80021290() {
    return this->flags & 0x800;
}

u32 Peacock::func_8002129C() {
    return this->flags & 0x20000;
}

u32 Peacock::func_800212AC() {
    return this->flags & 0x10000;
}

u32 Peacock::func_800212BC() {
    return this->flags & 0x8000;
}

u32 Peacock::func_800212C8() {
    return this->flags & 0x400;
}

u32 Peacock::func_800212D4() {
    return this->flags & 0x20;
}

u32 Peacock::func_800212E0() {
    return this->flags & 0x10;
}

u32 Peacock::func_800212EC() {
    return this->flags & 0x8;
}

u32 Peacock::func_800212F8() {
    return this->flags & 0x4;
}

u32 Peacock::func_80021304() {
    return this->flags & 0x2;
}

u32 Peacock::func_80021310() {
    return (this->flags & 0x6) == 0;
}

u32 Peacock::func_80021320() {
    return this->unk1C;
}

u32 Peacock::func_8002132C() {
    return this->unk1D;
}

void Peacock::func_80021338(char* dst) {
    memcpy(dst, this->color1.c, 4);
}

void Peacock::func_80021350(char* dst) {
    memcpy(dst, this->color2.c, 4);
}

u32 Peacock::func_80021368() {
    return this->flags & 0x1;
}

u32 Peacock::func_80021374() {
    return this->flags;
}

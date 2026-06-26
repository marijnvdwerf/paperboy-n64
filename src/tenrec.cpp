#include "common.h"
#include "tenrec.h"

extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390(void);

extern "C" s32 D_800740C0;

void Tenrec::func_8003B2B0() {
    if (this->unk18 != NULL) {
        delete[] this->unk18;
        this->unk18 = NULL;
    }
    this->unk22 = 0;
    this->unk26 = 0;
    this->unk28 = 0;
    this->unk20 = 0;
    this->unk30 = NULL;
}

void Tenrec::func_8003B2FC(s32 unused, Surface16970* src) {
    this->unk22 = 1;
    this->unk26 = src->unk26;
    this->unk20 = this->unk26 * 2;
    this->unk28 = src->unk28;
    this->unk30 = src;
    this->hdr.bitDepth = 0x10;
    u32 size = this->unk20 * this->unk28;
    func_8004B3BC(D_800740C0);
    this->unk18 = new u8[size];
    func_8004B390();
    if (this->unk18 == NULL) {
        __assert("", 0, 0, 0);
    }
}

void Tenrec::func_8003B3A0(s32 val) {
    D_800740C0 = val;
}

Tenrec::~Tenrec() {
    this->func_8003B2B0();
}

Tenrec::Tenrec() {
    this->unk30 = NULL;
}

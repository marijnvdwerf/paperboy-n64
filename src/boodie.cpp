#include "boodie.h"
#include "parrot.h"

extern "C" {
extern s32 D_80070B70;

void func_800284D8(s32 arg);
void func_8004B390(void);
void func_8004B3BC(s32);
}

void Boodie::vfunc2(Parrot* parrot) {
    if (this->unk0 != 0) {
        this->func_80016C80();
    }
    this->unk0 = parrot->beginArray();
    func_8004B3BC(D_80070B70);
    this->unk8 = new BoodieEntry[this->unk0];
    func_8004B390();
    if (this->unk8 == NULL) {
        __assert("", 0, 0, 0);
    }
    memset(this->unk8, 0, this->unk0 * sizeof(BoodieEntry));
    for (u32 i = 0; i < this->unk0; i++) {
        this->unk8[i].unk1 = parrot->readInt();
        this->unk8[i].unk2 = parrot->readInt();
        this->unk8[i].unk3 = parrot->readInt();
        this->unk8[i].unk0 = 0;
    }
    if (parrot->nextToken() != TOKEN_CLOSE_BRACE) {
        parrot->parseError(TOKEN_CLOSE_BRACE);
    }
}

void func_800284D8(s32 arg) {
    D_80070B70 = arg;
}

void Boodie::func_80016C80() {
    if (this->unk8 != NULL) {
        delete[] this->unk8;
        this->unk8 = NULL;
    }
}

void Boodie::func_8002851C(s32 count) {
    if (this->unk0 != 0) {
        this->func_80016C80();
    }
    func_8004B3BC(D_80070B70);
    this->unk0 = count;
    this->unk8 = new BoodieEntry[count];
    func_8004B390();
    if (this->unk8 == NULL) {
        __assert("", 0, 0, 0);
    }
    memset(this->unk8, 0, this->unk0 * sizeof(BoodieEntry));
}

Boodie::~Boodie() {
    this->func_80016C80();
}

Boodie::Boodie() {
    this->unk8 = NULL;
}

BoodieEntry* Boodie::func_8002863C() {
    return this->unk8;
}

void Boodie::func_80028648(s32 idx, const BoodieEntry* src) {
    this->unk8[idx].unk1 = src->unk1;
    this->unk8[idx].unk2 = src->unk2;
    this->unk8[idx].unk3 = src->unk3;
}

BoodieEntry* Boodie::func_80028680(s32 idx) {
    return &this->unk8[idx];
}

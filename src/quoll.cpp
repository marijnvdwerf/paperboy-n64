#include "quoll.h"

void Quoll::func_80016C80() {
    this->unk0 = 0;
}

Quoll::~Quoll() {
    this->func_80016C80();
}

Quoll::Quoll() {
    this->unk0 = 0;
}

u32 Quoll::func_80016CF0() {
    return this->unk0;
}

s32 Quoll::func_80016CFC() {
    return this->unk0 != 0;
}

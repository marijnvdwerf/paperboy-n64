#include "gdb_model_index_array_base.h"

void GdbModelIndexArrayBase::func_80016C80() {
    this->unk0 = 0;
}

GdbModelIndexArrayBase::~GdbModelIndexArrayBase() {
    this->func_80016C80();
}

GdbModelIndexArrayBase::GdbModelIndexArrayBase() {
    this->unk0 = 0;
}

u32 GdbModelIndexArrayBase::func_80016CF0() {
    return this->unk0;
}

s32 GdbModelIndexArrayBase::func_80016CFC() {
    return this->unk0 != 0;
}

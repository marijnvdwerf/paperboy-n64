#include "gdb_model_index_array.h"
#include "gol_file_parser.h"

extern "C" {
extern s32 D_80070B70;

void func_800284D8(s32 arg);
void func_8004B390(void);
void func_8004B3BC(s32);
}

void GdbModelIndexArray::vfunc2(GolFileParser* parser) {
    if (this->unk0 != 0) {
        this->func_80016C80();
    }
    this->unk0 = parser->beginArray();
    func_8004B3BC(D_80070B70);
    this->unk8 = new GdbModelIndexArray::Indices[this->unk0];
    func_8004B390();
    if (this->unk8 == NULL) {
        __assert("", 0, 0, 0);
    }
    memset(this->unk8, 0, this->unk0 * sizeof(GdbModelIndexArray::Indices));
    for (u32 i = 0; i < this->unk0; i++) {
        this->unk8[i].unk1 = parser->readInt();
        this->unk8[i].unk2 = parser->readInt();
        this->unk8[i].unk3 = parser->readInt();
        this->unk8[i].unk0 = 0;
    }
    if (parser->nextToken() != TOKEN_CLOSE_BRACE) {
        parser->parseError(TOKEN_CLOSE_BRACE);
    }
}

void func_800284D8(s32 arg) {
    D_80070B70 = arg;
}

void GdbModelIndexArray::func_80016C80() {
    if (this->unk8 != NULL) {
        delete[] this->unk8;
        this->unk8 = NULL;
    }
}

void GdbModelIndexArray::func_8002851C(s32 count) {
    if (this->unk0 != 0) {
        this->func_80016C80();
    }
    func_8004B3BC(D_80070B70);
    this->unk0 = count;
    this->unk8 = new GdbModelIndexArray::Indices[count];
    func_8004B390();
    if (this->unk8 == NULL) {
        __assert("", 0, 0, 0);
    }
    memset(this->unk8, 0, this->unk0 * sizeof(GdbModelIndexArray::Indices));
}

GdbModelIndexArray::~GdbModelIndexArray() {
    this->func_80016C80();
}

GdbModelIndexArray::GdbModelIndexArray() {
    this->unk8 = NULL;
}

GdbModelIndexArray::Indices* GdbModelIndexArray::func_8002863C() {
    return this->unk8;
}

void GdbModelIndexArray::func_80028648(s32 idx, const GdbModelIndexArray::Indices* src) {
    this->unk8[idx].unk1 = src->unk1;
    this->unk8[idx].unk2 = src->unk2;
    this->unk8[idx].unk3 = src->unk3;
}

GdbModelIndexArray::Indices* GdbModelIndexArray::func_80028680(s32 idx) {
    return &this->unk8[idx];
}

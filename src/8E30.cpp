#include "common.h"
#include "structs.h"

StructTT::~StructTT() {
}

void StructTT::func_80008260() {
    unk8 = 0;
    unkC = 0;
}

void StructTT::func_8000826C() {
    unk8 = 1;
    unkC = 0;
}

StructTT::StructTT() {
    unk8 = 0;
}

void StructTT::func_80008294() {
    unkC = 0;
}

u32 StructTT::func_8000829C() {
    return unk8;
}

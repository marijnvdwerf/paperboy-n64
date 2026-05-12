#include "common.h"
#include "structs.h"

s32 StructYYSubA8Base::vfunc26() {
    return 0;
}

s32 StructYYSubA8Base::vfunc30() {
    return 0;
}

s32 StructYYSubA8Base::vfunc27() {
    return 1;
}

s32 StructYYSubA8Base::vfunc25() {
    return 0;
}

s32 StructYYSubA8Base::vfunc29() {
    return 1;
}

s32 StructYYSubA8Base::vfunc28() {
    return 1;
}

s32 StructYYSubA8Base::vfunc24() {
    return 1;
}

s32 StructYYSubA8Base::vfunc23() {
    return 1;
}

s32 StructYYSubA8Base::vfunc22() {
    return 1;
}

s32 StructYYSubA8Base::vfunc21() {
    return 1;
}

s32 StructYYSubA8Base::vfunc20() {
    return 1;
}

s32 StructYYSubA8Base::vfunc19() {
    return 1;
}

s32 StructYYSubA8Base::vfunc18() {
    return 1;
}

s32 StructYYSubA8Base::vfunc17() {
    return 1;
}

s32 StructYYSubA8Base::vfunc16() {
    return 1;
}

s32 StructYYSubA8Base::vfunc15() {
    return 1;
}

s32 StructYYSubA8Base::vfunc14() {
    return 1;
}

s32 StructYYSubA8Base::vfunc13() {
    return 1;
}

s32 StructYYSubA8Base::vfunc11() {
    return 0x10;
}

s32 StructYYSubA8Base::vfunc12() {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/11570", func_80010A10);

void StructYYSubA8Base::func_80010A6C(StructYYSubA8Node* node) {
    node->next = this->unk18;
    this->unk18 = node;
}

s32 StructYYSubA8Base::vfunc9(s32 a, s32 b, s32 c, s32 d) {
    StructYYSubA8GrandBase::vfunc9(a, b, c, d);

    s32 ret = this->vfunc10();
    if (ret) {
        return ret;
    }

    StructYYSubA8Node* node = this->unk18;
    while (node != NULL) {
        if (node != this->unk1C) {
            node->vfunc1();
        }
        node = node->next;
    }

    return 0;
}

void StructYYSubA8Base::vfunc8() {
    StructYYSubA8Node* node = this->unk18;
    while (node != NULL) {
        node->vfunc2();
        node = node->next;
    }
    StructYYSubA8GrandBase::vfunc8();
}

void StructYYSubA8Base::vfunc6() {
    StructYYSubA8Node* node = this->unk18;
    while (node != NULL) {
        StructYYSubA8Node* nx = node->next;
        node->vfunc7();
        node = nx;
    }
    StructYYSubA8GrandBase::vfunc6();
}

s32 StructYYSubA8Base::vfunc5(s32 a, s32 b, s32 c, s32 d) {
    s32 ret = StructYYSubA8GrandBase::vfunc5(a, b, c, d);
    if (ret) {
        return ret;
    }

    s32 tmp = this->vfunc10();
    if (tmp) {
        return tmp;
    }

    return 0;
}

StructYYSubA8Base::StructYYSubA8Base() {
    this->unk18 = NULL;
    this->unk1C = NULL;
}

void* StructYYSubA8Base::func_80010C50() {
    return this->unk1C;
}

s32 StructYYSubA8Base::vfunc33() {
    return 0;
}

s32 StructYYSubA8Base::vfunc32() {
    return 0;
}

s32 StructYYSubA8Base::vfunc31() {
    return 0;
}

// INCLUDE_ASM("asm/nonmatchings/11570", _._17StructYYSubA8Base);

#include "common.h"
#include "structs.h"

s32 GolCommonDrawState::vfunc26() {
    return 0;
}

s32 GolCommonDrawState::vfunc30() {
    return 0;
}

s32 GolCommonDrawState::vfunc27() {
    return 1;
}

s32 GolCommonDrawState::vfunc25() {
    return 0;
}

s32 GolCommonDrawState::vfunc29() {
    return 1;
}

s32 GolCommonDrawState::vfunc28() {
    return 1;
}

s32 GolCommonDrawState::vfunc24() {
    return 1;
}

s32 GolCommonDrawState::vfunc23() {
    return 1;
}

s32 GolCommonDrawState::vfunc22() {
    return 1;
}

s32 GolCommonDrawState::vfunc21() {
    return 1;
}

s32 GolCommonDrawState::vfunc20() {
    return 1;
}

s32 GolCommonDrawState::vfunc19() {
    return 1;
}

s32 GolCommonDrawState::vfunc18() {
    return 1;
}

s32 GolCommonDrawState::vfunc17() {
    return 1;
}

s32 GolCommonDrawState::vfunc16() {
    return 1;
}

s32 GolCommonDrawState::vfunc15() {
    return 1;
}

s32 GolCommonDrawState::vfunc14() {
    return 1;
}

s32 GolCommonDrawState::vfunc13() {
    return 1;
}

s32 GolCommonDrawState::vfunc11() {
    return 0x10;
}

s32 GolCommonDrawState::vfunc12() {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/render/gol_common_draw_state", func_80010A10);

void GolCommonDrawState::func_80010A6C(StructYYSubA8Node* node) {
    node->next = this->unk18;
    this->unk18 = node;
}

s32 GolCommonDrawState::vfunc9(s32 a, s32 b, s32 c, s32 d) {
    GolDrawState::vfunc9(a, b, c, d);

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

void GolCommonDrawState::vfunc8() {
    StructYYSubA8Node* node = this->unk18;
    while (node != NULL) {
        node->vfunc2();
        node = node->next;
    }
    GolDrawState::vfunc8();
}

void GolCommonDrawState::vfunc6() {
    StructYYSubA8Node* node = this->unk18;
    while (node != NULL) {
        StructYYSubA8Node* nx = node->next;
        node->vfunc7();
        node = nx;
    }
    GolDrawState::vfunc6();
}

s32 GolCommonDrawState::vfunc5(s32 a, s32 b, s32 c, s32 d) {
    s32 ret = GolDrawState::vfunc5(a, b, c, d);
    if (ret) {
        return ret;
    }

    s32 tmp = this->vfunc10();
    if (tmp) {
        return tmp;
    }

    return 0;
}

GolCommonDrawState::GolCommonDrawState() {
    this->unk18 = NULL;
    this->unk1C = NULL;
}

void* GolCommonDrawState::func_80010C50() {
    return this->unk1C;
}

s32 GolCommonDrawState::vfunc33() {
    return 0;
}

s32 GolCommonDrawState::vfunc32() {
    return 0;
}

s32 GolCommonDrawState::vfunc31() {
    return 0;
}

// INCLUDE_ASM("asm/nonmatchings/render/gol_common_draw_state", _._18GolCommonDrawState);

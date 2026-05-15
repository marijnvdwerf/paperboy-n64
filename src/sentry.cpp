#include "streams.h"

s32 Sentry::vfunc6(FileInfo*) {
    return 6;
}

s32 Sentry::vfunc9(s32, FileInfo*) {
    return 6;
}

s32 Sentry::vfunc8(s32*) {
    return 6;
}

s32 Sentry::vfunc7(s32*, s32*) {
    return 6;
}

Sentry::~Sentry() {
}

Sentry::Sentry() {
    state = 0;
    flag = 0;
}

void Sentry::clearFlag() {
    flag = 0;
}

s32 Sentry::getFlag() {
    return flag;
}

s32 Sentry::getState() {
    return state;
}

s32 Sentry::isInactive() {
    return state == 0;
}

s32 Sentry::activate() {
    state = 1;
    return 1;
}

s32 Sentry::setMode15() {
    state = 0x15;
    return 0x15;
}

s32 Sentry::setMode19() {
    state = 0x19;
    return 0x19;
}

s32 Sentry::setMode14() {
    state = 0x14;
    return 0x14;
}

s32 Sentry::setMode13() {
    flag = 1;
    state = 0x13;
    return 0x13;
}

s32 Sentry::setMode12() {
    state = 0x12;
    return 0x12;
}

void Sentry::reset() {
    state = 0;
}

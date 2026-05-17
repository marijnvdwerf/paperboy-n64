#ifndef WSTRING_H
#define WSTRING_H

#include "common.h"

class WString {
  public:
    /* 0x0 */ u16* data;
    /* 0x4 */ u16 capacity;
    /* 0x6 */ u16 unk6;
    /* 0x8 */ u16 length;

    s32 func_800100C0();
    void func_80010118(u8* dst);
    void func_800101A4(u8* dst);
    void func_800101F0();
    s32 func_800101F8(u16* src);
    s32 func_80010280(WString* src);
    s32 func_800102FC(u16* src);
    s32 func_80010374(WString* src);
    s32 func_800103F0(u16* src);
    s32 func_800104B8(WString* src);
    void func_80010564();
    void func_800105E0();
    void func_80010640();
    s32 func_80010680(WString* src);
    s32 func_800106BC(u16* d, u16 cap);
    u16* func_8001075C(s32 index);
    ~WString();
    WString();

    u16 size(void) {
        return length - unk6;
    }
};

#endif

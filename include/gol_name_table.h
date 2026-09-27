#ifndef GOL_NAME_TABLE_H
#define GOL_NAME_TABLE_H

#include "common.h"

struct BandicootEntry {
    /* 0x00 */ char name[8];
    /* 0x08 */ void* value;
};

struct GolNameTable {
    /* 0x00 */ BandicootEntry* entries;
    /* 0x04 */ u32 capacity;
    /* 0x08 */ /* vptr */

    virtual ~GolNameTable();
    virtual void vfunc2(u32 size);
    virtual void vfunc3();
    virtual void vfunc4() = 0;

    GolNameTable();

    void addName(const char* name, void* value);
    static void func_80024C10(s32 val);
    void findByValue(void* value, char* out);
    void* findByName(const char* name);
    s32 hasEntries();
    u32 hash(const char* name);
};

#endif // GOL_NAME_TABLE_H

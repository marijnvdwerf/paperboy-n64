#include "common.h"
#include "bandicoot.h"

extern "C" int strncmp(const char*, const char*, unsigned);
extern "C" char* strncpy(char*, const char*, unsigned);
extern "C" char* strcat(char*, const char*);
extern "C" void* memset(void*, int, unsigned);
extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390();

extern s32 D_80070B30;

inline u32 Bandicoot::hash(const char* name) {
    u32 i;
    u32 shift;
    u32 value = 0;
    shift = 0;
    for (i = 0; i < 8 && name[i] != 0; i++) {
        value += name[i] << shift;
        shift = (shift + 7) & 31;
    }
    return value % capacity;
}

inline s32 Bandicoot::hasEntries() {
    return entries != 0;
}

inline Bandicoot::Bandicoot() {
    entries = NULL;
    capacity = 0;
}

inline Bandicoot::~Bandicoot() {
    vfunc3();
}

inline void Bandicoot::vfunc2(u32 size) {
    capacity = size;
    func_8004B3BC(D_80070B30);
    entries = new BandicootEntry[capacity];
    func_8004B390();

    if (entries == NULL) {
        __assert("", 0, 0, 0);
    }

    memset(entries, 0, capacity * sizeof(BandicootEntry));
}

void Bandicoot::addName(const char* name, void* value) {
    u32 slot = hash(name);
    u32 start = slot;

    char buf[40];
    while (1) {
        if (entries[slot].value == NULL) {
            break;
        }
        if (strncmp(entries[slot].name, name, 8) == 0) {
            strncpy(buf, name, 8);
            buf[8] = 0;
            strcat(buf, ": Duplicate name encountered");
            __assert("", 0, 0, 0);
        }
        slot++;
        if (slot >= capacity) {
            slot = 0;
        }
        if (slot == start) {
            __assert("", 0, 0, 0);
        }
    }

    strncpy(entries[slot].name, name, 8);
    entries[slot].value = value;
}

void Bandicoot::func_80024C10(s32 val) {
    D_80070B30 = val;
}

void Bandicoot::findByValue(void* value, char* out) {
    for (u32 i = 0; i < capacity; i++) {
        if (entries[i].value == value) {
            strncpy(out, entries[i].name, 8);
            return;
        }
    }
    *out = 0;
}

void* Bandicoot::findByName(const char* name) {
    u32 slot = hash(name);
    u32 start = slot;

    while (entries[slot].value != NULL) {
        if (strncmp(entries[slot].name, name, 8) == 0) {
            return entries[slot].value;
        }
        slot++;
        if (slot >= capacity) {
            slot = 0;
        }
        if (slot == start) {
            return NULL;
        }
    }

    return NULL;
}

void Bandicoot::vfunc3() {
    if (entries != NULL) {
        delete[] entries;
        entries = NULL;
    }
}

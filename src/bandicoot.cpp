#include "common.h"
#include "bandicoot.h"

extern "C" int strncmp(const char*, const char*, unsigned);
extern "C" char* strncpy(char*, const char*, unsigned);
extern "C" char* strcat(char*, const char*);
extern "C" void* memset(void*, int, unsigned);
extern "C" void func_8004B3BC(s32);
extern "C" void func_8004B390();

extern s32 D_80070B30;
extern const char D_80001E50[];

// TODO: replace with inline "" when all functions have been decompiled
INCLUDE_RODATA("asm/nonmatchings/bandicoot", D_80001E50);

#ifdef NON_MATCHING
void Bandicoot::addName(const char* name, void* value) {
    u32 slot = hashName(name, capacity);
    u32 start = slot;

    char buf[40];
    while (entries[slot].value != NULL) {
        if (strncmp(entries[slot].name, name, 8) == 0) {
            strncpy(buf, name, 8);
            buf[8] = 0;
            strcat(buf, ": Duplicate name encountered");
            __assert(D_80001E50, 0, 0, 0);
        }
        slot++;
        if (slot >= capacity) {
            slot = 0;
        }
        if (slot == start) {
            __assert(D_80001E50, 0, 0, 0);
        }
    }

    strncpy(entries[slot].name, name, 8);
    entries[slot].value = value;
}
#else
INCLUDE_ASM("asm/nonmatchings/bandicoot", addName__9BandicootPCcPv);
#endif

void Bandicoot::func_80024C10(s32 val) {
    D_80070B30 = val;
}

#ifdef NON_MATCHING
void Bandicoot::findByValue(void* value, char* out) {
    u32 offset = 0;
    u32 i = 0;
    if (capacity != 0) {
        do {
            BandicootEntry* entry = (BandicootEntry*)((u8*)entries + offset);
            i++;
            if (entry->value == value) {
                strncpy(out, entry->name, 8);
                return;
            }
            offset += sizeof(BandicootEntry);
        } while (i < capacity);
    }
    *out = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/bandicoot", findByValue__9BandicootPvPc);
#endif

#ifdef NON_MATCHING
void* Bandicoot::findByName(const char* name) {
    u32 slot = hashName(name, capacity);
    u32 start = slot;

    if (entries[slot].value == NULL) {
        return NULL;
    }

    do {
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
    } while (entries[slot].value != NULL);

    return NULL;
}
#else
INCLUDE_ASM("asm/nonmatchings/bandicoot", findByName__9BandicootPCc);
#endif

void Bandicoot::vfunc3() {
    if (entries != NULL) {
        delete[] entries;
        entries = NULL;
    }
}

void Bandicoot::vfunc2(u32 size) {
    capacity = size;
    func_8004B3BC(D_80070B30);
    entries = new BandicootEntry[capacity];
    func_8004B390();

    if (entries == NULL) {
        __assert(D_80001E50, 0, 0, 0);
    }

    memset(entries, 0, capacity * sizeof(BandicootEntry));
}

Bandicoot::~Bandicoot() {
    vfunc3();
}

Bandicoot::Bandicoot() {
    entries = NULL;
    capacity = 0;
}

s32 Bandicoot::hasEntries() {
    return entries != 0;
}

#ifdef NON_MATCHING
u32 Bandicoot::hash(u8* name) {
    u32 var_a2;
    u32 var_a3;
    u8* var_v1;
    u32 var_t0;
    u8 temp_v0;

    var_t0 = 0;
    var_a3 = 0;
    var_a2 = 0;
    var_v1 = name;
loop_1:
    temp_v0 = *var_v1;
    var_a2 += 1;
    if (temp_v0 != 0) {
        var_t0 += temp_v0 << var_a3;
        var_a3 = (var_a3 + 7) & 0x1F;
        var_v1 = name + var_a2;
        if (var_a2 < 8) {
            goto loop_1;
        }
    }
    return var_t0 % capacity;
}
#else
INCLUDE_ASM("asm/nonmatchings/bandicoot", hash__9BandicootPUc);
#endif

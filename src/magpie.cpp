#include "common.h"
#include "parrot.h"

extern "C" {
void qsort(void* base, void* nel, s32 width, void* compar);
s32 toupper(s32 c);
s32 strnicmp(char* s1, char* s2, s32 n);
unsigned int strlen(const char* s);
}

struct MagpieEntry {
    /* 0x0 */ s32 length;
    /* 0x4 */ s32 value;
    /* 0x8 */ char* name;
};

class Magpie : public Parrot {
  public:
    /* 0x650 */ u8 pad650[8];
    /* 0x658 */ s32 count;
    /* 0x65C */ u32* alphaIndex;
    /* 0x660 */ MagpieEntry* entries;
    /* 0x664 */ s32* readyFlag;
    /* 0x668 */ char* lastMatch;

    Magpie();
};

extern "C" s32 func_8004A00C(MagpieEntry* a, MagpieEntry* b);

extern "C" void func_80049DC0(Magpie* self) {
    s32 i;
    s32 offset;

    self->lastMatch = NULL;

    if (*self->readyFlag != 0) {
        return;
    }

    i = 0;
    offset = 0;
    qsort(self->entries, (void*)self->count, 0xC, (void*)func_8004A00C);

    while (i < self->count) {
        MagpieEntry* e = (MagpieEntry*)((u8*)self->entries + offset);
        e->length = strlen(e->name);
        offset += 0xC;
        i++;
    }

    for (i = 0; i < 0x1A; i++) {
        self->alphaIndex[i] = 0xFFFF;
    }

    self->alphaIndex[0x1A] = self->count;

    for (i = 0; i < self->count; i++) {
        s32 c = toupper(self->entries[i].name[0]);
        s32 idx = c - 'A';
        if (self->alphaIndex[idx] == 0xFFFF) {
            self->alphaIndex[idx] = i;
        }
    }

    for (i = 0x19; i >= 0; i--) {
        if (self->alphaIndex[i] == 0xFFFF) {
            self->alphaIndex[i] = self->alphaIndex[i + 1];
        }
    }

    *self->readyFlag = 1;
}

// entries load ordering and loop structure (top-tested j vs bottom-tested bnez)
#ifdef NON_MATCHING
extern "C" s32 func_80049F18(Magpie* self, char* name, s32 len) {
    s32 c = (u8)name[0] - 'A';
    u32 start = self->alphaIndex[c];
    u32 end = self->alphaIndex[c + 1];
    s32* p = (s32*)((u8*)self->entries + start * 12);
    s32* vp = p + 1;

    while (start < end) {
        if (len == p[0]) {
            if (strnicmp(name, (char*)vp[1], len) == 0) {
                self->lastMatch = (char*)vp[1];
                return vp[0];
            }
        }
        vp += 3;
        p += 3;
        start++;
    }

    self->parseError(0xD);
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/magpie", func_80049F18);
#endif

extern "C" s32 func_8004A00C(MagpieEntry* a, MagpieEntry* b) {
    u32 ca = toupper(a->name[0]);
    u32 cb = toupper(b->name[0]);
    ca &= 0xFF;
    cb &= 0xFF;
    if (ca < cb) {
        return -1;
    }
    return (ca ^ cb) != 0;
}

Magpie::Magpie() {
    alphaIndex = NULL;
    entries = NULL;
    count = 0;
}

extern "C" char* func_8004A0A4(Magpie* self) {
    return self->lastMatch;
}

extern "C" s32 func_8004A0B0(Magpie* self) {
    return *self->readyFlag;
}

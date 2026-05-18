#include "common.h"
#include "parrot.h"

extern "C" {
void qsort(void* base, void* nel, s32 width, void* compar);
s32 toupper(s32 c);
s32 strnicmp(char* s1, char* s2, s32 n);
unsigned int strlen(const char* s);
}

class OtherBird : public Parrot {
    public:
    virtual void selectDriver(const char* path);
    virtual char* getExtension();
    virtual void parseError(s32 code);
    virtual s32 nextToken();
};

struct MagpieEntry {
    /* 0x0 */ s32 length;
    /* 0x4 */ s32 value;
    /* 0x8 */ char* name;
};

class Magpie : public OtherBird {
  public:
    /* 0x650 */ u8 pad650[8];
    /* 0x658 */ s32 count;
    /* 0x65C */ u32* alphaIndex;
    /* 0x660 */ MagpieEntry* entries;
    /* 0x664 */ s32* readyFlag;
    /* 0x668 */ char* lastMatch;

    Magpie();
    virtual s32 vfunc21(char* name, s32 len);

    void prepare();
    char* getLastMatch();
    s32 isReady();
};

static s32 compareByFirstLetter(MagpieEntry* a, MagpieEntry* b);

void Magpie::prepare() {
    s32 i;
    s32 entryOffset;

    this->lastMatch = NULL;

    if (*this->readyFlag != 0) {
        return;
    }

    i = 0;
    entryOffset = 0;
    qsort(this->entries, (void*)this->count, sizeof(MagpieEntry), (void*)compareByFirstLetter);

    for (; i < this->count; i++) {
        MagpieEntry* e = (MagpieEntry*)(entryOffset + (s32)this->entries);
        e->length = strlen(e->name);
        entryOffset += sizeof(MagpieEntry);
    }

    for (i = 0; i < 0x1A; i++) {
        this->alphaIndex[i] = 0xFFFF;
    }

    this->alphaIndex[0x1A] = this->count;

    for (i = 0; i < this->count; i++) {
        s32 c = toupper(this->entries[i].name[0]);
        s32 idx = c - 'A';
        if (this->alphaIndex[idx] == 0xFFFF) {
            this->alphaIndex[idx] = i;
        }
    }

    for (i = 0x19; i >= 0; i--) {
        if (this->alphaIndex[i] == 0xFFFF) {
            this->alphaIndex[i] = this->alphaIndex[i + 1];
        }
    }

    *this->readyFlag = 1;
}

s32 Magpie::vfunc21(char* name, s32 len) {
    s32 c = (u8)name[0] - 'A';
    u32 start = this->alphaIndex[c];
    u32 end = this->alphaIndex[c + 1];
    MagpieEntry* p = &this->entries[start];

    while (1) {
        if (start >= end) {
            break;
        }
        if (len == p->length) {
            if (strnicmp(name, p->name, len) == 0) {
                this->lastMatch = p->name;
                return p->value;
            }
        }
        p++;
        start++;
    }

    this->parseError(0xD);
    return 1;
}

static s32 compareByFirstLetter(MagpieEntry* a, MagpieEntry* b) {
    u8 ca = toupper(a->name[0]);
    u8 cb = toupper(b->name[0]);
    if (ca < cb) {
        return -1;
    }
    return ca != cb;
}

Magpie::Magpie() {
    alphaIndex = NULL;
    entries = NULL;
    count = 0;
}

char* Magpie::getLastMatch() {
    return this->lastMatch;
}

s32 Magpie::isReady() {
    return *this->readyFlag;
}

#include "streams.h"

extern "C" {
extern s32 func_8004767C(void*, void*);
extern s32 func_80047A64(void*, s32);
extern s32 func_80047A50(void*, s32);
extern s32 func_8004798C(void*, void*, s32);
extern s32 func_800477D4(void*, char*, u16, s32, char*, char*, void*);
extern void func_80049B50(Otter*);
extern void func_80049B58(Otter*, s32);
extern void func_80049980(const char*, char*, char*);
extern void func_8000752C(const char*, char*, s32);
extern s32 func_80047490(void*, char*, s32*, s32*);
extern s32 func_80047428(void*, char*, s32, void*);
extern s32 func_800473C4(void*, char*, s32*);
extern s32 func_8004775C(void*, char*, u16, s32, char*, char*);
extern void func_800073E0(const void* src, void* dst, s32 n);
extern unsigned strlen(const char*);
extern s32 stricmp(const char*, const char*);
}

struct DirEntry {
    /* 0x0 */ s32 size;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ u16 unk8;
    /* 0xA */ char ext[4];
    /* 0xE */ char name[0x10];
};

extern "C" s32 func_80049400(Otter* self, const char* name) {
    FileInfo info;
    DirEntry entry;
    s32 found = 0;
    u32 i = 0;
    FileInfo* p = &info;

    do {
        self->vfunc9(i, p);
        if (stricmp(name, p->name) == 0) {
            found = 1;
            break;
        }
        i++;
    } while (i < 0x10);

    if (!found) {
        return 8;
    }

    func_80047428(self->stream->unk4, self->buf, i, &entry);
    Stream* s = self->stream;
    void* unk4 = s->unk4;
    char* buf = self->buf;
    s32 status = func_8004775C(unk4, buf, entry.unk8, entry.unk4, entry.name, entry.ext);
    if (status == 5) {
        return 8;
    }
    switch (status) {
        case 0:
            self->state = 0;
            break;
        case 1:
            self->state = 0x12;
            break;
        case 2:
            self->flag = 1;
            self->state = 0x13;
            break;
        case 4:
            self->state = 0x14;
            break;
        case 10:
            self->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            self->state = 1;
            break;
    }
    return self->state;
}

#ifdef NON_MATCHING
extern "C" s32 func_8004954C(Otter* self, FileInfo* arg1) {
    char extBuf[4];
    char nameBuf[0x10];

    memset(nameBuf, 0, 0x10);
    memset(extBuf, 0, 4);

    s32 dotPos = 0;
    s32 len = 0;
    if (arg1->name[0]) {
        const char* p = arg1->name;
        do {
            if (*p == '.') {
                dotPos = len;
            }
            p++;
            len++;
        } while (*p);
    }
    if (dotPos != 0) {
        len = dotPos;
    }
    func_8000752C(arg1->name, nameBuf, len);
    if (arg1->name[len] == '.') {
        len++;
        s32 extLen = 0;
        if (arg1->name[len]) {
            do {
                extLen++;
            } while (arg1->name[len + extLen]);
        }
        func_8000752C(arg1->name + len, extBuf, extLen);
    }

    s32 status = func_8004775C(self->stream->unk4, self->buf, (u16)arg1->unk24, arg1->unk28, nameBuf, extBuf);
    if (status == 5) {
        return 8;
    }
    switch (status) {
        case 0:
            self->state = 0;
            break;
        case 1:
            self->state = 0x12;
            break;
        case 2:
            self->flag = 1;
            self->state = 0x13;
            break;
        case 4:
            self->state = 0x14;
            break;
        case 10:
            self->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            self->state = 1;
            break;
    }
    return self->state;
}
#else
INCLUDE_ASM("asm/nonmatchings/otter", func_8004954C);
#endif

extern "C" s32 func_800496DC(Otter* self, s32* out) {
    s32 outVal;
    s32 status = func_800473C4(self->stream->unk4, self->buf, &outVal);
    if (status == 0) {
        *out = outVal;
    }
    switch (status) {
        case 0:
            self->state = 0;
            break;
        case 1:
            self->state = 0x12;
            break;
        case 2:
            self->flag = 1;
            self->state = 0x13;
            break;
        case 4:
            self->state = 0x14;
            break;
        case 10:
            self->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            self->state = 1;
            break;
    }
    return self->state;
}

extern "C" s32 func_80049790(Otter* self, s32 arg1, FileInfo* info) {
    DirEntry entry;
    s32 status = func_80047428(self->stream->unk4, self->buf, arg1, &entry);
    if (status == 0) {
        info->fileSize = entry.size;
        info->unk24 = entry.unk8;
        info->unk28 = entry.unk4;
        memset(info, 0, 0x20);
        func_800073E0(entry.name, info->name, 0x10);
        s32 sz = strlen(info->name);
        if (entry.ext[0]) {
            info->name[sz++] = '.';
            func_800073E0(entry.ext, &info->name[sz], 4);
        }
    }
    switch (status) {
        case 0:
            self->state = 0;
            break;
        case 1:
            self->state = 0x12;
            break;
        case 2:
            self->flag = 1;
            self->state = 0x13;
            break;
        case 4:
            self->state = 0x14;
            break;
        case 10:
            self->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            self->state = 1;
            break;
    }
    return self->state;
}

extern "C" s32 func_800498B8(Otter* self, s32* out1, s32* out2) {
    s32 outSecond;
    s32 outFirst;
    s32 status = func_80047490(self->stream->unk4, self->buf, &outFirst, &outSecond);
    *out1 = outSecond;
    if (out2) {
        *out2 = outFirst;
    }
    switch (status) {
        case 0:
            self->state = 0;
            break;
        case 1:
            self->state = 0x12;
            break;
        case 2:
            self->flag = 1;
            self->state = 0x13;
            break;
        case 4:
            self->state = 0x14;
            break;
        case 10:
            self->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            self->state = 1;
            break;
    }
    return self->state;
}

#ifdef NON_MATCHING
// Score ~60: `move v1,s1` and `li a1,0x2e` swap in scheduling.
extern "C" void func_80049980(const char* path, char* name, char* ext) {
    memset(name, 0, 0x10);
    memset(ext, 0, 4);

    s32 dotPos = 0;
    s32 len = 0;
    if (*path) {
        const char* p = path;
        do {
            if (*p == '.') {
                dotPos = len;
            }
            p++;
            len++;
        } while (*p);
    }

    if (dotPos != 0) {
        len = dotPos;
    }

    func_8000752C(path, name, len);

    if (path[len] == '.') {
        len++;
        s32 extLen = 0;
        if (path[len]) {
            do {
                extLen++;
            } while (path[len + extLen]);
        }
        func_8000752C(path + len, ext, extLen);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/otter", func_80049980);
#endif

#ifdef NON_MATCHING
// Score ~60: lhu/lw scheduling for s->unkA / s->unkC swapped vs target;
// no source-level lever found within budget.
extern "C" s32 func_80049A74(Otter* self, const char* path) {
    char ext[4];
    char name[0x10];
    char header[8];
    func_80049980(path, name, ext);
    Stream* s = self->stream;
    char* buf = self->buf;
    void* unk4 = s->unk4;
    u16 unkA = s->unkA;
    s32 unkC = s->unkC;
    s32 status = func_800477D4(unk4, buf, unkA, unkC, name, ext, header);
    if (status == 5) {
        return 8;
    }
    switch (status) {
        case 0:
            self->state = 0;
            break;
        case 1:
            self->state = 0x12;
            break;
        case 2:
            self->flag = 1;
            self->state = 0x13;
            break;
        case 4:
            self->state = 0x14;
            break;
        case 10:
            self->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            self->state = 1;
            break;
    }
    return self->state;
}
#else
INCLUDE_ASM("asm/nonmatchings/otter", func_80049A74);
#endif

extern "C" void func_80049B50(Otter* self) {
    self->stream = NULL;
}

extern "C" void func_80049B58(Otter* self, s32 mode) {
    switch (mode) {
        case 0:
            self->state = 0;
            break;
        case 1:
            self->state = 0x12;
            break;
        case 2:
            self->flag = 1;
            self->state = 0x13;
            break;
        case 4:
            self->state = 0x14;
            break;
        case 10:
            self->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            self->state = 1;
            break;
    }
}

extern "C" void func_80049BC8(Otter* self) {
    s32 status = func_8004767C(self->stream->unk4, self->buf);
    func_80049B58(self, status);
    if (self->state == 0) {
        self->vfunc2();
    }
}

extern "C" s32 func_80049C24(Otter* self) {
    s32 success = 0;
    void* x = self->stream->unk4;
    void* y = x;
    if (func_80047A64(x, self->unk10) != 0) {
        success = func_80047A50(x, self->unk10) != 0;
    }
    if (success) {
        func_80049B58(self, func_8004798C(y, self->buf, self->unk10));
    } else {
        self->state = 0x12;
    }
    return self->state;
}

extern "C" void func_80049CC4(Otter* self, Stream* stream, s32 mode, s32 hasInit) {
    self->stream = stream;
    self->unk10 = mode;
    if (hasInit) {
        self->vfunc2();
    } else {
        self->state = 0x12;
    }
}

Otter::~Otter() {
    func_80049B50(this);
}

Otter::Otter() {
    stream = NULL;
    unk10 = 0;
    memset(buf, 0, 0x68);
}

extern "C" Stream* func_80049DA0(Otter* self) {
    return self->stream;
}

extern "C" char* func_80049DAC(Otter* self) {
    return self->buf;
}

extern "C" s32 func_80049DB4(Otter* self) {
    return self->stream != NULL;
}

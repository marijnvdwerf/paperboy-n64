#include "otter.h"

extern "C" {
extern s32 func_8004767C(void*, void*);
extern s32 func_80047A64(void*, s32);
extern s32 func_80047A50(void*, s32);
extern s32 func_8004798C(void*, void*, s32);
extern s32 func_800477D4(void*, char*, u16, s32, char*, char*, void*);
extern s32 func_80047490(void*, char*, s32*, s32*);
extern s32 func_80047428(void*, char*, s32, void*);
extern s32 func_800473C4(void*, char*, s32*);
extern s32 func_8004775C(void*, char*, u16, s32, char*, char*);
extern void func_800073E0(const void* src, void* dst, s32 n);
extern void func_8000752C(const char*, char*, s32);
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

s32 Otter::vfunc5(const char* name) {
    u8 pad[24];
    FileInfo info;
    DirEntry entry;
    s32 found = 0;
    u32 i = 0;

    for (i = 0; i < 0x10; i++) {
        this->vfunc9(i, &info);
        if (stricmp(name, info.name) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        return 8;
    }

    func_80047428(this->unkC->unk4, this->buf, i, &entry);
    OtterStone* s = this->unkC;
    void* unk4 = s->unk4;
    char* buf = this->buf;
    s32 status = func_8004775C(unk4, buf, entry.unk8, entry.unk4, entry.name, entry.ext);
    if (status == 5) {
        return 8;
    }
    switch (status) {
        case 0:
            this->state = 0;
            break;
        case 1:
            this->state = 0x12;
            break;
        case 2:
            this->flag = 1;
            this->state = 0x13;
            break;
        case 4:
            this->state = 0x14;
            break;
        case 10:
            this->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            this->state = 1;
            break;
    }
    return this->state;
}

s32 Otter::vfunc6(FileInfo* arg1) {
    char extBuf[4];
    char nameBuf[0x10];
    char* extPtr = extBuf;

    memset(nameBuf, 0, 0x10);
    memset(extPtr, 0, 4);

    s32 dotPos = 0;
    s32 len = 0;
    if (arg1->name[0]) {
        s32 dot = '.';
        const char* p = arg1->name;
        do {
            if (*p == dot) {
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
        func_8000752C(arg1->name + len, extPtr, extLen);
    }

    OtterStone* s = this->unkC;
    void* unk4 = s->unk4;
    char* buf = this->buf;
    s32 status = func_8004775C(unk4, buf, (u16)arg1->unk24, arg1->unk28, nameBuf, extBuf);
    if (status == 5) {
        return 8;
    }
    switch (status) {
        case 0:
            this->state = 0;
            break;
        case 1:
            this->state = 0x12;
            break;
        case 2:
            this->flag = 1;
            this->state = 0x13;
            break;
        case 4:
            this->state = 0x14;
            break;
        case 10:
            this->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            this->state = 1;
            break;
    }
    return this->state;
}

inline void Otter::func_80049CC4(OtterStone* arg0, s32 mode, s32 hasInit) {
    this->unkC = arg0;
    this->unk10 = mode;
    if (hasInit) {
        this->vfunc2();
    } else {
        this->state = 0x12;
    }
}

inline s32 Otter::vfunc2() {
    s32 success = 0;
    void* x = this->unkC->unk4;
    void* y = x;
    if (func_80047A64(x, this->unk10) != 0) {
        success = func_80047A50(x, this->unk10) != 0;
    }
    if (success) {
        this->func_80049B58(func_8004798C(y, this->buf, this->unk10));
    } else {
        this->state = 0x12;
    }
    return this->state;
}

inline void Otter::vfunc3() {
    s32 status = func_8004767C(this->unkC->unk4, this->buf);
    this->func_80049B58(status);
    if (this->state == 0) {
        this->vfunc2();
    }
}

inline void Otter::func_80049B58(s32 mode) {
    switch (mode) {
        case 0:
            this->state = 0;
            break;
        case 1:
            this->state = 0x12;
            break;
        case 2:
            this->flag = 1;
            this->state = 0x13;
            break;
        case 4:
            this->state = 0x14;
            break;
        case 10:
            this->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            this->state = 1;
            break;
    }
}

inline void Otter::func_80049B50() {
    this->unkC = NULL;
}

inline s32 Otter::vfunc4(const char* path) {
    char ext[4];
    char name[0x10];
    char header[8];
    func_80049980(path, name, ext);
    OtterStone* s = this->unkC;
    char* buf = this->buf;
    void* unk4 = s->unk4;
    u16 unkA = s->unkA;
    u32 unkC;
    s32 status = func_800477D4(unk4, buf, unkA, (unkC = s->unkC), name, ext, header);
    if (status == 5) {
        return 8;
    }
    switch (status) {
        case 0:
            this->state = 0;
            break;
        case 1:
            this->state = 0x12;
            break;
        case 2:
            this->flag = 1;
            this->state = 0x13;
            break;
        case 4:
            this->state = 0x14;
            break;
        case 10:
            this->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            this->state = 1;
            break;
    }
    return this->state;
}

inline void Otter::func_80049980(const char* path, char* name, char* ext) {
    memset(name, 0, 0x10);
    memset(ext, 0, 4);

    s32 dotPos = 0;
    s32 len = 0;
    if (*path) {
        s32 dot = '.';
        const char* p = path;
        do {
            if (*p == dot) {
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

inline s32 Otter::vfunc7(s32* out1, s32* out2) {
    s32 outSecond;
    s32 outFirst;
    s32 status = func_80047490(this->unkC->unk4, this->buf, &outFirst, &outSecond);
    *out1 = outSecond;
    if (out2) {
        *out2 = outFirst;
    }
    switch (status) {
        case 0:
            this->state = 0;
            break;
        case 1:
            this->state = 0x12;
            break;
        case 2:
            this->flag = 1;
            this->state = 0x13;
            break;
        case 4:
            this->state = 0x14;
            break;
        case 10:
            this->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            this->state = 1;
            break;
    }
    return this->state;
}

inline s32 Otter::vfunc9(s32 arg1, FileInfo* info) {
    DirEntry entry;
    s32 status = func_80047428(this->unkC->unk4, this->buf, arg1, &entry);
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
            this->state = 0;
            break;
        case 1:
            this->state = 0x12;
            break;
        case 2:
            this->flag = 1;
            this->state = 0x13;
            break;
        case 4:
            this->state = 0x14;
            break;
        case 10:
            this->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            this->state = 1;
            break;
    }
    return this->state;
}

inline s32 Otter::vfunc8(s32* out) {
    s32 outVal;
    s32 status = func_800473C4(this->unkC->unk4, this->buf, &outVal);
    if (status == 0) {
        *out = outVal;
    }
    switch (status) {
        case 0:
            this->state = 0;
            break;
        case 1:
            this->state = 0x12;
            break;
        case 2:
            this->flag = 1;
            this->state = 0x13;
            break;
        case 4:
            this->state = 0x14;
            break;
        case 10:
            this->state = 0x15;
            break;
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            this->state = 1;
            break;
    }
    return this->state;
}

#include "otter.h"

extern "C" {
extern void func_800073E0(const void* src, void* dst, s32 n);
extern void func_8000752C(const char*, char*, s32);
extern unsigned strlen(const char*);
extern s32 stricmp(const char*, const char*);
}

s32 Otter::vfunc5(const char* name) {
    u8 pad[24];
    FileInfo info;
    OSPfsState entry;
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

    this->unkC->unk4->pfsFileState(&this->pfs, i, &entry);
    OtterStone* s = this->unkC;
    N64ControllerSystem* unk4 = s->unk4;
    OSPfs* pfs = &this->pfs;
    s32 status = unk4->pfsDeleteFile(pfs, entry.company_code, entry.game_code, (u8*)entry.game_name, (u8*)entry.ext_name);
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
    N64ControllerSystem* unk4 = s->unk4;
    OSPfs* pfs = &this->pfs;
    s32 status = unk4->pfsDeleteFile(pfs, (u16)arg1->companyCode, arg1->gameCode, (u8*)nameBuf, (u8*)extBuf);
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
    this->port = mode;
    if (hasInit) {
        this->vfunc2();
    } else {
        this->state = 0x12;
    }
}

inline s32 Otter::vfunc2() {
    s32 success = 0;
    N64ControllerSystem* x = this->unkC->unk4;
    N64ControllerSystem* y = x;
    if (x->isJoyport(this->port) != 0) {
        success = x->isAbsolute(this->port) != 0;
    }
    if (success) {
        this->func_80049B58(y->pfsInitPak(&this->pfs, this->port));
    } else {
        this->state = 0x12;
    }
    return this->state;
}

inline void Otter::vfunc3() {
    s32 status = this->unkC->unk4->pfsRepairId(&this->pfs);
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
    s32 header[2];
    func_80049980(path, name, ext);
    OtterStone* s = this->unkC;
    OSPfs* pfs = &this->pfs;
    N64ControllerSystem* unk4 = s->unk4;
    u16 unkA = s->companyCode;
    u32 unkC;
    s32 status = unk4->pfsFindFile(pfs, unkA, (unkC = s->gameCode), (u8*)name, (u8*)ext, header);
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
    s32 status = this->unkC->unk4->pfsNumFiles(&this->pfs, &outFirst, &outSecond);
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
    OSPfsState entry;
    s32 status = this->unkC->unk4->pfsFileState(&this->pfs, arg1, &entry);
    if (status == 0) {
        info->fileSize = entry.file_size;
        info->companyCode = entry.company_code;
        info->gameCode = entry.game_code;
        memset(info, 0, 0x20);
        func_800073E0(entry.game_name, info->name, 0x10);
        s32 sz = strlen(info->name);
        if (entry.ext_name[0]) {
            info->name[sz++] = '.';
            func_800073E0(entry.ext_name, &info->name[sz], 4);
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
    s32 status = this->unkC->unk4->pfsFreeBlocks(&this->pfs, &outVal);
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

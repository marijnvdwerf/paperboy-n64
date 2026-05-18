#include "penguin.h"

// vtable slot 17: Pelican::vfunc17 override
s32 Penguin::vfunc17(Otter* otter, const char* path, s32 length) {
    char ext[4];
    char name[16];
    s32 r;

    Otter::func_80049980(path, name, ext);
    this->unk30 = otter;

    if (otter->state != 0) {
        s32 v = otter->vfunc2();
        if (v != 0)
            return v;
    }

    Otter* o = this->unk30;
    OtterStone* stone = o->unkC;
    OtterStone* findStone = stone;
    N64ControllerSystem& ncs = *stone->unk4;

    if (this->openFlags & 1) {
        u16 companyCode;
        s32 gameCode;
        r = ncs.pfsAllocateFile(&o->pfs, (companyCode = stone->companyCode), (gameCode = stone->gameCode), (u8*)name, (u8*)ext, length, &this->fileNo);
        if (r == 7)
            return 0x17;
        if (r == 8)
            return 0x16;
        if (r == 9)
            return 0xA;
    } else {
        u16 companyCode;
        s32 gameCode;
        r = ncs.pfsFindFile(&o->pfs, (companyCode = findStone->companyCode), (gameCode = findStone->gameCode), (u8*)name, (u8*)ext, &this->fileNo);
        if (r == 5)
            return 8;
    }

    this->unk30->func_80049B58(r);
    return this->unk30->state;
}

// vtable slot 3: seek override
s32 Penguin::seek(u32 pos) {
    this->fileOffset = pos;
    return 0;
}

// vtable slot 6: sync override
s32 Penguin::sync() {
    return 0;
}

// vtable slot 5: rawWrite override
s32 Penguin::rawWrite(void* buf, s32 len) {
    Otter* o = this->unk30;
    N64ControllerSystem* ncs = o->unkC->unk4;
    s32 r = ncs->pfsReadWriteFile(&o->pfs, this->fileNo, 1, this->fileOffset, len, (u8*)buf);
    if (r == 6)
        return 0xE;
    this->unk30->func_80049B58(r);
    return this->unk30->state;
}

// vtable slot 4: rawRead override
s32 Penguin::rawRead(void* buf, s32 len, s32* out) {
    N64ControllerSystem* ncs = this->unk30->unkC->unk4;
    *out = 0;
    s32 r = ncs->pfsReadWriteFile(&this->unk30->pfs, this->fileNo, 0, this->fileOffset, len, (u8*)buf);
    if (r == 6)
        return 0xD;
    if (r == 0)
        *out = len;
    this->unk30->func_80049B58(r);
    return this->unk30->state;
}

// vtable slot 2: rawClose override
s32 Penguin::rawClose() {
    this->fileNo = -1;
    this->unk30 = NULL;
    return 0;
}

Penguin::Penguin() {
    fileNo = -1;
    unk30 = NULL;
}

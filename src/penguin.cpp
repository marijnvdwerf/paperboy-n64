#include "common.h"
#include "file.h"
#include "otter.h"
#include "ultratypes.h"

class Pelican : public RomFile {
  public:
    virtual ~Pelican();
};

class Penguin : public Pelican {
  public:
    /* 0x30 */ Otter* otter;
    /* 0x34 */ s32 fileNo;

    Penguin();
    virtual s32 rawClose();
    virtual s32 seek(s32 pos);
};

// vtable slot 14: rawOpen override
#ifdef NON_MATCHING
extern "C" s32 func_80048E80(Penguin* this_, Otter* otter, const char* path, s32 length) {
    char ext[4];
    char name[16];
    s32 r;

    Otter::func_80049980(path, name, ext);
    this_->otter = otter;

    if (otter->state != 0) {
        s32 v = otter->vfunc2();
        if (v != 0) return v;
    }

    Otter* o = this_->otter;
    OtterStone* stone = o->unkC;
    N64ControllerSystem* ncs = stone->unk4;

    if (this_->openFlags & 1) {
        r = ncs->pfsAllocateFile(&o->pfs, stone->companyCode, stone->gameCode,
                                 (u8*)name, (u8*)ext, length, &this_->fileNo);
        if (r == 7) return 0x17;
        if (r == 8) return 0x16;
        if (r == 9) return 0xA;
    } else {
        r = ncs->pfsFindFile(&o->pfs, stone->companyCode, stone->gameCode,
                             (u8*)name, (u8*)ext, &this_->fileNo);
        if (r == 5) return 8;
    }

    this_->otter->func_80049B58(r);
    return this_->otter->state;
}
#else
INCLUDE_ASM("asm/nonmatchings/penguin", func_80048E80);
#endif

// vtable slot 3: seek override
s32 Penguin::seek(s32 pos) {
    this->fileOffset = pos;
    return 0;
}

// vtable slot 6: sync override
extern "C" s32 func_80048FD0(Penguin* this_) {
    return 0;
}

// vtable slot 5: rawWrite override
extern "C" s32 func_80048FD8(Penguin* this_, void* buf, s32 len) {
    Otter* o = this_->otter;
    N64ControllerSystem* ncs = o->unkC->unk4;
    s32 r = ncs->pfsReadWriteFile(
        &o->pfs, this_->fileNo, 1, this_->fileOffset, len, (u8*)buf);
    if (r == 6) return 0xE;
    this_->otter->func_80049B58(r);
    return this_->otter->state;
}

// vtable slot 4: rawRead override
extern "C" s32 func_80049048(Penguin* this_, void* buf, s32 len, s32* out) {
    N64ControllerSystem* ncs = this_->otter->unkC->unk4;
    *out = 0;
    s32 r = ncs->pfsReadWriteFile(
        &this_->otter->pfs, this_->fileNo, 0, this_->fileOffset, len, (u8*)buf);
    if (r == 6) return 0xD;
    if (r == 0) *out = len;
    this_->otter->func_80049B58(r);
    return this_->otter->state;
}

// vtable slot 2: rawClose override
s32 Penguin::rawClose() {
    this->fileNo = -1;
    this->otter = NULL;
    return 0;
}

Penguin::Penguin() {
    fileNo = -1;
    otter = NULL;
}


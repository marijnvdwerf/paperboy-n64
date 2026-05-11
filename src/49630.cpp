#include "common.h"
#include "structs.h"

#ifdef PAL
TEXT_PAD(0x10);
#endif

extern "C" {
extern s32 D_800768B0;
extern s32 D_80076504;
extern OSIoMesg D_800768B4;
extern OSMesgQueue D_800768D0;
extern OSMesg D_800768CC;
s32 func_8004B390();
s32 func_8004B3BC(s32);
}

s32 LocalIOBase::func_80048A30() {
    D_800768B0 = 1;
    return 1;
}

s32 LocalIOBase::func_80048A40(s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (this->unk4 & 1) {
        this->virt9();
    }
    if (arg4 == 0) {
        return 3;
    }
    if (!(arg3 & 0x20) || this->unk20 == NULL) {
        if (this->unk20 != NULL) {
            delete[] this->unk20;
            this->unk20 = NULL;
        }
        arg4 += arg4 & 1;
        func_8004B3BC(D_80076504);
        this->unk20 = new u8[arg4];
        func_8004B390();
        if (this->unk20 == NULL) {
            return 4;
        }
        this->unk14 = arg4;
    }
    this->unk0 = arg3 | 0x40;
    this->unk18 = 0;
    this->unk1C = 0;
    this->unk8 = 0;
    this->unk2C = arg1;
    this->unk10 = arg2;
    this->unk4 = 1;
    return 0;
}

s32 LocalIOBase::virt16() {
    osRecvMesg(&D_800768D0, NULL, OS_MESG_BLOCK);
    return 0;
}

s32 LocalIOBase::virt15(void* buf, s32 size, s32* outActual) {
    s32 nbytes = size;
    s32 remaining = this->unk10 - this->unk8;
    if (remaining <= 0) {
        *outActual = 0;
        return 0x10;
    }
    s32 align = this->unk8 & 1;
    this->unk8 -= align;
    remaining += align;
    if (remaining < size) {
        nbytes = remaining;
        *outActual = nbytes;
        nbytes += nbytes & 1;
    } else {
        *outActual = nbytes;
    }
    osInvalDCache(buf, nbytes);
    osPiStartDma(&D_800768B4, 0, 0, this->unk2C + this->unk8, buf, nbytes, &D_800768D0);
    return 0;
}

s32 LocalIOBase::virt4(void* buf, s32 size, s32* outActual) {
    s32 nbytes = size;
    s32 remaining = this->unk10 - this->unk8;
    if (remaining <= 0) {
        *outActual = 0;
        return 0x10;
    }
    s32 align = this->unk8 & 1;
    this->unk8 -= align;
    remaining += align;
    if (remaining < size) {
        nbytes = remaining;
        *outActual = nbytes;
        nbytes += nbytes & 1;
    } else {
        *outActual = nbytes;
    }
    if (D_800768B0) {
        for (;;) {}
    }
    osInvalDCache(buf, nbytes);
    osPiStartDma(&D_800768B4, 0, 0, this->unk2C + this->unk8, buf, nbytes, &D_800768D0);
    osRecvMesg(&D_800768D0, NULL, OS_MESG_BLOCK);
    return 0;
}

s32 LocalIOBase::virt3(u32 arg1) {
    if (arg1 >= (u32)this->unk10) {
        return 0xF;
    }
    this->unk8 = arg1;
    return 0;
}

s32 LocalIOBase::virt2() {
    return 8;
}

s32 LocalIOBase::virt1() {
    return 8;
}

s32 LocalIOBase::func_80048D58() {
    return 8;
}

extern "C" u8* D_800763F8;

extern "C" s32 func_80048D60(LocalIOBase* self) {
    if (self->unk4 & 2) {
        u32 offset = self->unk24 * 0x34;
        // TODO: fake match — int+int casts to force reg-allocator order on the addu
        LocalIOBase* parent = *(LocalIOBase**)((u32)offset + (u32)D_800763F8);
        return self->unk8 + func_80048D60(parent);
    }
    return self->unk2C;
}

extern "C" void func_80048DD4() {
    osCreateMesgQueue(&D_800768D0, &D_800768CC, 1);
}

LocalIOBase::~LocalIOBase() {
    this->virt9();
}

LocalIOBase::LocalIOBase() {
    this->unk2C = 0;
}

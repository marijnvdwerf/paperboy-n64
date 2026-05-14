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

s32 RomFile::func_80048A30() {
    D_800768B0 = 1;
    return 1;
}

s32 RomFile::func_80048A40(s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (this->state & 1) {
        this->close();
    }
    if (arg4 == 0) {
        return 3;
    }
    if (!(arg3 & 0x20) || this->buffer == NULL) {
        if (this->buffer != NULL) {
            delete[] this->buffer;
            this->buffer = NULL;
        }
        arg4 += arg4 & 1;
        func_8004B3BC(D_80076504);
        this->buffer = new u8[arg4];
        func_8004B390();
        if (this->buffer == NULL) {
            return 4;
        }
        this->bufferCapacity = arg4;
    }
    this->openFlags = arg3 | 0x40;
    this->bufferStart = 0;
    this->bufferEnd = 0;
    this->fileOffset = 0;
    this->unk2C = arg1;
    this->fileSize = arg2;
    this->state = 1;
    return 0;
}

s32 RomFile::rawReadWait() {
    osRecvMesg(&D_800768D0, NULL, OS_MESG_BLOCK);
    return 0;
}

s32 RomFile::rawReadAsync(void* buf, s32 size, s32* outActual) {
    s32 nbytes = size;
    s32 remaining = this->fileSize - this->fileOffset;
    if (remaining <= 0) {
        *outActual = 0;
        return 0x10;
    }
    s32 align = this->fileOffset & 1;
    this->fileOffset -= align;
    remaining += align;
    if (remaining < size) {
        nbytes = remaining;
        *outActual = nbytes;
        nbytes += nbytes & 1;
    } else {
        *outActual = nbytes;
    }
    osInvalDCache(buf, nbytes);
    osPiStartDma(&D_800768B4, 0, 0, this->unk2C + this->fileOffset, buf, nbytes, &D_800768D0);
    return 0;
}

s32 RomFile::rawRead(void* buf, s32 size, s32* outActual) {
    s32 nbytes = size;
    s32 remaining = this->fileSize - this->fileOffset;
    if (remaining <= 0) {
        *outActual = 0;
        return 0x10;
    }
    s32 align = this->fileOffset & 1;
    this->fileOffset -= align;
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
    osPiStartDma(&D_800768B4, 0, 0, this->unk2C + this->fileOffset, buf, nbytes, &D_800768D0);
    osRecvMesg(&D_800768D0, NULL, OS_MESG_BLOCK);
    return 0;
}

s32 RomFile::seek(u32 arg1) {
    if (arg1 >= (u32)this->fileSize) {
        return 0xF;
    }
    this->fileOffset = arg1;
    return 0;
}

s32 RomFile::rawClose() {
    return 8;
}

s32 RomFile::rawOpen() {
    return 8;
}

s32 RomFile::func_80048D58(char* path) {
    return 8;
}

extern "C" u8* D_800763F8;

extern "C" s32 func_80048D60(RomFile* self) {
    if (self->state & 2) {
        u32 offset = self->archiveIndex * 0x34;
        // TODO: fake match — int+int casts to force reg-allocator order on the addu
        RomFile* parent = *(RomFile**)((u32)offset + (u32)D_800763F8);
        return self->fileOffset + func_80048D60(parent);
    }
    return self->unk2C;
}

extern "C" void func_80048DD4() {
    osCreateMesgQueue(&D_800768D0, &D_800768CC, 1);
}

RomFile::~RomFile() {
    this->close();
}

RomFile::RomFile() {
    this->unk2C = 0;
}

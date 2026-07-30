typedef unsigned char u8;
typedef unsigned long u32;
typedef signed long s32;
typedef unsigned short u16;

struct C {
    u8 pad[0x40];
    /* 0x40 */ u32 unk40;
    u8 pad2[0x1E];
    /* 0x62 */ u16 bitDepth;

    void decomp(u8* src, u8* dst);
};

void C::decomp(u8* src, u8* dst) {
    C* self = this;
    s32 totalWritten = 0;
    u32 pixelPos = 0;
    u32 bytesPerPixel = (u32)(self->bitDepth + 7) >> 3;
    s32 mask = ~0x80;
    s32 negOne = -1;
    u8* dstPtr = dst;
    s32 header;
    s32 count;
    s32 savedPos;
    s32 nextPos;
    u32 j;

    while (pixelPos < self->unk40) {
        header = *src;
        src++;
        if (self->unk40 < (u32)(pixelPos + (header & 0x7F) + 1)) {
            return;
        }
        if (header & 0x80) {
            nextPos = pixelPos + 1;
            count = header & mask;
            pixelPos = nextPos + count;
            j = 0;
            savedPos = totalWritten;
            if (bytesPerPixel != 0) {
                do {
                    *dstPtr = *src;
                    src++;
                    totalWritten++;
                    j++;
                    dstPtr++;
                } while (j < bytesPerPixel);
            }
            for (count--; count != negOne; count--)
                if (bytesPerPixel != 0) {
                    j = 0;
                    do {
                        totalWritten++;
                        *dstPtr = *(dst + (savedPos + j));
                        j++;
                        dstPtr++;
                    } while (j < bytesPerPixel);
                }
        } else {
            nextPos = pixelPos + 1;
            pixelPos = nextPos + header;
            do {
                j = 0;
                if (bytesPerPixel != 0) {
                    do {
                        *dstPtr = *src;
                        src++;
                        totalWritten++;
                        j++;
                        dstPtr++;
                    } while (j < bytesPerPixel);
                }
                header--;
            } while (header != negOne);
        }
    }
}

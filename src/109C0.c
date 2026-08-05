// TODO: convert to C++ so HeapPool methods can be called directly
#include "common.h"

typedef void (*InitFunc)(void);

// Functions
void func_8000FDC0();
void func_8000FEEC(void*);
void D_8000FF88(void*);
void func_80010000();
void func_8001005C();
void func_800079F4(s32, void*);
void func_80048DD4();
void init__8HeapPoolPUcUlUl(void*, void*, s32, s32);

// Data
extern u8 D_800768F0;
extern OSViMode D_80077CD0;
extern OSThread D_80086640;
extern u8 D_800867F0[];
extern u8 D_800864B0[];
extern s32 D_800865B0;
extern u8* D_800865C0[];
extern OSThread D_800887F0;
extern u8 D_800889A0[];
extern OSMesg D_8008C9A0[];
extern OSMesgQueue D_8008CA20;
extern u8 D_80148370;
extern s32 D_8006D5F0;

extern InitFunc entry_CTORS_START;
extern InitFunc entry_CTORS_END;
extern InitFunc entry_DTORS_START;
extern InitFunc entry_DTORS_END;

void func_8000FDC0(void) {
    s32 i = 0;
    s32 len;
    u8* tok;

    D_800865B0 = 0;
    if (D_800864B0[0] == 0) {
        return;
    }
    for (; D_800865B0 < 0x20; i += len + 1) {
        len = 0;
        while (D_800864B0[i + len] && D_800864B0[i + len] != ' ') {
            len++;
        }
        D_800865C0[D_800865B0] = tok = &D_800864B0[i];
        if (D_800864B0[i + len] == 0) {
            D_800865B0++;
            return;
        }
        tok[len] = 0;
        D_800865B0++;
    }
}

void __pure_virtual(void) {
}

void boot(void) {
    osUnmapTLBAll();
    osInitialize();
    D_800864B0[0] = 0;
    osCreateThread(&D_80086640, 1, func_8000FEEC, 0, &D_800867F0[0x2000], 10);
    osStartThread(&D_80086640);
}

void func_8000FEEC(void* arg) {
    osCreateViManager(0xFE);
    osViSetMode(&D_80077CD0);
    osViBlack(1);
    osViSetSpecialFeatures(0x55);
    osCreatePiManager(150, &D_8008CA20, D_8008C9A0, 32);
    osCreateThread(&D_800887F0, 3, D_8000FF88, NULL, &D_800889A0[0x4000], 10);
    osStartThread(&D_800887F0);
    osSetThreadPri(NULL, 0);
    while (1) {}
}

void func_8000FF88(void* arg) {
    func_8001005C();
    D_8006D5F0 = 0x400000 - OS_K0_TO_PHYSICAL(&D_80148370);
    init__8HeapPoolPUcUlUl(&D_800768F0, &D_80148370, D_8006D5F0, 0x40);
    func_80048DD4();
    func_8000FDC0();
    func_800079F4(D_800865B0, &D_800865C0);
    func_80010000();
}

void func_80010000(void) {
    InitFunc* p = &entry_DTORS_START;
    InitFunc* end = &entry_DTORS_END;

    for (; p < end; p += 4) {
        if (*p != NULL) {
            (*p)();
        }
    }
}

void func_8001005C(void) {
    InitFunc* p = &entry_CTORS_START;
    InitFunc* end = &entry_CTORS_END;

    for (; p < end; p += 4) {
        if (*p != NULL) {
            (*p)();
        }
    }
}

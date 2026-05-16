#ifndef N64_CONTROLLER_SYSTEM_H
#define N64_CONTROLLER_SYSTEM_H

#include "controller_system.h"

extern "C" {
#include <PR/sched.h>
}

struct N64ControllerSystem : public ControllerSystem {
    /* 0x0054 */ s32 readIdx;
    /* 0x0058 */ s32 writeIdx;
    /* 0x005C */ s32 connected[4];
    /* 0x006C */ OSContStatus status[4];
    /* 0x007C */ s32 changeCount;
    /* 0x0080 */ s32 pollReady;
    /* 0x0084 */ u8 unk84[4];
    /* 0x0088 */ OSThread thread;
    /* 0x????*/ u8 stack[0x4000];
    /* 0x4238 */ OSContPad pads[2][4];
    /* 0x4268 */ ControllerDevice devices[4];
    /* 0x47A8 */ void* msgArgs;
    /* 0x47AC */ OSSched* sched;

    N64ControllerSystem();
    virtual void init();
    virtual ~N64ControllerSystem();
    virtual ControllerDevice* findByPort(s32 i);
    virtual ControllerDevice* findOrDefault(s32 i);
    virtual s32 shutdown();
    virtual s32 restart();
    virtual s32 poll(s32 arg1);
    virtual s32 getChangeCount();

    void func_80046BC0();
    s32 pfsFreeBlocks(OSPfs* pfs, s32* bytes_not_used);
    s32 pfsFileState(OSPfs* pfs, s32 file_no, OSPfsState* state);
    s32 pfsNumFiles(OSPfs* pfs, s32* max_files, s32* files_used);
    s32 func_800474F8(OSPfs* pfs);
    s32 motorStop(OSPfs* pfs);
    s32 motorStart(OSPfs* pfs);
    s32 motorInit(OSPfs* pfs, s32 channel);
    s32 pfsRepairId(OSPfs* pfs);
    s32 pfsReadWriteFile(OSPfs* pfs, s32 file_no, u8 flag, s32 offset, s32 nbytes, u8* data);
    s32 pfsDeleteFile(OSPfs* pfs, u16 cc, u32 gc, u8* gn, u8* en);
    s32 pfsFindFile(OSPfs* pfs, u16 cc, u32 gc, u8* gn, u8* en, s32* file_no);
    s32 pfsAllocateFile(OSPfs* pfs, u16 cc, u32 gc, u8* gn, u8* en, s32 length, s32* file_no);
    s32 tryInitAccessory(OSPfs* pfs, s32 controller_no);
    s32 pfsInitPak(OSPfs* pfs, s32 controller_no);
    s32 pfsIsPlug(u8* bitpattern);
    s32 isAbsolute(s32 i);
    s32 isJoyport(s32 i);
    void startThread();
    void refresh();
    void setSched(OSSched* s);
};

#endif

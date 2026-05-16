#include "common.h"
#include "n64_controller_system.h"

extern "C" {
extern OSMesgQueue D_800AE7C0;
extern OSMesg D_800AE7E0[];
extern OSMesgQueue D_800AE820;
extern OSMesg D_800AE838[];
extern OSMesgQueue D_800AE840;
extern OSMesg D_800AE858[];
extern OSMesgQueue D_800AE860;
extern OSMesg D_800AE878[];
extern OSScClient D_800AE880;
extern s32 D_800768A0;
extern void (*D_800768A4)(void);
extern void threadEntry(void*);
extern char D_800766A0[];
extern char D_800766C0[];
extern char D_800766E0[];
extern char D_80076700[];
extern char D_80076720[];
extern char D_80076740[];
extern char D_80076760[];
extern char D_80076780[];
extern char D_800767A0[];
extern char D_800767C0[];
extern char D_800767E0[];
extern char D_80076800[];
extern char D_80076820[];
extern char D_80076840[];
extern char D_80076860[];
extern char D_80076880[];
}

struct PlugArgs {
    u8* bitpattern;
    s32 out;
};

struct InitPakArgs {
    OSPfs* pfs;
    s32 controller_no;
    s32 out;
};

struct AllocateFileArgs {
    OSPfs* pfs;
    u16 company_code;
    u32 game_code;
    u8* game_name;
    u8* ext_name;
    s32 length;
    s32* file_no;
    s32 out;
};

struct FindFileArgs {
    OSPfs* pfs;
    u16 company_code;
    u32 game_code;
    u8* game_name;
    u8* ext_name;
    s32* file_no;
    s32 out;
};

struct DeleteFileArgs {
    OSPfs* pfs;
    u16 company_code;
    u32 game_code;
    u8* game_name;
    u8* ext_name;
    s32 out;
};

struct ReadWriteFileArgs {
    OSPfs* pfs;
    s32 file_no;
    u8 flag;
    s32 offset;
    s32 nbytes;
    u8* data;
    s32 out;
};

struct RepairIdArgs {
    OSPfs* pfs;
    s32 out;
};

struct MotorArgs {
    OSPfs* pfs;
    s32 channel;
    s32 out;
};

struct NumFilesArgs {
    OSPfs* pfs;
    s32* max_files;
    s32* files_used;
    s32 out;
};

struct FileStateArgs {
    OSPfs* pfs;
    s32 file_no;
    OSPfsState* state;
    s32 out;
};

struct FreeBlocksArgs {
    OSPfs* pfs;
    s32* bytes_not_used;
    s32 out;
};

void N64ControllerSystem::func_80046BC0() {
    shutdown();
    memset(&D_800AE880, 0, 8);
    osCreateMesgQueue(&D_800AE7C0, D_800AE7E0, 0x10);
    osCreateMesgQueue(&D_800AE820, D_800AE838, 1);
    osScAddClient(sched, &D_800AE880, &D_800AE7C0);
    osCreateThread(&thread, 2, &threadEntry, this, &stack[sizeof(stack)], 0x33);
    osStartThread(&thread);
    D_800768A0 = 1;
    osSendMesg(&D_800AE7C0, &D_800766A0, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    s32 idx = 0;
    for (s32 i = 0; i < 4; i++) {
        if (connected[i]) {
            devices[i].func_8004880C(this, i);
            entries[idx++] = &devices[i];
        }
    }
    unk28 = 1;
}

s32 N64ControllerSystem::restart() {
    for (s32 i = 0; i < 4; i++) {
        if (entries[i] != NULL) {
            entries[i]->disconnect();
            entries[i] = NULL;
        }
    }
    memset(connected, 0, sizeof(connected));
    changeCount = 0;
    osSendMesg(&D_800AE7C0, &D_800766A0, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    s32 idx = 0;
    for (s32 i = 0; i < 4; i++) {
        if (connected[i]) {
            devices[i].func_8004880C(this, i);
            entries[idx++] = &devices[i];
        }
    }
    return 1;
}

extern "C" void threadEntry(void* arg) {
    N64ControllerSystem* this_ = (N64ControllerSystem*)arg;
    OSMesg sp24;
    u8 sp20;
    s32 i;

    osCreateMesgQueue(&D_800AE860, D_800AE878, 1);
    osCreateMesgQueue(&D_800AE840, D_800AE858, 1);
    osSetEventMesg(5, &D_800AE840, (OSMesg)1);
    osContInit(&D_800AE840, &sp20, this_->status);

    while (1) {
        osRecvMesg(&D_800AE7C0, &sp24, OS_MESG_BLOCK);
        s16 type = *(s16*)sp24;
        switch (type) {
            case 1: {
                if (this_->unk28 != 0) {
                    osContStartQuery(&D_800AE840);
                    osRecvMesg(&D_800AE840, NULL, OS_MESG_BLOCK);
                    osContGetQuery(this_->status);
                    this_->changeCount = 0;
                    for (i = 0; i < 4; i++) {
                        if ((this_->status[i].type & 5) && (this_->connected[i] != !(this_->status[i].errno & 0xC))) {
                            this_->changeCount++;
                        }
                    }
                    osContStartReadData(&D_800AE840);
                    osRecvMesg(&D_800AE840, NULL, OS_MESG_BLOCK);
                    osContGetReadData(this_->pads[this_->writeIdx]);
                    this_->pollReady = 1;
                    if (D_800768A4 != NULL) {
                        ((void (*)())D_800768A4)();
                    }
                }
                break;
            }
            case 4: {
                for (i = 0; i < 4; i++) {
                    ControllerDevice* d = this_->entries[i];
                    if (d != NULL && d->pakPresent != 0) {
                        osMotorInit(&D_800AE840, &d->pfs, d->port);
                        osMotorStop(&d->pfs);
                        osMotorStop(&d->pfs);
                        osMotorStop(&d->pfs);
                    }
                }
                break;
            }
            case 7: {
                memset(this_->status, 0, sizeof(this_->status));
                osContStartQuery(&D_800AE840);
                osRecvMesg(&D_800AE840, NULL, OS_MESG_BLOCK);
                osContGetQuery(this_->status);
                this_->count = 0;
                for (i = 0; i < 4; i++) {
                    if (!(this_->status[i].type & 5)) {
                        this_->connected[i] = 0;
                    } else {
                        this_->connected[i] = (this_->status[i].errno & 0xC) == 0;
                        if (this_->connected[i]) {
                            this_->count++;
                        }
                    }
                }
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 10: {
                PlugArgs* args = (PlugArgs*)this_->msgArgs;
                args->out = osPfsIsPlug(&D_800AE840, args->bitpattern);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 11: {
                InitPakArgs* args = (InitPakArgs*)this_->msgArgs;
                args->out = osPfsInitPak(&D_800AE840, args->pfs, args->controller_no);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 12: {
                AllocateFileArgs* args = (AllocateFileArgs*)this_->msgArgs;
                args->out = osPfsAllocateFile(args->pfs, args->company_code, args->game_code, args->game_name, args->ext_name, args->length, args->file_no);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 13: {
                FindFileArgs* args = (FindFileArgs*)this_->msgArgs;
                args->out = osPfsFindFile(args->pfs, args->company_code, args->game_code, args->game_name, args->ext_name, args->file_no);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 14: {
                DeleteFileArgs* args = (DeleteFileArgs*)this_->msgArgs;
                args->out = osPfsDeleteFile(args->pfs, args->company_code, args->game_code, args->game_name, args->ext_name);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 15: {
                ReadWriteFileArgs* args = (ReadWriteFileArgs*)this_->msgArgs;
                args->out = osPfsReadWriteFile(args->pfs, args->file_no, args->flag, args->offset, args->nbytes, args->data);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 16: {
                RepairIdArgs* args = (RepairIdArgs*)this_->msgArgs;
                args->out = osPfsRepairId(args->pfs);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 17: {
                MotorArgs* args = (MotorArgs*)this_->msgArgs;
                args->out = osMotorInit(&D_800AE840, args->pfs, args->channel);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 18: {
                MotorArgs* args = (MotorArgs*)this_->msgArgs;
                args->out = osMotorStart(args->pfs);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 19: {
                MotorArgs* args = (MotorArgs*)this_->msgArgs;
                args->out = osMotorStop(args->pfs);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 23: {
                MotorArgs* args = (MotorArgs*)this_->msgArgs;
                args->out = osMotorStop(args->pfs);
                args->out = osMotorStop(args->pfs);
                args->out = osMotorStop(args->pfs);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 20: {
                NumFilesArgs* args = (NumFilesArgs*)this_->msgArgs;
                args->out = osPfsNumFiles(args->pfs, args->max_files, args->files_used);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 21: {
                FileStateArgs* args = (FileStateArgs*)this_->msgArgs;
                args->out = osPfsFileState(args->pfs, args->file_no, args->state);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
            case 22: {
                FreeBlocksArgs* args = (FreeBlocksArgs*)this_->msgArgs;
                args->out = osPfsFreeBlocks(args->pfs, args->bytes_not_used);
                osSendMesg(&D_800AE820, &D_800766C0, OS_MESG_BLOCK);
                break;
            }
        }
    }
}

ControllerDevice* N64ControllerSystem::findOrDefault(s32 i) {
    return &devices[i];
}

ControllerDevice* N64ControllerSystem::findByPort(s32 i) {
    return &devices[i];
}

extern "C" void setEventCallback(void (*fn)(void)) {
    D_800768A4 = fn;
}

s32 N64ControllerSystem::pfsFreeBlocks(OSPfs* pfs, s32* bytes_not_used) {
    FreeBlocksArgs msg;
    msg.pfs = pfs;
    msg.bytes_not_used = bytes_not_used;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076880, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::pfsFileState(OSPfs* pfs, s32 file_no, OSPfsState* state) {
    FileStateArgs msg;
    msg.pfs = pfs;
    msg.file_no = file_no;
    msg.state = state;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076860, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::pfsNumFiles(OSPfs* pfs, s32* max_files, s32* files_used) {
    NumFilesArgs msg;
    msg.pfs = pfs;
    msg.max_files = max_files;
    msg.files_used = files_used;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076840, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::func_800474F8(OSPfs* pfs) {
    MotorArgs msg;
    msg.pfs = pfs;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076820, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::motorStop(OSPfs* pfs) {
    MotorArgs msg;
    msg.pfs = pfs;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076800, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::motorStart(OSPfs* pfs) {
    MotorArgs msg;
    msg.pfs = pfs;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_800767E0, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::motorInit(OSPfs* pfs, s32 channel) {
    MotorArgs msg;
    msg.pfs = pfs;
    msg.channel = channel;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_800767C0, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::pfsRepairId(OSPfs* pfs) {
    RepairIdArgs msg;
    msg.pfs = pfs;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_800767A0, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::pfsReadWriteFile(OSPfs* pfs, s32 file_no, u8 flag, s32 offset, s32 nbytes, u8* data) {
    ReadWriteFileArgs msg;
    msg.pfs = pfs;
    msg.file_no = file_no;
    msg.flag = flag;
    msg.offset = offset;
    msg.nbytes = nbytes;
    msg.data = data;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076780, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::pfsDeleteFile(OSPfs* pfs, u16 cc, u32 gc, u8* gn, u8* en) {
    DeleteFileArgs msg;
    msg.pfs = pfs;
    msg.company_code = cc;
    msg.game_code = gc;
    msg.game_name = gn;
    msg.ext_name = en;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076760, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::pfsFindFile(OSPfs* pfs, u16 cc, u32 gc, u8* gn, u8* en, s32* file_no) {
    FindFileArgs msg;
    msg.pfs = pfs;
    msg.company_code = cc;
    msg.game_code = gc;
    msg.game_name = gn;
    msg.ext_name = en;
    msg.file_no = file_no;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076740, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::pfsAllocateFile(OSPfs* pfs, u16 cc, u32 gc, u8* gn, u8* en, s32 length, s32* file_no) {
    AllocateFileArgs msg;
    msg.pfs = pfs;
    msg.company_code = cc;
    msg.game_code = gc;
    msg.game_name = gn;
    msg.ext_name = en;
    msg.length = length;
    msg.file_no = file_no;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076720, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

// reg-alloc: target keeps 0xB constant in s0 via bne+delay-slot trick; mine emits bnel and loses it
#ifdef NON_MATCHING
s32 N64ControllerSystem::func_800478DC(OSPfs* pfs, s32 controller_no) {
    InitPakArgs msg;
    msg.pfs = pfs;
    msg.controller_no = controller_no;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076700, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    s32 v = msg.out;
    if (v != 0xB)
        return v;
    v = motorInit(pfs, controller_no);
    if (v == 0xB)
        return 0xA;
    return v;
}
#else
INCLUDE_ASM("asm/nonmatchings/477C0", func_800478DC);
#endif

s32 N64ControllerSystem::pfsInitPak(OSPfs* pfs, s32 controller_no) {
    InitPakArgs msg;
    msg.pfs = pfs;
    msg.controller_no = controller_no;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_80076700, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::pfsIsPlug(u8* bitpattern) {
    PlugArgs msg;
    msg.bitpattern = bitpattern;
    msgArgs = &msg;
    osSendMesg(&D_800AE7C0, &D_800766E0, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
    msgArgs = NULL;
    return msg.out;
}

s32 N64ControllerSystem::isAbsolute(s32 i) {
    return status[i].type & 4;
}

s32 N64ControllerSystem::isJoyport(s32 i) {
    return status[i].type & 1;
}

s32 N64ControllerSystem::getChangeCount() {
    return changeCount;
}

// reg-alloc: target keeps 3 induction vars (i*0x150, 0x4268+i*0x150, i*6); compiler folds to fewer here
#ifdef NON_MATCHING
s32 N64ControllerSystem::poll(s32 arg1) {
    s32 result = 0;
    if (pollReady == 0) {
        return 0;
    }
    for (s32 i = 0; i < 4; i++) {
        if (devices[i].enabled != 0) {
            devices[i].pad = &pads[readIdx][i];
            s32 r = devices[i].vfunc6(arg1);
            if (r != 0) {
                result = r;
            }
        }
    }
    if (result == 0 && changeCount != 0) {
        result = 1;
    }
    s32 t = writeIdx;
    pollReady = 0;
    readIdx = t;
    writeIdx = (t == 0);
    return result;
}
#else
INCLUDE_ASM("asm/nonmatchings/477C0", poll__19N64ControllerSysteml);
#endif
// scheduling: target places `li v0,1` before register restores (offset 104c); mine puts it last
#ifdef NON_MATCHING
s32 N64ControllerSystem::shutdown() {
    if (unk28 != 0) {
        s32 i = 0;
        do {
            ControllerDevice* d = entries[i];
            if (d != NULL) {
                d->disconnect();
                entries[i] = NULL;
            }
            i++;
        } while (i < 4);
        osStopThread(&thread);
        init();
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/477C0", shutdown__19N64ControllerSystem);
#endif

void N64ControllerSystem::startThread() {
    memset(&D_800AE880, 0, 8);
    osCreateMesgQueue(&D_800AE7C0, D_800AE7E0, 0x10);
    osCreateMesgQueue(&D_800AE820, D_800AE838, 1);
    osScAddClient(sched, &D_800AE880, &D_800AE7C0);
    osCreateThread(&thread, 2, &threadEntry, this, &stack[sizeof(stack)], 0x33);
    osStartThread(&thread);
    D_800768A0 = 1;
}

void N64ControllerSystem::refresh() {
    osSendMesg(&D_800AE7C0, &D_800766A0, 1);
    osRecvMesg(&D_800AE820, NULL, 1);
}

N64ControllerSystem::~N64ControllerSystem() {
    shutdown();
}

N64ControllerSystem::N64ControllerSystem() {
    memset(connected, 0, 0x10);
    memset(pads, 0, 0x30);
    pollReady = 0;
    readIdx = 0;
    writeIdx = 1;
    changeCount = 0;
    ControllerSystem::init();
}

void N64ControllerSystem::init() {
    memset(connected, 0, 0x10);
    memset(pads, 0, 0x30);
    pollReady = 0;
    readIdx = 0;
    writeIdx = 1;
    changeCount = 0;
    ControllerSystem::init();
}

void N64ControllerSystem::setSched(OSSched* s) {
    this->sched = s;
}

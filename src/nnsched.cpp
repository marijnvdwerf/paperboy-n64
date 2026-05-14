#include "common.h"
#include "sched.h"

// NNSched scheduler (adapted from N64 SDK kantan-demos nnsched.c, with perf tracking)

struct NNScClient {
    /* 0x0 */ NNScClient* next;
    /* 0x4 */ OSMesgQueue* msgQ;
};

struct NNSched {
    /* 0x000 */ u32 retraceMsg; // NN_SC_RETRACE_MSG
    /* 0x004 */ u32 prenmiMsg; // NN_SC_PRE_NMI_MSG
    /* 0x008 */ OSMesgQueue audioRequestMQ;
    /* 0x020 */ OSMesg audioRequestBuf[8];
    /* 0x040 */ OSMesgQueue graphicsRequestMQ;
    /* 0x058 */ OSMesg graphicsRequestBuf[8];
    /* 0x078 */ OSMesgQueue retraceMQ;
    /* 0x090 */ OSMesg retraceMsgBuf[8];
    /* 0x0B0 */ OSMesgQueue rspMQ;
    /* 0x0C8 */ OSMesg rspMsgBuf[8];
    /* 0x0E8 */ OSMesgQueue rdpMQ;
    /* 0x100 */ OSMesg rdpMsgBuf[8];
    /* 0x120 */ OSMesgQueue waitMQ;
    /* 0x138 */ OSMesg waitMsgBuf[8];
    /* 0x158 */ OSThread schedulerThread;
    /* 0x308 */ OSThread audioThread;
    /* 0x4B8 */ OSThread graphicsThread;
    /* 0x668 */ NNScClient* clientList;
    /* 0x66C */ OSScTask* curGraphicsTask;
    /* 0x670 */ OSScTask* curAudioTask;
    /* 0x674 */ OSScTask* graphicsTaskSuspended;
    /* 0x678 */ u32 firstTime;
};

#define NN_SC_STACKSIZE 0x2000
#define NN_SC_AUTASK_NUM 4
#define NN_SC_GTASK_NUM 8
#define NN_SC_PERF_NUM 4

struct NNScPerf {
    /* 0x000 */ u32 gtask_cnt;
    /* 0x004 */ u32 autask_cnt;
    /* 0x008 */ u64 retrace_time;
    /* 0x010 */ u64 gtask_stime[NN_SC_GTASK_NUM];
    /* 0x050 */ u64 rdp_etime[NN_SC_GTASK_NUM];
    /* 0x090 */ u64 rsp_etime[NN_SC_GTASK_NUM];
    /* 0x0D0 */ u64 autask_stime[NN_SC_AUTASK_NUM];
    /* 0x0F0 */ u64 autask_etime[NN_SC_AUTASK_NUM];
    /* 0x110 */ u64 pad110;
};

extern u32 framecont;
extern u32 nnsc_perf_index;
extern u32 nnsc_perf_flag;
extern NNScPerf* nnsc_perf_ptr;
extern u64 nnScStack[NN_SC_STACKSIZE / 8];
extern u64 nnScAudioStack[NN_SC_STACKSIZE / 8];
extern u64 nnScGraphicsStack[NN_SC_STACKSIZE / 8];
extern NNScPerf nnsc_perf[NN_SC_PERF_NUM];
extern NNScPerf* nnsc_perf_inptr;

void nnScEventHandler(NNSched*);
void nnScExecuteAudio(NNSched*);
void nnScExecuteGraphics(NNSched*);

void nnScCreateScheduler(NNSched* sc) {
    sc->firstTime = 1;
    sc->retraceMsg = 1; // NN_SC_RETRACE_MSG
    sc->curGraphicsTask = NULL;
    sc->curAudioTask = NULL;
    sc->graphicsTaskSuspended = NULL;
    sc->clientList = NULL;
    sc->prenmiMsg = 3; // NN_SC_PRE_NMI_MSG
    osCreateMesgQueue(&sc->retraceMQ, sc->retraceMsgBuf, 8);
    osCreateMesgQueue(&sc->rspMQ, sc->rspMsgBuf, 8);
    osCreateMesgQueue(&sc->rdpMQ, sc->rdpMsgBuf, 8);
    osCreateMesgQueue(&sc->graphicsRequestMQ, sc->graphicsRequestBuf, 8);
    osCreateMesgQueue(&sc->audioRequestMQ, sc->audioRequestBuf, 8);
    osCreateMesgQueue(&sc->waitMQ, sc->waitMsgBuf, 8);
    osViSetEvent(&sc->retraceMQ, (OSMesg)0x29A, 1);
    osSetEventMesg(OS_EVENT_SP, &sc->rspMQ, (OSMesg)0x29B);
    osSetEventMesg(OS_EVENT_DP, &sc->rdpMQ, (OSMesg)0x29C);
    osSetEventMesg(OS_EVENT_PRENMI, &sc->retraceMQ, (OSMesg)0x29D);
    osCreateThread(&sc->schedulerThread, 0x13, (void (*)(void*))&nnScEventHandler, sc, nnScStack + NN_SC_STACKSIZE / sizeof(u64), 0x78);
    osStartThread(&sc->schedulerThread);
    osCreateThread(&sc->audioThread, 0x12, (void (*)(void*))&nnScExecuteAudio, sc, nnScAudioStack + NN_SC_STACKSIZE / sizeof(u64), 0x6E);
    osStartThread(&sc->audioThread);
    osCreateThread(&sc->graphicsThread, 0x11, (void (*)(void*))&nnScExecuteGraphics, sc, nnScGraphicsStack + NN_SC_STACKSIZE / sizeof(u64), 0x64);
    osStartThread(&sc->graphicsThread);
}

void nnScEventBroadcast(NNSched* sc, u32* msg);

void nnScEventHandler(NNSched* sc) {
    OSMesg msg = (OSMesg)0;
    while (1) {
        osRecvMesg(&sc->retraceMQ, &msg, OS_MESG_BLOCK);
        framecont++;
        switch ((s32)msg) {
            case 0x29A: // VIDEO_MSG
                nnScEventBroadcast(sc, &sc->retraceMsg);
                if (nnsc_perf_flag == 0) {
                    nnsc_perf_flag++;
                    nnsc_perf_ptr = &nnsc_perf[nnsc_perf_index];
                    nnsc_perf_index = (nnsc_perf_index + 1) & 3;
                    nnsc_perf_inptr = &nnsc_perf[nnsc_perf_index];
                    nnsc_perf_inptr->autask_cnt = 0;
                    nnsc_perf_inptr->gtask_cnt = 0;
                    nnsc_perf_inptr->retrace_time = ((u64)osGetTime() << 6) / 0xBB8ULL;
                }
                break;
            case 0x29D: // PRE_NMI_MSG
                nnScEventBroadcast(sc, &sc->prenmiMsg);
                break;
        }
    }
}

void nnScWaitTaskReady(NNSched* sc, OSScTask* task);

void nnScExecuteAudio(NNSched* sc) {
    OSMesg msg = (OSMesg)0;
    OSScTask* task = (OSScTask*)0;
    OSScTask* gfxTask = (OSScTask*)0;
    u32 yieldFlag = 0;
    u32 task_cnt;
    while (1) {
        osRecvMesg(&sc->audioRequestMQ, (OSMesg*)&task, OS_MESG_BLOCK);
        osWritebackDCacheAll();
        yieldFlag = 0;
        gfxTask = sc->curGraphicsTask;
        if (gfxTask) {
            osSpTaskYield();
            osRecvMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);
            if (osSpTaskYielded(&gfxTask->list)) {
                yieldFlag = 1;
            } else {
                yieldFlag = 2;
            }
        }
        if (nnsc_perf_inptr->autask_cnt < NN_SC_AUTASK_NUM) {
            task_cnt = nnsc_perf_inptr->autask_cnt;
            nnsc_perf_inptr->autask_stime[task_cnt] =
                ((u64)osGetTime() << 6) / 0xBB8ULL - nnsc_perf_inptr->retrace_time;
        }
        sc->curAudioTask = task;
        osSpTaskLoad(&task->list);
        osSpTaskStartGo(&task->list);
        osRecvMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);
        sc->curAudioTask = NULL;
        if (nnsc_perf_inptr->autask_cnt < NN_SC_AUTASK_NUM) {
            nnsc_perf_inptr->autask_etime[task_cnt] =
                ((u64)osGetTime() << 6) / 0xBB8ULL - nnsc_perf_inptr->retrace_time;
            nnsc_perf_inptr->autask_cnt++;
        }
        if (sc->graphicsTaskSuspended) {
            osSendMesg(&sc->waitMQ, &msg, OS_MESG_BLOCK);
        }
        if (yieldFlag == 1) {
            osSpTaskLoad(&gfxTask->list);
            osSpTaskStartGo(&gfxTask->list);
        } else if (yieldFlag == 2) {
            osSendMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);
        }
        osSendMesg(task->msgQ, task->msg, OS_MESG_BLOCK);
    }
}

void nnScExecuteGraphics(NNSched* sc) {
    OSMesg msg = (OSMesg)0;
    OSScTask* task;
    u32 task_cnt;
    while (1) {
        osRecvMesg(&sc->graphicsRequestMQ, (OSMesg*)&task, OS_MESG_BLOCK);
        osWritebackDCacheAll();
        nnScWaitTaskReady(sc, task);
        if (sc->curAudioTask) {
            sc->graphicsTaskSuspended = task;
            osRecvMesg(&sc->waitMQ, &msg, OS_MESG_BLOCK);
            sc->graphicsTaskSuspended = NULL;
        }
        if (nnsc_perf_inptr->gtask_cnt < NN_SC_GTASK_NUM) {
            task_cnt = nnsc_perf_inptr->gtask_cnt;
            nnsc_perf_inptr->gtask_stime[task_cnt] =
                ((u64)osGetTime() << 6) / 0xBB8ULL - nnsc_perf_inptr->retrace_time;
        }
        sc->curGraphicsTask = task;
        osSpTaskLoad(&task->list);
        osSpTaskStartGo(&task->list);
        osRecvMesg(&sc->rspMQ, &msg, OS_MESG_BLOCK);
        sc->curGraphicsTask = NULL;
        if (nnsc_perf_inptr->gtask_cnt < NN_SC_GTASK_NUM) {
            nnsc_perf_inptr->rsp_etime[task_cnt] =
                ((u64)osGetTime() << 6) / 0xBB8ULL - nnsc_perf_inptr->retrace_time;
        }
        osRecvMesg(&sc->rdpMQ, &msg, OS_MESG_BLOCK);
        if (nnsc_perf_inptr->gtask_cnt < NN_SC_GTASK_NUM) {
            nnsc_perf_inptr->rdp_etime[task_cnt] =
                ((u64)osGetTime() << 6) / 0xBB8ULL - nnsc_perf_inptr->retrace_time;
            nnsc_perf_inptr->gtask_cnt++;
        }
        if (task->flags & OS_SC_SWAPBUFFER) {
            osViSwapBuffer(task->framebuffer);
            nnsc_perf_flag = 0;
        }
        osSendMesg(task->msgQ, task->msg, OS_MESG_BLOCK);
    }
}

inline void nnScAddClient_(NNSched* sc, NNScClient* c, OSMesgQueue* msgQ) {
    OSIntMask mask;
    mask = osSetIntMask(OS_IM_NONE);
    c->msgQ = msgQ;
    c->next = sc->clientList;
    sc->clientList = c;
    osSetIntMask(mask);
}

inline void nnScRemoveClient_(NNSched* sc, NNScClient* c) {
    NNScClient* client = sc->clientList;
    NNScClient* prev = NULL;
    OSIntMask mask;
    mask = osSetIntMask(OS_IM_NONE);
    while (client != NULL) {
        if (client == c) {
            if (prev) {
                prev->next = c->next;
            } else {
                sc->clientList = c->next;
            }
            break;
        }
        prev = client;
        client = client->next;
    }
    osSetIntMask(mask);
}

void nnScWaitTaskReady(NNSched* sc, OSScTask* task) {
    OSMesg msg = (OSMesg)0;
    NNScClient client;
    void* fb = task->framebuffer;
    while (osViGetCurrentFramebuffer() == fb || osViGetNextFramebuffer() == fb) {
        nnScAddClient_(sc, &client, &sc->waitMQ);
        osRecvMesg(&sc->waitMQ, &msg, OS_MESG_BLOCK);
        nnScRemoveClient_(sc, &client);
    }
}

void nnScEventBroadcast(NNSched* sc, u32* msg) {
    NNScClient* client;
    for (client = sc->clientList; client != NULL; client = client->next) {
        osSendMesg(client->msgQ, (OSMesg)msg, OS_MESG_NOBLOCK);
    }
}

void nnScRemoveClient(NNSched* sc, NNScClient* c) {
    NNScClient* client = sc->clientList;
    NNScClient* prev = NULL;
    OSIntMask mask;
    mask = osSetIntMask(OS_IM_NONE);
    while (client != NULL) {
        if (client == c) {
            if (prev) {
                prev->next = c->next;
            } else {
                sc->clientList = c->next;
            }
            break;
        }
        prev = client;
        client = client->next;
    }
    osSetIntMask(mask);
}

void nnScAddClient(NNSched* sc, NNScClient* c, OSMesgQueue* msgQ) {
    OSIntMask mask;
    mask = osSetIntMask(OS_IM_NONE);
    c->msgQ = msgQ;
    c->next = sc->clientList;
    sc->clientList = c;
    osSetIntMask(mask);
}

OSMesgQueue* nnScGetGfxMQ(NNSched* sc) {
    return &sc->graphicsRequestMQ;
}

OSMesgQueue* nnScGetAudioMQ(NNSched* sc) {
    return &sc->audioRequestMQ;
}

#ifndef GOANNA_H
#define GOANNA_H

#pragma interface

#include "common.h"
#include "vector.h"

class Parrot;
struct GoannaTrack;

struct Quat4b {
    s8 x, y, z, w;
};

struct Goanna {
    /* 0x00 */ u16 numOffsets;
    /* 0x02 */ u16 numRotations;
    /* 0x04 */ u16 numTimes;
    /* 0x08 */ Vec3f* offsets;
    /* 0x0C */ Quat4b* rotations;
    /* 0x10 */ u16* times;

    Goanna();
    ~Goanna();

    void func_80020410(const char* name, s32 useCockatoo);
    void func_80020548(Parrot* file);
    s32 func_80020860(Vec3f* out, GoannaTrack* track, f32 time, s32 totalFrames);
    s32 func_80020B00(f32* out, GoannaTrack* track, f32 time, s32 totalFrames);
    void func_80020D24(s32 index, f32* out);
    static void func_80020DA4(s32 value);
    void func_80020DB0();
    void func_80020E40();
};

struct GoannaTrack {
    /* 0x00 */ s32 rotStart;
    /* 0x04 */ s32 rotTimeBase;
    /* 0x08 */ s32 offsetStart;
    /* 0x0C */ s32 offsetTimeBase;
    /* 0x10 */ u16 rotCount;
    /* 0x12 */ u16 offsetCount;
};

#endif

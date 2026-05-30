#include "common.h"
#include "cockatoo.h"
#include "goanna.h"

extern "C" s32 func_8004B3BC(s32);
extern "C" s32 func_8004B390(void);
extern "C" void quat_nlerp(f32* a, f32* b, f32 t, f32* out);
extern "C" s32 D_80070B00;

class OtherBird : public Parrot {
  public:
    /* 0x650 */ u8 pad650[8];

    virtual void selectDriver(const char* path) CXX_OVERRIDE;
    virtual char* getExtension() CXX_OVERRIDE;
    virtual void parseError(s32 code) CXX_OVERRIDE;
    virtual s32 nextToken() CXX_OVERRIDE;
    virtual s32 vfunc21(char* name, s32 len) CXX_OVERRIDE;
};

class Lorikeet : public OtherBird {};

void Goanna::func_80020410(const char* name, s32 useCockatoo) {
    if (this->rotations) {
        this->func_80020E40();
    }

    Parrot* file;
    if (useCockatoo) {
        file = new Cockatoo();
        if (!file)
            __assert("", NULL, 0, NULL);
        file->setExtension(".adb");
    } else {
        file = new Lorikeet();
        if (!file)
            __assert("", NULL, 0, NULL);
    }

    file->selectDriver(name);
    file->expectToken(0x27);
    this->func_80020548(file);
    file->close();
    delete file;
}

void Goanna::func_80020548(Parrot* file) {
    u32 i;
    s32 count;

    file->expectToken(TOKEN_OPEN_BRACE);

    while (1) {
        u32 token = file->nextToken();

        if (token == TOKEN_CLOSE_BRACE) {
            return;
        }

        switch (token) {
            case 0x28: {
                // offsets (OFFSET)
                count = file->beginArray();
                func_8004B3BC(D_80070B00);
                this->offsets = new Vec3f[count];
                func_8004B390();
                if (!this->offsets)
                    __assert("", NULL, 0, NULL);
                this->numOffsets = count;
                for (i = 0; i < count; i++) {
                    this->offsets[i].x = file->readFloat();
                    this->offsets[i].y = file->readFloat();
                    this->offsets[i].z = file->readFloat();
                }
                file->expectToken(TOKEN_CLOSE_BRACE);
                break;
            }

            case 0x29: {
                // rotations (ROTATION)
                count = file->beginArray();
                func_8004B3BC(D_80070B00);
                this->rotations = new Quat4b[count];
                func_8004B390();
                if (!this->rotations)
                    __assert("", NULL, 0, NULL);
                this->numRotations = count;
                for (i = 0; i < count; i++) {
                    this->rotations[i].x = (s32)(file->readFloat() * 127.0f);
                    this->rotations[i].y = (s32)(file->readFloat() * 127.0f);
                    this->rotations[i].z = (s32)(file->readFloat() * 127.0f);
                    this->rotations[i].w = (s32)(file->readFloat() * 127.0f);
                }
                file->expectToken(TOKEN_CLOSE_BRACE);
                break;
            }

            case 0x2A: {
                // times (TIME)
                count = file->beginArray();
                func_8004B3BC(D_80070B00);
                this->times = new u16[count];
                func_8004B390();
                if (!this->times)
                    __assert("", NULL, 0, NULL);
                this->numTimes = count;
                for (i = 0; i < count; i++) {
                    this->times[i] = file->readInt();
                }
                file->expectToken(TOKEN_CLOSE_BRACE);
                break;
            }

            default:
                file->parseError(0);
                break;
        }
    }
}

s32 Goanna::func_80020860(Vec3f* out, GoannaTrack* track, f32 time, s32 totalFrames) {
    if (track->offsetCount == 0) {
        return 0;
    }

    if (track->offsetCount == 1) {
        *out = this->offsets[track->offsetStart];
    } else {
        u32 j;
        for (j = 0; j < track->offsetCount; j++) {
            if (time < (f32)this->times[track->offsetTimeBase + j]) {
                break;
            }
        }

        Vec3f* prev;
        Vec3f* next;
        s32 gap;
        f32 fracNum;
        s32 idx;

        if (j == 0) {
            u32 cnt = track->offsetCount;
            s32 timeBase = track->offsetTimeBase;
            s32 start = track->offsetStart;
            idx = timeBase + cnt - 1;
            u16 lastKeyTime = this->times[idx];
            u16 firstKeyTime = this->times[timeBase];
            gap = totalFrames - (lastKeyTime - firstKeyTime);
            fracNum = (f32)(gap - firstKeyTime) + time;
            prev = &this->offsets[start + cnt - 1];
            next = &this->offsets[start];
        } else if (j == track->offsetCount) {
            s32 timeBase = track->offsetTimeBase;
            s32 start = track->offsetStart;
            idx = timeBase + j - 1;
            u16 lastKeyTime = this->times[idx];
            u16 firstKeyTime = this->times[timeBase];
            gap = totalFrames - (lastKeyTime - firstKeyTime);
            fracNum = time - (f32)lastKeyTime;
            prev = &this->offsets[start + j - 1];
            next = &this->offsets[start];
        } else {
            idx = track->offsetTimeBase + j;
            u16 nextKeyTime = this->times[idx];
            u16 prevKeyTime = this->times[idx - 1];
            gap = nextKeyTime - prevKeyTime;
            fracNum = time - (f32)prevKeyTime;
            idx = track->offsetStart + j;
            prev = &this->offsets[idx - 1];
            next = &this->offsets[idx];
        }

        if (gap == 0) {
            fracNum = 0.0f;
        } else {
            fracNum = fracNum / (f32)gap;
        }

        f32 temp[3];
        out->x = next->x - prev->x;
        out->y = next->y - prev->y;
        out->z = next->z - prev->z;
        temp[0] = out->x * fracNum;
        temp[1] = out->y * fracNum;
        temp[2] = out->z * fracNum;
        out->x = prev->x + temp[0];
        out->y = prev->y + temp[1];
        out->z = prev->z + temp[2];
    }

    return 1;
}

s32 Goanna::func_80020B00(f32* out, GoannaTrack* track, f32 time, s32 totalFrames) {
    u32 count = track->rotCount;

    if (count == 0) {
        return 0;
    }

    if (count == 1) {
        this->func_80020D24(track->rotStart, out);
        return 1;
    }

    u32 j = 0;
    if (count != 0) {
        s32 timeBase = track->rotTimeBase;
        while (1) {
            if (time < (f32)this->times[timeBase + j]) {
                break;
            }
            j++;
            if (j >= count) {
                break;
            }
        }
    }

    f32 q0[4];
    f32 q1[4];
    s32 gap;
    f32 fracNum;
    s32 idx;

    if (j == 0) {
        u32 cnt = track->rotCount;
        s32 timeBase = track->rotTimeBase;
        s32 start = track->rotStart;
        idx = timeBase + cnt - 1;
        u16 lastKeyTime = this->times[idx];
        u16 firstKeyTime = this->times[timeBase];
        gap = totalFrames - (lastKeyTime - firstKeyTime);
        fracNum = (f32)(gap - firstKeyTime) + time;
        this->func_80020D24(start + cnt - 1, q0);
        this->func_80020D24(track->rotStart, q1);
    } else if (j == track->rotCount) {
        s32 timeBase = track->rotTimeBase;
        s32 start = track->rotStart;
        idx = timeBase + j - 1;
        u16 lastKeyTime = this->times[idx];
        u16 firstKeyTime = this->times[timeBase];
        gap = totalFrames - (lastKeyTime - firstKeyTime);
        fracNum = time - (f32)lastKeyTime;
        this->func_80020D24(start + j - 1, q0);
        this->func_80020D24(track->rotStart, q1);
    } else {
        idx = track->rotTimeBase + j;
        u16 nextKeyTime = this->times[idx];
        u16 prevKeyTime = this->times[idx - 1];
        gap = nextKeyTime - prevKeyTime;
        fracNum = time - (f32)prevKeyTime;
        idx = track->rotStart + j;
        this->func_80020D24(idx - 1, q0);
        this->func_80020D24(idx, q1);
    }

    if (gap == 0) {
        fracNum = 0.0f;
    } else {
        fracNum = fracNum / (f32)gap;
    }

    quat_nlerp(q0, q1, fracNum, out);
    return 1;
}

void Goanna::func_80020D24(s32 index, f32* out) {
    f32 scale = 1.0f / 127.0f;
    out[0] = this->rotations[index].x * scale;
    out[1] = this->rotations[index].y * scale;
    out[2] = this->rotations[index].z * scale;
    out[3] = this->rotations[index].w * scale;
}

void Goanna::func_80020DA4(s32 value) {
    D_80070B00 = value;
}

void Goanna::func_80020DB0() {
    u32 i;

    for (i = 0; i < this->numOffsets; i++) {
        this->offsets[i].y = -this->offsets[i].y;
    }

    for (i = 0; i < this->numRotations; i++) {
        this->rotations[i].y = -this->rotations[i].y;
        this->rotations[i].w = -this->rotations[i].w;
    }
}

void Goanna::func_80020E40() {
    if (this->offsets) {
        delete[] this->offsets;
        this->offsets = NULL;
    }
    if (this->rotations) {
        delete[] this->rotations;
        this->rotations = NULL;
    }
    if (this->times) {
        delete[] this->times;
        this->times = NULL;
    }
    this->numOffsets = 0;
    this->numRotations = 0;
    this->numTimes = 0;
}

Goanna::~Goanna() {
    this->func_80020E40();
}

Goanna::Goanna() {
    numOffsets = 0;
    offsets = NULL;
    numRotations = 0;
    rotations = NULL;
    numTimes = 0;
    times = NULL;
}

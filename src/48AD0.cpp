#include "common.h"

extern "C" {
#include <PR/os_cont.h>
#include <PR/os_pfs.h>
}

#pragma interface

class ControllerDevice;
class ControllerSystem;

// Parent class. Holds all data before the vptr (0x00..0x97) plus the virtual
// table; implementation lives in the 0x41000 TU. Declared here so the SN
// compiler places the vptr at offset 0x98 — see "vptr position" in the
// project skill notes (SN puts the vptr at the tail of the data members of
// the class that declares the virtuals).
class InputDeviceBase {
  public:
    /* 0x000 */ u8 pad_00[0x14];
    /* 0x014 */ s32 enabled;
    /* 0x018 */ u8 pad_18[0x08];
    /* 0x020 */ s32 hasAnalogHooks;
    /* 0x024 */ u8 pad_24[0x04];
    /* 0x028 */ u16* buttonEventTable;
    /* 0x02C */ u16* analogEventTable;
    /* 0x030 */ s32 unk30;
    /* 0x034 */ s32 unk34;
    /* 0x038 */ u8 pad_38[0x0C];
    /* 0x044 */ s32 unk44;
    /* 0x048 */ s32 unk48;
    /* 0x04C */ u8 pad_4C[0x04];
    /* 0x050 */ s32 unk50;
    /* 0x054 */ s32 unk54;
    /* 0x058 */ u8 pad_58[0x30];
    /* 0x088 */ s32 port;
    /* 0x08C */ void** eventCallbacks;
    /* 0x090 */ u8 pad_90[0x08];
    /* 0x098 */ // vptr placed here by SN

    InputDeviceBase();

    virtual void vfunc1();
    virtual void vfunc2(s32 code, s32 value, s32 deliver);
    virtual void vfunc3(s32 axis, f32 val) = 0;
    virtual ~InputDeviceBase();
    virtual s32 vfunc5();
    virtual s32 vfunc6(s32 deliver);
    virtual void vfunc7();
    virtual s32 vfunc8();
    virtual s32 vfunc9();
    virtual void vfunc10();
    virtual f32 vfunc11(s32 axis) = 0;
    virtual f32 vfunc12(s32 mask) = 0;
    virtual u8 vfunc13(s32 code) = 0;
    virtual s32 vfunc14() = 0;
    virtual s32 vfunc15() = 0;
    virtual s32 vfunc16() = 0;
    virtual s32 vfunc17() = 0;
    virtual void vfunc18();
    virtual void vfunc19(s32 dz) = 0;
    virtual void vfunc20();
    virtual void vfunc21() = 0;
    virtual void vfunc22() = 0;
    virtual void vfunc23() = 0;
    virtual void vfunc24() = 0;
    virtual void vfunc25() = 0;
    virtual void vfunc26() = 0;
    virtual void vfunc27() = 0;
    virtual s32 vfunc28() = 0;
    virtual void vfunc29() = 0;
    virtual s32 vfunc30() = 0;
    virtual s32 vfunc31() = 0;
    virtual s32 vfunc32() = 0;
    virtual void vfunc33();
    virtual void vfunc34();
    virtual s32 vfunc35() = 0;
    virtual s32 vfunc36() = 0;
    virtual s32 vfunc37() = 0;
    virtual s32 vfunc38();
};

// Concrete input device. 4 instances live in the ControllerSystem (477C0)
// task array at +0x4268, stride 0x150. Provides overrides for every pure
// virtual in InputDeviceBase and replaces several non-pure ones, including
// the dtor.
class ControllerDevice : public InputDeviceBase {
  public:
    /* 0x09C */ u16 prevButton;
    /* 0x09E */ s8 prevStickX;
    /* 0x09F */ s8 prevStickY;
    /* 0x0A0 */ s16 prevErrno;
    /* 0x0A2 */ u8 pad_A2[0x02];
    /* 0x0A4 */ OSContPad* pad;
    /* 0x0A8 */ u8 buttonStates[4];
    /* 0x0AC */ f32 sticks[2];
    /* 0x0B4 */ s32 deadzone;
    /* 0x0B8 */ s32 flagB8;
    /* 0x0BC */ s32 flagBC;
    /* 0x0C0 */ s32 pakPresent;
    /* 0x0C4 */ u8 pad_C4[0x20];
    /* 0x0E4 */ ControllerSystem* system;
    /* 0x0E8 */ OSPfs pfs;
    /* 0x150 */

    ControllerDevice();
    virtual void vfunc1();
    virtual void vfunc2(s32 code, s32 value, s32 deliver);
    virtual void vfunc3(s32 axis, f32 val);
    virtual ~ControllerDevice();
    virtual s32 vfunc5();
    virtual s32 vfunc6(s32 deliver);
    virtual s32 vfunc8();
    virtual s32 vfunc9();
    virtual f32 vfunc11(s32 axis);
    virtual f32 vfunc12(s32 mask);
    virtual u8 vfunc13(s32 code);
    virtual s32 vfunc14();
    virtual s32 vfunc15();
    virtual s32 vfunc16();
    virtual s32 vfunc17();
    virtual void vfunc19(s32 dz);
    virtual void vfunc21();
    virtual void vfunc22();
    virtual void vfunc23();
    virtual void vfunc24();
    virtual void vfunc25();
    virtual void vfunc26();
    virtual void vfunc27();
    virtual s32 vfunc28();
    virtual void vfunc29();
    virtual s32 vfunc30();
    virtual s32 vfunc31();
    virtual s32 vfunc32();
    virtual s32 vfunc35();
    virtual s32 vfunc36();
    virtual s32 vfunc37();
    virtual s32 vfunc38();
};

extern "C" void func_800410E0(ControllerDevice*, f32, s32, s32);
extern "C" void func_800418F8(ControllerDevice*, s32);
extern "C" void func_800416DC(ControllerDevice*, s32, s8, s32);

// Event listener target object held in self->eventCallbacks. Virtuals
// onPressed (slot 1, vtable +0x8/+0xC) and onReleased (slot 2, +0x10/+0x14).
class EventListener {
  public:
    virtual void onPressed(ControllerDevice* dev, s32 code, s32 userData) = 0;
    virtual void onReleased(ControllerDevice* dev, s32 code, s32 userData) = 0;
};

extern "C" void func_80047ED0(ControllerDevice*);
extern "C" s32 func_8004836C(ControllerDevice*, s32);
extern "C" s32 func_80047618(class ControllerSystem*, OSPfs*, s32);
extern "C" s32 func_800474F8(class ControllerSystem*, OSPfs*);
extern "C" s32 func_80047558(class ControllerSystem*, OSPfs*);
extern "C" s32 func_800475B8(class ControllerSystem*, OSPfs*);
extern "C" s32 func_80041A74(InputDeviceBase*);
extern "C" void func_80041ABC(InputDeviceBase*);
extern "C" void memset(void*, s32, s32);

#ifdef NON_MATCHING
extern "C" void func_80047ED0(ControllerDevice* self) {
    f32 nx, ny;
    s32 v;
    s32 stick;
    s32 dz;

    dz = self->deadzone;
    stick = self->pad->stick_x;
    if (dz < stick) {
        v = stick - dz;
    } else {
        v = stick + dz;
        if (stick >= -dz) {
            v = 0;
        }
    }
    nx = (f32)v / (75.0f - (f32)self->deadzone);
    if (nx > 1.0f) {
        nx = 1.0f;
    } else if (nx < -1.0f) {
        nx = -1.0f;
    }
    func_800410E0(self, nx, *(s32*)&self->sticks[0], 0x40000000);

    dz = self->deadzone;
    stick = self->pad->stick_y;
    if (dz < stick) {
        v = stick - dz;
    } else {
        v = stick + dz;
        if (stick >= -dz) {
            v = 0;
        }
    }
    ny = (f32)v / (75.0f - (f32)self->deadzone);
    if (ny > 1.0f) {
        ny = 1.0f;
    } else if (ny < -1.0f) {
        ny = -1.0f;
    }
    func_800410E0(self, ny, *(s32*)&self->sticks[1], 0x40000002);
}
#else
INCLUDE_ASM("asm/nonmatchings/48AD0", func_80047ED0);
#endif

#ifdef NON_MATCHING
extern "C" s32 func_80048024(ControllerDevice* self, s32 arg1) {
    s32 mask;
    s32 i;
    s32 v;
    s32 stick;
    s32 dz;
    f32 nx, ny;

    if (self->pad->errno != 0) {
        return (self->pad->errno & 8) != 0;
    }
    self->pad->stick_y = -self->pad->stick_y;
    if (self->eventCallbacks != NULL) {
        if (self->pad->button != self->prevButton) {
            i = 0;
            mask = 1;
            do {
                s32 curBit = self->pad->button & mask;
                if (curBit != (self->prevButton & mask)) {
                    self->vfunc2(i | 0x30000000, curBit != 0, 1);
                }
                i++;
                mask <<= 1;
            } while (i < 16);
        }
        if (self->hasAnalogHooks != 0) {
            func_80047ED0(self);
        }
    }

    dz = self->deadzone;
    stick = self->pad->stick_x;
    if (dz < stick) {
        v = stick - dz;
    } else {
        v = stick + dz;
        if (stick >= -dz) {
            v = 0;
        }
    }
    nx = (f32)v / (75.0f - (f32)self->deadzone);
    if (nx > 1.0f) {
        nx = 1.0f;
    } else if (nx < -1.0f) {
        nx = -1.0f;
    }
    self->sticks[0] = nx;

    dz = self->deadzone;
    stick = self->pad->stick_y;
    if (dz < stick) {
        v = stick - dz;
    } else {
        v = stick + dz;
        if (stick >= -dz) {
            v = 0;
        }
    }
    ny = (f32)v / (75.0f - (f32)self->deadzone);
    if (ny > 1.0f) {
        ny = 1.0f;
    } else if (ny < -1.0f) {
        ny = -1.0f;
    }
    self->sticks[1] = ny;

    *(u32*)&self->prevButton = *(u32*)self->pad;
    self->prevErrno = *(s16*)((u8*)self->pad + 4);
    func_800418F8(self, arg1);
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/48AD0", vfunc6__16ControllerDevicel);
#endif

void ControllerDevice::vfunc26() {
}

void ControllerDevice::vfunc25() {
}

void ControllerDevice::vfunc24() {
}

void ControllerDevice::vfunc23() {
}

void ControllerDevice::vfunc21() {
}

void ControllerDevice::vfunc22() {
}

s32 ControllerDevice::vfunc17() {
    return 0;
}

s32 ControllerDevice::vfunc16() {
    return 0;
}

s32 ControllerDevice::vfunc14() {
    return 0;
}

s32 ControllerDevice::vfunc15() {
    return 0;
}

f32 ControllerDevice::vfunc11(s32 axis) {
    return this->sticks[axis];
}

static inline s32 mapPfsRc(ControllerDevice* self, s32 rc) {
    switch (rc) {
        case 0:
            self->pakPresent = 1;
            return 0;
        case 1:
            self->pakPresent = 0;
            return 1;
        case 4:
            self->pakPresent = 0;
            return 3;
        default:
            self->pakPresent = 0;
            return 2;
    }
}

s32 ControllerDevice::vfunc38() {
    if (this->system == NULL && this->pakPresent != 0) {
        return 1;
    }
    func_80047618(this->system, &this->pfs, this->port);
    return mapPfsRc(this, func_800474F8(this->system, &this->pfs));
}

extern "C" s32 func_8004836C(ControllerDevice* self, s32 rc) {
    return mapPfsRc(self, rc);
}

s32 ControllerDevice::vfunc37() {
    if (this->system == NULL && this->pakPresent != 0) {
        return 1;
    }
    return func_8004836C(this, func_80047558(this->system, &this->pfs));
}

s32 ControllerDevice::vfunc36() {
    if (this->system == NULL && this->pakPresent != 0) {
        return 1;
    }
    return func_8004836C(this, func_800475B8(this->system, &this->pfs));
}

s32 ControllerDevice::vfunc35() {
    if (this->system == NULL) {
        return 1;
    }
    return func_8004836C(this, func_80047618(this->system, &this->pfs, this->port));
}

void ControllerDevice::vfunc3(s32 axis, f32 val) {
    s32 mask = 1;
    if (!this->enabled) {
        return;
    }
    for (s32 i = 0; i < 7; i++, mask <<= 1) {
        if (axis == mask) {
            this->sticks[i] = val;
        }
    }
}

#ifdef NON_MATCHING
void ControllerDevice::vfunc2(s32 code, s32 value, s32 deliver) {
    s8 state;
    s32 outCode;
    s32 codeSave;
    s32 idx;
    s32 bit;

    outCode = code & 0xF0000000;
    idx = code & 0xFFFF;
    state = value;
    bit = 1 << idx;
    codeSave = code;

    switch (outCode) {
        case 0x30000000:
            if (state != 0) {
                this->prevButton = this->prevButton | bit;
            } else {
                this->prevButton = this->prevButton & ~bit;
            }
            outCode |= this->buttonEventTable[idx];
            break;
        case 0x40000000:
            this->buttonStates[idx] = state;
            outCode |= this->analogEventTable[idx];
            break;
    }
    if (deliver != 0) {
        EventListener* cb = (EventListener*)this->eventCallbacks;
        if (cb != NULL) {
            if (state != 0) {
                cb->onPressed(this, outCode, this->unk30);
            } else {
                cb->onReleased(this, outCode, this->unk30);
            }
        }
    }
    func_800416DC(this, codeSave, state, deliver);
}
#else
INCLUDE_ASM("asm/nonmatchings/48AD0", vfunc2__16ControllerDevicelll);
#endif

f32 ControllerDevice::vfunc12(s32 mask) {
    s32 m = 1;
    for (s32 i = 0; i < 2; i++, m <<= 1) {
        if (mask == m) {
            return this->sticks[i];
        }
    }
    return 0.0f;
}

u8 ControllerDevice::vfunc13(s32 code) {
    switch (code & 0xF0000000) {
        case 0x30000000:
            return this->prevButton & (1 << code);
        case 0x40000000:
            return this->buttonStates[code & 0xFFFF];
        default:
            return 0;
    }
}

extern "C" void func_8004869C(ControllerDevice* self) {
    if (self->eventCallbacks != NULL) {
        if (self->pad->button != self->prevButton) {
            s32 i = 0;
            s32 mask = 1;
            do {
                s32 curBit = self->pad->button & mask;
                if (curBit != (self->prevButton & mask)) {
                    self->vfunc2(i | 0x30000000, curBit != 0, 1);
                }
                i++;
                mask <<= 1;
            } while (i < 16);
        }
        if (self->hasAnalogHooks != 0) {
            func_80047ED0(self);
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/48AD0", _vt.16ControllerDevice);

extern "C" f32 func_80048758(ControllerDevice* self, s8 raw) {
    s32 stick;
    s32 dz;

    stick = raw;
    dz = self->deadzone;
    if (dz >= stick) {
        if (stick < -dz) {
            stick += dz;
        } else {
            stick = 0;
        }
    } else {
        stick -= dz;
    }
    f32 r = (f32)stick / (75.0f - (f32)self->deadzone);
    if (r > 1.0f) {
        r = 1.0f;
    } else if (r < -1.0f) {
        r = -1.0f;
    }
    return r;
}

s32 ControllerDevice::vfunc5() {
    if (this->enabled) {
        return func_80041A74(this);
    }
    return 1;
}

extern "C" s32 func_8004880C(ControllerDevice* self, ControllerSystem* system, s32 port) {
    self->vfunc5();
    self->system = system;
    self->port = port;
    self->enabled = 1;
    return 1;
}

void ControllerDevice::vfunc1() {
    func_80041ABC(this);
    this->system = NULL;
    memset(&this->prevButton, 0, 6);
    memset(this->buttonStates, 0, 4);
    memset(this->sticks, 0, 8);
    this->unk44 = 0xE;
    this->unk48 = 2;
    this->unk34 = 3;
    this->vfunc19(0xB);
    this->flagBC = 0;
    this->flagB8 = 0;
    this->unk54 = 4;
    this->unk50 = 2;
    memset(&this->pfs, 0, 0x68);
    this->pakPresent = 0;
}

ControllerDevice::~ControllerDevice() {
    vfunc5();
}

ControllerDevice::ControllerDevice() {
    vfunc1();
}

extern "C" s32 func_800489A8(ControllerDevice* self) {
    return self->pakPresent;
}

extern "C" OSPfs* func_800489B4(ControllerDevice* self) {
    return &self->pfs;
}

extern "C" void func_800489BC(ControllerDevice* self, OSContPad* p) {
    self->pad = p;
}

s32 ControllerDevice::vfunc32() {
    return this->flagBC;
}

s32 ControllerDevice::vfunc31() {
    return this->flagB8;
}

s32 ControllerDevice::vfunc30() {
    this->flagBC = 0;
    return 1;
}

void ControllerDevice::vfunc29() {
    this->flagBC = 1;
}

s32 ControllerDevice::vfunc28() {
    this->flagB8 = 0;
    return 1;
}

void ControllerDevice::vfunc27() {
    this->flagB8 = 1;
}

void ControllerDevice::vfunc19(s32 dz) {
    this->deadzone = dz;
}

s32 ControllerDevice::vfunc9() {
    return 2;
}

s32 ControllerDevice::vfunc8() {
    return 0x10;
}

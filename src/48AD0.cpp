#include "common.h"
#include "input.h"

extern "C" s32 func_80047618(class ControllerSystem*, OSPfs*, s32);
extern "C" s32 func_800474F8(class ControllerSystem*, OSPfs*);
extern "C" s32 func_80047558(class ControllerSystem*, OSPfs*);
extern "C" s32 func_800475B8(class ControllerSystem*, OSPfs*);

#ifdef NON_MATCHING
void ControllerDevice::func_80047ED0() {
    f32 nx, ny;
    s32 v;
    s32 stick;
    s32 dz;

    dz = this->deadzone;
    stick = this->pad->stick_x;
    if (dz < stick) {
        v = stick - dz;
    } else {
        v = stick + dz;
        if (stick >= -dz) {
            v = 0;
        }
    }
    nx = (f32)v / (75.0f - (f32)this->deadzone);
    if (nx > 1.0f) {
        nx = 1.0f;
    } else if (nx < -1.0f) {
        nx = -1.0f;
    }
    this->func_800410E0(nx, *(s32*)&this->sticks[0], 0x40000000);

    dz = this->deadzone;
    stick = this->pad->stick_y;
    if (dz < stick) {
        v = stick - dz;
    } else {
        v = stick + dz;
        if (stick >= -dz) {
            v = 0;
        }
    }
    ny = (f32)v / (75.0f - (f32)this->deadzone);
    if (ny > 1.0f) {
        ny = 1.0f;
    } else if (ny < -1.0f) {
        ny = -1.0f;
    }
    this->func_800410E0(ny, *(s32*)&this->sticks[1], 0x40000002);
}
#else
INCLUDE_ASM("asm/nonmatchings/48AD0", func_80047ED0__16ControllerDevice);
#endif

#ifdef NON_MATCHING
s32 ControllerDevice::vfunc6(s32 arg1) {
    s32 mask;
    s32 i;
    s32 v;
    s32 stick;
    s32 dz;
    f32 nx, ny;

    if (this->pad->errno != 0) {
        return (this->pad->errno & 8) != 0;
    }
    this->pad->stick_y = -this->pad->stick_y;
    if (this->handler != NULL) {
        if (this->pad->button != this->prevButton) {
            i = 0;
            mask = 1;
            do {
                s32 curBit = this->pad->button & mask;
                if (curBit != (this->prevButton & mask)) {
                    this->onInput(i | 0x30000000, curBit != 0, 1);
                }
                i++;
                mask <<= 1;
            } while (i < 16);
        }
        if (this->hasAnalogHooks != 0) {
            this->func_80047ED0();
        }
    }

    dz = this->deadzone;
    stick = this->pad->stick_x;
    if (dz < stick) {
        v = stick - dz;
    } else {
        v = stick + dz;
        if (stick >= -dz) {
            v = 0;
        }
    }
    nx = (f32)v / (75.0f - (f32)this->deadzone);
    if (nx > 1.0f) {
        nx = 1.0f;
    } else if (nx < -1.0f) {
        nx = -1.0f;
    }
    this->sticks[0] = nx;

    dz = this->deadzone;
    stick = this->pad->stick_y;
    if (dz < stick) {
        v = stick - dz;
    } else {
        v = stick + dz;
        if (stick >= -dz) {
            v = 0;
        }
    }
    ny = (f32)v / (75.0f - (f32)this->deadzone);
    if (ny > 1.0f) {
        ny = 1.0f;
    } else if (ny < -1.0f) {
        ny = -1.0f;
    }
    this->sticks[1] = ny;

    *(u32*)&this->prevButton = *(u32*)this->pad;
    this->prevErrno = *(s16*)((u8*)this->pad + 4);
    InputDeviceBase::vfunc6(arg1);
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

f32 ControllerDevice::getAxisRaw(s32 axis) {
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

s32 ControllerDevice::pakCheck() {
    if (this->system == NULL && this->pakPresent != 0) {
        return 1;
    }
    func_80047618(this->system, &this->pfs, this->port);
    return mapPfsRc(this, func_800474F8(this->system, &this->pfs));
}

s32 ControllerDevice::func_8004836C(s32 rc) {
    return mapPfsRc(this, rc);
}

s32 ControllerDevice::vfunc37() {
    if (this->system == NULL && this->pakPresent != 0) {
        return 1;
    }
    return this->func_8004836C(func_80047558(this->system, &this->pfs));
}

s32 ControllerDevice::vfunc36() {
    if (this->system == NULL && this->pakPresent != 0) {
        return 1;
    }
    return this->func_8004836C(func_800475B8(this->system, &this->pfs));
}

s32 ControllerDevice::pakInit() {
    if (this->system == NULL) {
        return 1;
    }
    return this->func_8004836C(func_80047618(this->system, &this->pfs, this->port));
}

void ControllerDevice::onRepeat(s32 axis, f32 val) {
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
void ControllerDevice::onInput(s32 code, s32 value, s32 deliver) {
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
            outCode |= this->buttonMap[idx];
            break;
        case 0x40000000:
            this->buttonStates[idx] = state;
            outCode |= this->analogMap[idx];
            break;
    }
    if (deliver != 0) {
        EventListener* cb = (EventListener*)this->handler;
        if (cb != NULL) {
            if (state != 0) {
                cb->onPressed(this, outCode, this->timestamp);
            } else {
                cb->onReleased(this, outCode, this->timestamp);
            }
        }
    }
    InputDeviceBase::onInput(codeSave, state, deliver);
}
#else
INCLUDE_ASM("asm/nonmatchings/48AD0", onInput__16ControllerDevicelll);
#endif

f32 ControllerDevice::getAxisValue(s32 mask) {
    s32 m = 1;
    for (s32 i = 0; i < 2; i++, m <<= 1) {
        if (mask == m) {
            return this->sticks[i];
        }
    }
    return 0.0f;
}

u8 ControllerDevice::getButtonState(s32 code) {
    switch (code & 0xF0000000) {
        case 0x30000000:
            return this->prevButton & (1 << code);
        case 0x40000000:
            return this->buttonStates[code & 0xFFFF];
        default:
            return 0;
    }
}

void ControllerDevice::func_8004869C() {
    if (this->handler != NULL) {
        if (this->pad->button != this->prevButton) {
            s32 i = 0;
            s32 mask = 1;
            do {
                s32 curBit = this->pad->button & mask;
                if (curBit != (this->prevButton & mask)) {
                    this->onInput(i | 0x30000000, curBit != 0, 1);
                }
                i++;
                mask <<= 1;
            } while (i < 16);
        }
        if (this->hasAnalogHooks != 0) {
            this->func_80047ED0();
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/48AD0", _vt.16ControllerDevice);

f32 ControllerDevice::func_80048758(s8 raw) {
    s32 stick;
    s32 dz;

    stick = raw;
    dz = this->deadzone;
    if (dz >= stick) {
        if (stick < -dz) {
            stick += dz;
        } else {
            stick = 0;
        }
    } else {
        stick -= dz;
    }
    f32 r = (f32)stick / (75.0f - (f32)this->deadzone);
    if (r > 1.0f) {
        r = 1.0f;
    } else if (r < -1.0f) {
        r = -1.0f;
    }
    return r;
}

s32 ControllerDevice::disconnect() {
    if (this->enabled) {
        return InputDeviceBase::disconnect();
    }
    return 1;
}

s32 ControllerDevice::func_8004880C(ControllerSystem* system, s32 port) {
    this->disconnect();
    this->system = system;
    this->port = port;
    this->enabled = 1;
    return 1;
}

#if 0
// TODO: include when rodata order matches
void ControllerDevice::init() {
    InputDeviceBase::init();
    this->system = NULL;
    memset(&this->prevButton, 0, 6);
    memset(this->buttonStates, 0, 4);
    memset(this->sticks, 0, 8);
    this->unk44 = 0xE;
    this->unk48 = 2;
    this->repeatMask = 3;
    this->setDeadzone(0xB);
    this->flagBC = 0;
    this->flagB8 = 0;
    this->deviceType = 4;
    this->unk50 = 2;
    memset(&this->pfs, 0, 0x68);
    this->pakPresent = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/48AD0", init__16ControllerDevice);
#endif

ControllerDevice::~ControllerDevice() {
    disconnect();
}

ControllerDevice::ControllerDevice() {
    init();
}

s32 ControllerDevice::func_800489A8() {
    return this->pakPresent;
}

OSPfs* ControllerDevice::func_800489B4() {
    return &this->pfs;
}

void ControllerDevice::func_800489BC(OSContPad* p) {
    this->pad = p;
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

void ControllerDevice::setDeadzone(s32 dz) {
    this->deadzone = dz;
}

s32 ControllerDevice::getAxisCount() {
    return 2;
}

s32 ControllerDevice::getButtonCount() {
    return 0x10;
}

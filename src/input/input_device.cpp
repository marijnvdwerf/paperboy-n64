#include "common.h"
#include "input/input_device.h"
#include "gol_string_table.h"

extern "C" char* strncpy(char*, const char*, unsigned);
extern "C" f32 atanf(f32);
extern "C" void __builtin_delete(void*);
extern "C" void func_80042804(void*, s32, void*);
extern "C" void func_8004278C(void*, s32, void*);

extern u16 D_80076180[256];

static inline const u16* emptyEntry() {
    static const u16 entry[4] = { 0 };
    return entry;
}

INCLUDE_RODATA("asm/nonmatchings/input/input_device", _vt.11InputDevice);

void InputDevice::func_800410E0(f32 val, f32 prev, u32 id) {
    s32 pressed = -1;
    s32 released = pressed;
    u32 code = id;
    id &= 0xFFFF;
    if (val > 0.0f) {
        if (prev <= 0.0f)
            pressed = id;
        if (this->getButtonState(code + 1)) {
            released = id + 1;
        }
    } else if (val < 0.0f) {
        if (prev >= 0.0f)
            pressed = id + 1;
        if (this->getButtonState(code)) {
            released = id;
        }
    } else {
        if (this->getAxisRaw(id >> 1) > 0.0f) {
            released = id;
        } else if (this->getAxisRaw(id >> 1) < 0.0f) {
            released = id + 1;
        }
    }
    if (pressed >= 0) {
        this->onInput(pressed | 0x40000000, 1, 1);
    }
    if (released >= 0) {
        this->onInput(released | 0x40000000, 0, 1);
    }
}

void InputDevice::func_80041270(s32 delta) {
    u32 prefix = 0;
    this->repeatTimer -= delta;
    if (!this->repeatEnabled)
        return;
    if (this->repeatTimer > 0)
        return;
    if (!this->handler)
        return;

    switch (this->deviceType) {
        case 4:
            prefix = 0x30000000;
            break;
        case 3:
            prefix = 0x10000000;
            break;
        case 2:
            prefix = 0x20000000;
            break;
    }
    s32 repeated = 0;
    s32 i = repeated;
    while (1) {
        if (i >= this->getButtonCount())
            break;
        if (this->getButtonState(prefix | i)) {
            repeated++;
            this->handler->onRepeat(this, prefix | this->buttonMap[i], this->timestamp);
        }
        i++;
    }
    if (this->hasAnalogHooks) {
        s32 j = 0;
        while (1) {
            if (j >= this->getAxisCount() * 2)
                break;
            if (this->getButtonState(j | 0x40000000)) {
                repeated++;
                this->handler->onRepeat(this, this->analogMap[j] | 0x40000000, this->timestamp);
            }
            j++;
        }
    }
    if (repeated) {
        this->repeatTimer = this->repeatRate;
    } else {
        this->repeatTimer = this->repeatDelay;
    }
    for (s32 k = 0; k < this->listenerCount; k++) {
        func_8004278C(this->listeners[k], this->timestamp, this);
    }
}

s32 InputDevice::pakCheck() {
    return this->vfunc37();
}

s32 InputDevice::disable() {
    this->active = 0;
    return 0;
}

s32 InputDevice::enable() {
    this->active = 1;
    return 1;
}

s32 InputDevice::isActive() {
    return this->active;
}

void InputDevice::poll() {
}

s32 InputDevice::vfunc18() {
    return 0;
}

s32 InputDevice::getAxisCount() {
    return 0x10;
}

s32 InputDevice::getButtonCount() {
    return 0x100;
}

s32 InputDevice::func_800414E4(void* listener) {
    s32 i = 0;
    while (i < this->listenerCount) {
        if (this->listeners[i] == listener) {
            while (i < 4) {
                this->listeners[i] = this->listeners[i + 1];
                i++;
            }
            this->listenerCount--;
            return 1;
        }
        i++;
    }
    return 0;
}

s32 InputDevice::func_80041564(void* listener) {
    this->listeners[this->listenerCount++] = listener;
    return this->listenerCount;
}

s32 InputDevice::func_80041580(s32 axis1, s32 axis2) {
    f32 x = this->getAxisValue(axis1);
    f32 y = this->getAxisValue(axis2);
    if (axis1 == 2) {
        x = -x;
    } else if (axis2 == 2) {
        y = -y;
    }
    if (x == 0.0f) {
        if (y == 0.0f)
            return 0;
        if (y > 0.0f)
            return 3;
        return 7;
    }
    if (y == 0.0f) {
        if (x > 0.0f)
            return 1;
        return 5;
    }
    f32 angle = atanf(y / x);
    if (x < 0.0f) {
        angle = angle + 3.14159265f;
    }
    if (angle < 0.0f) {
        angle = angle + 6.28318530f;
    }
    angle = angle + 0.3926999867f;
    angle = angle / 0.78539816f;
    return (s32)angle + 1;
}

void InputDevice::onInput(s32 a1, s32 value, s32 deliver) {
    s8 save = value;
    if (save) {
        this->repeatTimer = this->repeatDelay;
    }
}

void InputDevice::func_800416F8() {
    s32 mask = 1;
    s32 buttons = this->repeatMask;
    s32 i = 0;
    while (i < 0x10) {
        if (mask & buttons) {
            this->onRepeat(mask, 0);
        }
        i++;
        mask <<= 1;
    }
}

const u16* InputDevice::vfunc10(s32 code) {
    u16 index = code;
    GolStringTable* device;
    code &= 0xF0000000;
    u32 type = code;
    switch (type) {
        case 0x10000000:
        case 0x20000000:
        case 0x30000000:
            device = this->buttonDevice;
            break;
        case 0x40000000:
            device = this->analogDevice;
            break;
        default:
            return emptyEntry();
    }
    if (device && index < device->count) {
        return device->getEntry(index);
    }
    return emptyEntry();
}

u16* InputDevice::func_80041814() {
    if (this->analogMap == D_80076180)
        return 0;
    return this->analogMap;
}

u16* InputDevice::func_80041834() {
    if (this->buttonMap == D_80076180)
        return 0;
    return this->buttonMap;
}

void InputDevice::func_80041854(u16* a1, u16* a2) {
    this->buttonMap = a1;
    if (!a1) {
        this->buttonMap = D_80076180;
    }
    this->analogMap = a2;
    if (!a2) {
        this->analogMap = D_80076180;
    }
}

void InputDevice::func_80041884(const char* n) {
    strncpy(this->name, n, 0x1F);
    this->name[0x1F] = 0;
}

void InputDevice::func_800418B4(s32 rate, s32 delay) {
    if (rate) {
        this->repeatDelay = delay;
        this->repeatRate = rate;
        this->repeatEnabled = 1;
        this->repeatTimer = this->repeatDelay;
    } else {
        this->repeatEnabled = 0;
    }
}

void InputDevice::func_800418DC(s32 rate, s32 delay) {
    this->repeatDelay = delay;
    this->repeatRate = rate;
    this->repeatEnabled = 1;
    this->repeatTimer = this->repeatDelay;
}

s32 InputDevice::vfunc6(s32 delta) {
    s32 i = 0;
    this->timestamp += delta;
    while (i < this->listenerCount) {
        func_80042804(this->listeners[i], this->timestamp, this);
        i++;
    }
    this->func_80041270(delta);
    return 0;
}

void InputDevice::func_80041988() {
    u32 prefix = 0;
    switch (this->deviceType) {
        case 4:
            prefix = 0x30000000;
            break;
        case 3:
            prefix = 0x10000000;
            break;
        case 2:
            prefix = 0x20000000;
            break;
    }
    s32 i = 0;
    while (1) {
        if (i >= this->getButtonCount())
            break;
        if (this->getButtonState(prefix | i)) {
            this->onInput(prefix | i, 0, 1);
        }
        i++;
    }
}

s32 InputDevice::disconnect() {
    if (this->enabled == 0) {
        return 1;
    }
    this->init();
    return this->enabled < 1;
}

void InputDevice::init() {
    this->dirty = 0;
    this->hasAnalogHooks = 0;
    this->repeatEnabled = 0;
    this->active = 0;
    this->enabled = 0;
    this->analogMap = D_80076180;
    this->buttonMap = D_80076180;
    memset(this, 0, 0x10);
    this->unk10 = 0;
    this->buttonDevice = 0;
    this->analogDevice = 0;
    this->unk44 = 0;
    this->unk48 = 0;
    this->listenerCount = 0;
    this->deviceType = 0;
    this->unk50 = 0;
    this->repeatMask = 0;
    this->port = 0;
    this->timestamp = 0;
    this->repeatRate = 0;
    this->repeatTimer = 0;
    this->name[0] = 0;
    this->handler = 0;
}

InputDevice::~InputDevice() {
    disconnect();
}

InputDevice::InputDevice() {
    u16 i = 0;
    do {
        D_80076180[i] = i;
        i++;
    } while (i < 256);
    init();
}

s32 InputDevice::func_80041C00() {
    return this->dirty;
}

s32 InputDevice::func_80041C0C() {
    this->dirty = 1;
    return 1;
}

void InputDevice::func_80041C18(s32 a1) {
    func_800418DC(a1, a1);
}

void InputDevice::func_80041C34() {
    this->repeatEnabled = 0;
}

void InputDevice::func_80041C3C() {
    this->hasAnalogHooks = 1;
}

void InputDevice::func_80041C48() {
    this->hasAnalogHooks = 0;
}

void InputDevice::func_80041C50(GolStringTable* val) {
    this->analogDevice = val;
}

void InputDevice::func_80041C58(GolStringTable* val) {
    this->buttonDevice = val;
}

void InputDevice::func_80041C60(s32 a1) {
    func_800418B4(a1, this->repeatDelay);
}

void InputDevice::func_80041C80(s32 val) {
    this->unk50 = val;
}

void InputDevice::func_80041C88(s32 val) {
    this->deviceType = val;
}

void InputDevice::func_80041C90(InputDevice::Callback* val) {
    this->handler = val;
}

s32 InputDevice::func_80041C98() {
    return this->unk10;
}

s32 InputDevice::func_80041CA4() {
    return this->port;
}

s32 InputDevice::func_80041CB0() {
    return this->unk50;
}

s32 InputDevice::func_80041CBC() {
    return this->deviceType;
}

s32 InputDevice::func_80041CC8() {
    return this->repeatMask;
}

char* InputDevice::func_80041CD4() {
    return this->name;
}

s32 InputDevice::func_80041CDC() {
    return this->unk48;
}

s32 InputDevice::func_80041CE8() {
    return this->unk44;
}

s32 InputDevice::func_80041CF4() {
    return this->repeatRate;
}

InputDevice::Callback* InputDevice::func_80041D00() {
    return this->handler;
}

u32 InputDevice::func_80041D0C() {
    return (u32)this->listeners[1] & 0xF0000000;
}

extern "C" u32 func_80041D1C(u32 id) {
    return id & 0xF0000000;
}

s32 InputDevice::func_80041D28() {
    return this->hasAnalogHooks;
}

u32 InputDevice::func_80041D34() {
    return this->enabled;
}

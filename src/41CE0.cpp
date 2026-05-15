#include "common.h"
#include "input.h"

extern "C" char* strncpy(char*, const char*, unsigned);
extern "C" f32 atanf(f32);
extern "C" void __builtin_delete(void*);
extern "C" void func_80042804(void*, s32, void*);
extern "C" void func_8004278C(void*, s32, void*);
extern "C" void* func_80045C9C(void*, u16);

extern u16 D_80076180[256];
extern char D_80004490[];

INCLUDE_RODATA("asm/nonmatchings/41CE0", D_80004490);

INCLUDE_RODATA("asm/nonmatchings/41CE0", _vt.15InputDeviceBase);

// reg-alloc: s0/s1/s2 assignment, a1 pre-computation before branch
#ifdef NON_MATCHING
void InputDeviceBase::func_800410E0(f32 val, f32 prev, u16 id) {
    s32 pressed = -1;
    s32 released = pressed;
    s32 half = id >> 1;
    if (val > 0.0f) {
        if (prev <= 0.0f)
            pressed = id;
        if (this->getButtonState(id + 1)) {
            released = id + 1;
        }
    } else if (val < 0.0f) {
        if (prev >= 0.0f)
            pressed = id + 1;
        if (this->getButtonState(id)) {
            released = id;
        }
    } else {
        if (this->getAxisRaw(half) > 0.0f) {
            released = id;
        } else if (this->getAxisRaw(half) < 0.0f) {
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
#else
INCLUDE_ASM("asm/nonmatchings/41CE0", func_800410E0__15InputDeviceBaseffUs);
#endif

// complex update function with nested loops and vcalls
#ifdef NON_MATCHING
s32 InputDeviceBase::func_80041270(s32 delta) {
    s32 s1 = 0;
    u32 prefix = 0;
    this->repeatTimer -= delta;
    if (!this->repeatEnabled)
        return;
    if (this->repeatTimer > 0)
        return;
    if (!this->handler)
        return;

    switch (this->deviceType) {
        case 3:
            prefix = 0x10000000;
            break;
        case 2:
            prefix = 0x20000000;
            break;
        case 4:
            prefix = 0x30000000;
            break;
    }
    s32 i = 0;
    while (1) {
        s32 count = this->getButtonCount();
        if (i >= count)
            break;
        u32 id = prefix | i;
        if (this->getButtonState(0)) {
            s1++;
            u16 mapped = this->buttonMap[i];
            void** h = (void**)this->handler;
            void* obj = *h;
            // vcall through handler's vtable slot 3
        }
        i++;
    }
    if (this->hasAnalogHooks) {
        s32 j = 0;
        while (1) {
            s32 count2 = this->getAxisCount() * 2;
            if (j >= count2)
                break;
            u32 id2 = j | 0x40000000;
            if (this->getButtonState(0)) {
                s1++;
                u16 mapped2 = this->analogMap[j];
                void** h2 = (void**)this->handler;
                void* obj2 = *h2;
                // vcall through handler's vtable slot 3
            }
            j++;
        }
    }
    if (s1) {
        this->repeatTimer = this->repeatRate;
    } else {
        this->repeatTimer = this->repeatDelay;
    }
    if (this->listenerCount > 0) {
        void** p = this->listeners;
        s32 k = 0;
        do {
            func_8004278C(*p, this->timestamp, this);
            p++;
            k++;
        } while (k < this->listenerCount);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/41CE0", func_80041270__15InputDeviceBasel);
#endif

s32 InputDeviceBase::pakCheck() {
    return this->vfunc37();
}

s32 InputDeviceBase::disable() {
    this->active = 0;
    return 0;
}

s32 InputDeviceBase::enable() {
    this->active = 1;
    return 1;
}

s32 InputDeviceBase::isActive() {
    return this->active;
}

void InputDeviceBase::poll() {
}

s32 InputDeviceBase::vfunc18() {
    return 0;
}

s32 InputDeviceBase::getAxisCount() {
    return 0x10;
}

s32 InputDeviceBase::getButtonCount() {
    return 0x100;
}

s32 InputDeviceBase::func_800414E4(void* listener) {
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

// reg-alloc diff: v0/v1 swapped, addu reordered
#ifdef NON_MATCHING
void InputDeviceBase::func_80041564(void* listener) {
    s32 n = this->listenerCount;
    this->listeners[n] = listener;
    this->listenerCount = n + 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/41CE0", func_80041564__15InputDeviceBasePv);
#endif

// reg-alloc diff: s2/ra save ordering, move s2,a2 placement
#ifdef NON_MATCHING
s32 InputDeviceBase::func_80041580(s32 axis1, s32 axis2) {
    f32 x = this->getAxisValue(0);
    f32 y = this->getAxisValue(0);
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
    f32 a2 = angle;
    if (x < 0.0f) {
        a2 = a2 + 3.14159265f;
    }
    if (a2 < 0.0f) {
        a2 = a2 + 6.28318530f;
    }
    a2 = a2 + 0.39269908f;
    a2 = a2 / 0.78539816f;
    return (s32)a2 + 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/41CE0", func_80041580__15InputDeviceBasell);
#endif

void InputDeviceBase::onInput(s32 a1, s32 value, s32 deliver) {
    s8 save = value;
    if (save) {
        this->repeatTimer = this->repeatDelay;
    }
}

void InputDeviceBase::func_800416F8() {
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

INCLUDE_ASM("asm/nonmatchings/41CE0", vfunc10__15InputDeviceBase);

u16* InputDeviceBase::func_80041814() {
    u16* p = this->analogMap;
    if (p == D_80076180)
        return 0;
    return p;
}

u16* InputDeviceBase::func_80041834() {
    u16* p = this->buttonMap;
    if (p == D_80076180)
        return 0;
    return p;
}

void InputDeviceBase::func_80041854(u16* a1, u16* a2) {
    this->buttonMap = a1;
    if (!a1) {
        this->buttonMap = D_80076180;
    }
    this->analogMap = a2;
    if (!a2) {
        this->analogMap = D_80076180;
    }
}

void InputDeviceBase::func_80041884(const char* n) {
    strncpy(this->name, n, 0x1F);
    this->name[0x1F] = 0;
}

// reg-alloc diff: target caches delay in v1 for delay-slot store
#ifdef NON_MATCHING
void InputDeviceBase::func_800418B4(s32 rate, s32 delay) {
    if (rate) {
        this->repeatDelay = delay;
        this->repeatRate = rate;
        this->repeatEnabled = 1;
        this->repeatTimer = delay;
    } else {
        this->repeatEnabled = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/41CE0", func_800418B4__15InputDeviceBasell);
#endif

// reg-alloc diff: target caches delay in v1 for delay-slot store
#ifdef NON_MATCHING
void InputDeviceBase::func_800418DC(s32 rate, s32 delay) {
    this->repeatDelay = delay;
    this->repeatRate = rate;
    this->repeatEnabled = 1;
    this->repeatTimer = delay;
}
#else
INCLUDE_ASM("asm/nonmatchings/41CE0", func_800418DC__15InputDeviceBasell);
#endif

// reg-alloc diff: s0/s1/s2 assignment swapped
#ifdef NON_MATCHING
s32 InputDeviceBase::vfunc6(s32 delta) {
    s32 i = 0;
    this->timestamp += delta;
    if (this->listenerCount > 0) {
        void** p = this->listeners;
        do {
            func_80042804(*p, this->timestamp, this);
            p++;
            i++;
        } while (i < this->listenerCount);
    }
    this->func_80041270(delta);
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/41CE0", vfunc6__15InputDeviceBasel);
#endif

// switch codegen diff: beql vs beq for case 2, s1/s3 init ordering
#ifdef NON_MATCHING
void InputDeviceBase::func_80041988() {
    u32 prefix = 0;
    s32 i = 0;
    switch (this->deviceType) {
        case 3:
            prefix = 0x10000000;
            break;
        case 2:
            prefix = 0x20000000;
            break;
        case 4:
            prefix = 0x30000000;
            break;
    }
    while (1) {
        s32 count = this->getButtonCount();
        if (i >= count)
            break;
        s32 id = prefix | i;
        if (this->getButtonState(id)) {
            this->onInput(id, 0, 1);
        }
        i++;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/41CE0", func_80041988__15InputDeviceBase);
#endif

s32 InputDeviceBase::disconnect() {
    if (this->enabled == 0) {
        return 1;
    }
    this->init();
    return this->enabled < 1;
}

#if 0
// TODO: pending rodata reorganisation
void InputDeviceBase::init() {
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
#else
INCLUDE_ASM("asm/nonmatchings/41CE0", init__15InputDeviceBase);
#endif

InputDeviceBase::~InputDeviceBase() {
    disconnect();
}

InputDeviceBase::InputDeviceBase() {
    u16 i = 0;
    do {
        D_80076180[i] = i;
        i++;
    } while (i < 256);
    init();
}

s32 InputDeviceBase::func_80041C00() {
    return this->dirty;
}

s32 InputDeviceBase::func_80041C0C() {
    this->dirty = 1;
    return 1;
}

void InputDeviceBase::func_80041C18(s32 a1) {
    func_800418DC(a1, a1);
}

void InputDeviceBase::func_80041C34() {
    this->repeatEnabled = 0;
}

void InputDeviceBase::func_80041C3C() {
    this->hasAnalogHooks = 1;
}

void InputDeviceBase::func_80041C48() {
    this->hasAnalogHooks = 0;
}

void InputDeviceBase::func_80041C50(void* val) {
    this->analogDevice = val;
}

void InputDeviceBase::func_80041C58(void* val) {
    this->buttonDevice = val;
}

void InputDeviceBase::func_80041C60(s32 a1) {
    func_800418B4(a1, this->repeatDelay);
}

void InputDeviceBase::func_80041C80(s32 val) {
    this->unk50 = val;
}

void InputDeviceBase::func_80041C88(s32 val) {
    this->deviceType = val;
}

void InputDeviceBase::func_80041C90(void* val) {
    this->handler = val;
}

s32 InputDeviceBase::func_80041C98() {
    return this->unk10;
}

s32 InputDeviceBase::func_80041CA4() {
    return this->port;
}

s32 InputDeviceBase::func_80041CB0() {
    return this->unk50;
}

s32 InputDeviceBase::func_80041CBC() {
    return this->deviceType;
}

s32 InputDeviceBase::func_80041CC8() {
    return this->repeatMask;
}

char* InputDeviceBase::func_80041CD4() {
    return this->name;
}

s32 InputDeviceBase::func_80041CDC() {
    return this->unk48;
}

s32 InputDeviceBase::func_80041CE8() {
    return this->unk44;
}

s32 InputDeviceBase::func_80041CF4() {
    return this->repeatRate;
}

void* InputDeviceBase::func_80041D00() {
    return this->handler;
}

u32 InputDeviceBase::func_80041D0C() {
    return (u32)this->listeners[1] & 0xF0000000;
}

extern "C" u32 func_80041D1C(u32 id) {
    return id & 0xF0000000;
}

s32 InputDeviceBase::func_80041D28() {
    return this->hasAnalogHooks;
}

u32 InputDeviceBase::func_80041D34() {
    return this->enabled;
}

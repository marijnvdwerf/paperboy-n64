#ifndef INPUT_H
#define INPUT_H

#include "common.h"

extern "C" {
#include <PR/os_cont.h>
#include <PR/os_pfs.h>
}

class ControllerDevice;
class ControllerSystem;

class InputDeviceBase {
  public:
    /* 0x00 */ void* listeners[4];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u32 enabled;
    /* 0x18 */ s32 active;
    /* 0x1C */ s32 repeatEnabled;
    /* 0x20 */ s32 hasAnalogHooks;
    /* 0x24 */ s32 dirty;
    /* 0x28 */ u16* buttonMap;
    /* 0x2C */ u16* analogMap;
    /* 0x30 */ s32 timestamp;
    /* 0x34 */ s32 repeatMask;
    /* 0x38 */ s32 repeatDelay;
    /* 0x3C */ s32 repeatRate;
    /* 0x40 */ s32 repeatTimer;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 listenerCount;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 deviceType;
    /* 0x58 */ char name[0x20];
    /* 0x78 */ u8 pad78[0x10];
    /* 0x88 */ s32 port;
    /* 0x8C */ void* handler;
    /* 0x90 */ void* buttonDevice;
    /* 0x94 */ void* analogDevice;
    /* 0x98 */ // vtable

    InputDeviceBase();
    virtual void init();
    virtual void onInput(s32 code, s32 value, s32 deliver);
    virtual void onRepeat(s32 button, s32 zero) = 0;
    virtual ~InputDeviceBase();
    virtual s32 disconnect();
    virtual s32 vfunc6(s32 deliver);
    virtual s32 isActive();
    virtual s32 getButtonCount();
    virtual s32 getAxisCount();
    virtual void* vfunc10();
    virtual f32 getAxisRaw(s32 axis) = 0;
    virtual f32 getAxisValue(s32 mask) = 0;
    virtual u8 getButtonState(s32 code) = 0;
    virtual s32 vfunc14() = 0;
    virtual s32 vfunc15() = 0;
    virtual s32 vfunc16() = 0;
    virtual s32 vfunc17() = 0;
    virtual s32 vfunc18();
    virtual void setDeadzone(s32 dz) = 0;
    virtual void poll();
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
    virtual s32 enable();
    virtual s32 disable();
    virtual s32 pakInit() = 0;
    virtual s32 vfunc36() = 0;
    virtual s32 vfunc37() = 0;
    virtual s32 pakCheck();

    void func_800410E0(f32 val, f32 prev, u16 id);
    s32 func_80041270(s32 delta);
    s32 func_800414E4(void* listener);
    void func_80041564(void* listener);
    s32 func_80041580(s32 axis1, s32 axis2);
    void func_800416F8();
    u16* func_80041814();
    u16* func_80041834();
    void func_80041854(u16* a1, u16* a2);
    void func_80041884(const char* name);
    void func_800418B4(s32 rate, s32 delay);
    void func_800418DC(s32 rate, s32 delay);
    void func_80041988();
    s32 func_80041C00();
    s32 func_80041C0C();
    void func_80041C18(s32 a1);
    void func_80041C34();
    void func_80041C3C();
    void func_80041C48();
    void func_80041C50(void* val);
    void func_80041C58(void* val);
    void func_80041C60(s32 a1);
    void func_80041C80(s32 val);
    void func_80041C88(s32 val);
    void func_80041C90(void* val);
    s32 func_80041C98();
    s32 func_80041CA4();
    s32 func_80041CB0();
    s32 func_80041CBC();
    s32 func_80041CC8();
    char* func_80041CD4();
    s32 func_80041CDC();
    s32 func_80041CE8();
    s32 func_80041CF4();
    void* func_80041D00();
    u32 func_80041D0C();
    s32 func_80041D28();
    u32 func_80041D34();
};

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
    virtual void init();
    virtual void onInput(s32 code, s32 value, s32 deliver);
    virtual void onRepeat(s32 axis, f32 val);
    virtual ~ControllerDevice();
    virtual s32 disconnect();
    virtual s32 vfunc6(s32 deliver);
    virtual s32 getButtonCount();
    virtual s32 getAxisCount();
    virtual f32 getAxisRaw(s32 axis);
    virtual f32 getAxisValue(s32 mask);
    virtual u8 getButtonState(s32 code);
    virtual s32 vfunc14();
    virtual s32 vfunc15();
    virtual s32 vfunc16();
    virtual s32 vfunc17();
    virtual void setDeadzone(s32 dz);
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
    virtual s32 pakInit();
    virtual s32 vfunc36();
    virtual s32 vfunc37();
    virtual s32 pakCheck();

    void func_80047ED0();
    s32 func_8004836C(s32 rc);
    void func_8004869C();
    f32 func_80048758(s8 raw);
    s32 func_8004880C(ControllerSystem* system, s32 port);
    s32 func_800489A8();
    OSPfs* func_800489B4();
    void func_800489BC(OSContPad* p);
};

class EventListener {
  public:
    virtual void onPressed(ControllerDevice* dev, s32 code, s32 userData) = 0;
    virtual void onReleased(ControllerDevice* dev, s32 code, s32 userData) = 0;
};

#endif

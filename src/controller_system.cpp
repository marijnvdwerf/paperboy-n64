#include "controller_system.h"

void ControllerSystem::func_80041D40() {
    for (u32 i = 0; i < 4; i++) {
        if (this->active[i]) {
            this->active[i] = (this->entries[i]->enable() == 0);
        }
    }
    if (this->unk10 && this->unk38) {
        this->unk10 = (this->unk38->enable() == 0);
    }
    if (this->unk14 && this->unk3C) {
        this->unk14 = (this->unk3C->enable() == 0);
    }
}

void ControllerSystem::func_80041E1C() {
    for (u32 i = 0; i < 4; i++) {
        if (this->entries[i] && this->entries[i]->isActive()) {
            this->active[i] = 1;
            this->entries[i]->disable();
        }
    }
    if (this->unk38 && this->unk38->isActive()) {
        this->unk10 = 1;
        this->unk38->disable();
    }
    if (this->unk3C && this->unk3C->isActive()) {
        this->unk14 = 1;
        this->unk3C->disable();
    }
}

ControllerDevice* ControllerSystem::findOrDefault(s32 port) {
    if (this->unk38 && this->unk38->port == port)
        return this->unk38;
    if (this->unk3C && this->unk3C->port == port)
        return this->unk3C;
    return this->findByPort(port);
}

ControllerDevice* ControllerSystem::findByPort(s32 port) {
    s32 i = 0;
    while (i < this->count) {
        ControllerDevice* d = this->entries[i];
        if (d->port == port) {
            return d;
        }
        i++;
    }
    return NULL;
}

void ControllerSystem::func_80041FD4() {
    s32 i = 0;
    while (1) {
        if (i >= this->count)
            break;
        this->entries[i]->hasAnalogHooks = 0;
        i++;
    }
}

void ControllerSystem::func_80042004() {
    s32 i = 0;
    while (1) {
        if (i >= this->count)
            break;
        this->entries[i]->hasAnalogHooks = 1;
        i++;
    }
}

void ControllerSystem::init() {
    this->unk28 = 0;
    this->count = 0;
    this->unk30 = 0;
    this->unk2C = 0;
    this->unk3C = NULL;
    this->unk38 = NULL;
    memset(this->entries, 0, sizeof(this->entries));
    memset(this->active, 0, sizeof(this->active));
    memset(this->unk18, 0, sizeof(this->unk18));
    this->unk14 = 0;
    this->unk10 = 0;
}

ControllerSystem::~ControllerSystem() {
}

ControllerSystem::ControllerSystem() {
    init();
}

void ControllerSystem::func_8004210C(s32 i, s32 val) {
    this->unk18[i] = val;
}

ControllerDevice* ControllerSystem::func_8004211C(s32 i) {
    return this->entries[i];
}

ControllerDevice* ControllerSystem::func_80042130() {
    return this->unk3C;
}

ControllerDevice* ControllerSystem::func_8004213C() {
    return this->unk38;
}

s32 ControllerSystem::func_80042148(s32 i) {
    return this->unk18[i];
}

s32 ControllerSystem::func_8004215C() {
    return this->count;
}

s32 ControllerSystem::func_80042168() {
    return this->unk30;
}

s32 ControllerSystem::func_80042174() {
    return this->unk2C;
}

s32 ControllerSystem::func_80042180() {
    return this->unk28;
}

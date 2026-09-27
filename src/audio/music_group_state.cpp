#include "common.h"
#include "structs.h"

MusicGroupState::~MusicGroupState() {
}

MusicGroupState::MusicGroupState() {
    this->unk0 = 0;
}

u32 MusicGroupState::func_8003CA28() {
    return this->unk0;
}

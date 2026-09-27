#ifndef CMB_MODEL_PART_DATA_H
#define CMB_MODEL_PART_DATA_H

#include "common.h"

class GolFileParser;

class CmbModelPartData {
  public:
    /* 0x00 */ f32 speed;
    /* 0x04 */ f32 vec1[3];
    /* 0x10 */ f32 vec2[4];
    /* 0x20 */ s32 field20;
    /* 0x24 */ u16 field24;
    /* 0x26 */ u16 field26;
    /* 0x28 */ f32 pos[3];

    CmbModelPartData();

    void read(GolFileParser* parser);
    void setPos(f32* src);
    void getPos(f32* dst);
    f32 wrapValue(f32 value);
};

#endif

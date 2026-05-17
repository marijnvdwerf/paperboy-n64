#include "common.h"
#include "parrot.h"
#include "sandpiper.h"

void Sandpiper::read(Parrot* parrot) {
    parrot->expectToken(TOKEN_OPEN_BRACE);

    while (1) {
        s32 token = parrot->nextToken();
        if (token == TOKEN_CLOSE_BRACE) {
            return;
        }
        switch (token) {
            case 0x2B:
                field20 = parrot->readInt();
                break;
            case 0x2D: {
                s32 t = parrot->nextToken();
                if (t != TOKEN_INT) {
                    parrot->parseError(TOKEN_INT);
                }
                s32 v = parrot->intValue;
                field24 = v;
                break;
            }
            case 0x2E: {
                s32 t = parrot->nextToken();
                if (t != TOKEN_INT) {
                    parrot->parseError(TOKEN_INT);
                }
                s32 v = parrot->intValue;
                field26 = v;
                break;
            }
            case 0x2F:
                speed = parrot->readInt() * 0.001f;
                break;
            case 0x30:
                vec1[0] = parrot->readFloat();
                vec1[1] = parrot->readFloat();
                vec1[2] = parrot->readFloat();
                break;
            case 0x31:
                vec2[0] = parrot->readFloat();
                vec2[1] = parrot->readFloat();
                vec2[2] = parrot->readFloat();
                vec2[3] = parrot->readFloat();
                break;
            case 0x32:
                pos[0] = parrot->readFloat();
                pos[1] = parrot->readFloat();
                pos[2] = parrot->readFloat();
                break;
            default:
                parrot->parseError(0);
                break;
        }
    }
}

void Sandpiper::setPos(f32* src) {
    pos[0] = src[0];
    pos[1] = src[1];
    pos[2] = src[2];
}

void Sandpiper::getPos(f32* dst) {
    dst[0] = pos[0];
    dst[1] = pos[1];
    dst[2] = pos[2];
}

f32 Sandpiper::wrapValue(f32 value) {
    f32 limit = field26;
    if (value >= limit) {
        s32 n = (s32)(value / limit);
        value -= limit * n;
    }
    return value;
}

Sandpiper::Sandpiper() {
    field24 = 0;
    field26 = 0;
    field20 = 0;
    speed = 0.0f;
    vec1[0] = 0.0f;
    vec1[1] = 0.0f;
    vec1[2] = 0.0f;
    vec2[0] = 0.0f;
    vec2[1] = 0.0f;
    vec2[2] = 0.0f;
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 0.0f;
    vec2[3] = 4.999999967e+36f;
}

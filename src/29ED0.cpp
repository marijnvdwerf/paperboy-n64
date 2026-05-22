#include "common.h"

struct Plane {
    f32 nx;
    f32 ny;
    f32 nz;
    f32 d;
};

struct Frustum {
    u8 pad[0x6C];
    Plane planes[6]; // 0x6C..0xCC
};

struct AABB {
    f32 x0, y0, z0;
    f32 x1, y1, z1;
};

struct Vec3f {
    f32 x, y, z;
};

extern "C" s32 func_800292D0(Frustum* frustum, AABB* box) {
    Plane* plane = frustum->planes;
    Plane* end = frustum->planes + 6;
    s32 bits = 0;

    if (plane < end) {
        f32 x0 = box->x0;
        f32 y0 = box->y0;
        f32 z0 = box->z0;
        f32 x1 = box->x1;
        f32 y1 = box->y1;
        f32 z1 = box->z1;

        do {
            f32 A = plane->nx;
            f32 B = plane->ny;
            f32 C = plane->nz;
            f32 D = plane->d;

            f32 Ax0 = A * x0;
            f32 By0 = B * y0;
            f32 Cz0 = C * z0 + D;
            f32 Ax1 = A * x1;
            f32 By1 = B * y1;
            f32 Cz1 = C * z1 + D;

            s32 count = 0;

            f32 sum = By0 + Cz0;
            if (sum + Ax0 < 0.0f) {
                bits |= 0x01;
                count++;
            }
            if (sum + Ax1 < 0.0f) {
                bits |= 0x02;
                count++;
            }

            sum = Ax0 + By1;
            if (sum + Cz0 < 0.0f) {
                bits |= 0x04;
                count++;
            }
            if (sum + Cz1 < 0.0f) {
                bits |= 0x20;
                count++;
            }

            sum = By0 + Cz1;
            if (Ax0 + sum < 0.0f) {
                bits |= 0x08;
                count++;
            }
            if (Ax1 + sum < 0.0f) {
                bits |= 0x40;
                count++;
            }

            sum = Ax1 + By1;
            if (Cz1 + sum < 0.0f) {
                bits |= 0x10;
                count++;
            }
            if (Cz0 + sum < 0.0f) {
                bits |= 0x80;
                count++;
            }

            if (count == 8) {
                return 0;
            }
            plane++;
        } while (plane < end);
    }

    if (bits == 0) {
        return 2;
    }
    return 1;
}

extern "C" s32 func_80029480(Frustum* frustum, Vec3f* point, f32 radius) {
    s32 inside = 0;
    Plane* plane = frustum->planes;
    Plane* end = frustum->planes + 6;

    while (plane < end) {
        f32 dot = plane->nx * point->x + plane->ny * point->y + plane->nz * point->z + plane->d;

        if (radius < dot) {
            inside++;
        } else if (dot < -radius) {
            return 0;
        }

        plane++;
    }

    if (inside == 6) {
        return 2;
    }
    return 1;
}

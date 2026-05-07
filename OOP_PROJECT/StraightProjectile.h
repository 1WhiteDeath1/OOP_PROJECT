#pragma once
#include "Projectile.h"
class StraightProjectile:public Projectile
{
public:
    StraightProjectile(float x, float y, float w, float h,
        int dmg, bool playerOwned,
        float dx, float dy, float spd)
        : Projectile(x, y, w, h,
            dmg, playerOwned,
            dx, dy,
            spd,
            0.0f)    // weight = 0 → no arc, straight line
    {
    }

    virtual ~StraightProjectile() = default;
};


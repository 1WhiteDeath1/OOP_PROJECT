#pragma once
#include "Projectile.h"
class BallisticProjectile: public Projectile
{
public:
    BallisticProjectile(float x, float y, float w, float h,
        int dmg, bool playerOwned,
        float dx, float dy,
        float spd, float weight)
        : Projectile(x, y, w, h,
            dmg, playerOwned,
            dx, dy,
            spd,
            weight)   // weight passed in by subclass
    {
    }

    virtual ~BallisticProjectile() = default;
};


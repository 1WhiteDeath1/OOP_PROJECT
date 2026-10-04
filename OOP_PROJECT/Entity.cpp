#include "Entity.h"

bool Entity::isTouchingBlock(const World& w) const {
    return isTouchingGround(w) ||
        isTouchingCeiling(w) ||
        isTouchingLeftWall(w) ||
        isTouchingRightWall(w);
}

bool Entity::isTouchingGround(const World& w) const {
    float bottomY = y + height;
    float midX = x + width / 2;

    // x + width is the first pixel of the next tile, so the right edge is sampled one pixel inside
    return velocityY >= 0.f &&
        (w.isSolid(x, bottomY) ||
            w.isSolid(midX, bottomY) ||
            w.isSolid(x + width - 1, bottomY));
}

bool Entity::isTouchingCeiling(const World& w) const {
    float midX = x + width / 2;

    return velocityY < 0.f &&
        (w.isSolid(x, y) ||
            w.isSolid(midX, y) ||
            w.isSolid(x + width - 1, y));
}

bool Entity::isTouchingLeftWall(const World& w) const
{
    return velocityX < 0.f &&
        (w.isSolid(x, y + height * 0.25f) ||
            w.isSolid(x, y + height * 0.75f));
}


bool Entity::isTouchingRightWall(const World& w) const
{
    return velocityX > 0.f &&
        (w.isSolid(x + width - 1, y + height * 0.25f) ||
            w.isSolid(x + width - 1, y + height * 0.75f));
}
#include "DamagableEntity.h"
#include "World.h"

void DamagableEntity::applyGravity(float frameTime) {
	if (touchingGround && velocityY >= 0) {
		velocityY = 0;
		return;
	}
	touchingGround = false;
	velocityY += float(G) * frameTime;
	if (velocityY > 800) velocityY = 800;
}
void DamagableEntity::setPosition(float X, float Y) {// its purpose is to fix the char position when in vehicle to vehicle position
	x = X;
	y = Y;

}

void DamagableEntity::checkXCollisions(const World& w) {
	// keep everything inside the map, outside of it there is no ground to land on
	if (x < 0) x = 0;
	if (x > World::WIDTH * World::CELL - width) x = World::WIDTH * World::CELL - width;

	if (isTouchingRightWall(w)) {
		int blockC = (int)((x + width - 1) / World::CELL);
		x = (float)(blockC * World::CELL) - width;
		if (velocityX > 0) velocityX = 0;
	}
	if (isTouchingLeftWall(w)) {
		int blockC = (int)(x / World::CELL);
		x = (float)((blockC + 1) * World::CELL);
		if (velocityX < 0) velocityX = 0;
	}
}

void DamagableEntity::checkYCollisions(const World& w) {
	if (isTouchingGround(w)) {
		float bottomY = y + height;
		int blockR = (int)(bottomY / World::CELL);
		y = (float)(blockR * World::CELL) - height;
		velocityY = 0;
		touchingGround = true;
	}
	else {
		touchingGround = false;
	}
	if (isTouchingCeiling(w)) {
		int blockR = (int)(y / World::CELL);
		y = (float)((blockR + 1) * World::CELL);
		velocityY = 0;
	}
}

void DamagableEntity::checkGroundCollisions(const World& w) {
	checkXCollisions(w);
	checkYCollisions(w);
}

void DamagableEntity::takeDamage(int amount) {
	currentHp -= amount;
	if (currentHp <= 0) {
		currentHp = 0;
		die();
	}
}
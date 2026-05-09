#include "DamagableEntity.h"
#include "World.h"

void DamagableEntity::applyGravity(float frameTime) {
	if (!touchingGround) {
		velocityY += float(G) * frameTime;


		if (velocityY > 800) velocityY = 800;
	}
	else {
		velocityY = 0;
	}
}
void DamagableEntity::setPosition(float X, float Y) {// its purpose is to fix the char position when in vehicle to vehicle position
	x = X;
	y = Y;

}

void DamagableEntity::checkGroundCollisions(const World& w) {
	


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

	//right obstacle
	if (isTouchingRightWall(w))  {
		int blockC = (int)((x + width) / World::CELL);
		x = (float)(blockC * World::CELL) - width;
		if (velocityX > 0) velocityX = 0;
	}

	//left obstacle
	if (isTouchingLeftWall(w)) {
		int blockC = (int)(x / World::CELL);
		x = (float)((blockC +1) * World::CELL) - width;
		if (velocityX < 0) velocityX = 0;
	}

	//top
	if (isTouchingCeiling(w)) {
		int blockR = (int)(y / World::CELL);
		y = (float)((blockR + 1) * World::CELL);
		velocityY = 0;
	}
}

void DamagableEntity::takeDamage(int amount) {
	currentHp -= amount;
	if (currentHp <= 0) {
		currentHp = 0;
		die();
	}
}
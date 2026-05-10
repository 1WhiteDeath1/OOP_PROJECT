#include "Soldier.h"


Soldier::Soldier(float x, float y, float w, float h, int hp) : DamagableEntity(x, y, w, h, hp) {
}

Soldier::~Soldier() {
	delete inventory[0];
	delete inventory[1];
}


void Soldier::jump() {
	if (touchingGround) {
		velocityY = -jumpStrength;
		isJumping = true;
		touchingGround = false;
	}
}

void Soldier::crouch(bool c) {
	isCrouching = c;
}

void Soldier::movement(float frameTime, const World& w) {

	//gravity
	applyGravity(frameTime);


	//move
	x += velocityX * frameTime;
	y += velocityY * frameTime;


	//collisions
	checkGroundCollisions(w);

	//friction
	velocityX *= 0.75;

	//max speed
	if (velocityX > walkSpeed) velocityX = walkSpeed;
	if (velocityX < -walkSpeed) velocityX = -walkSpeed;

	if (touchingGround) isJumping = false;

}


void Soldier::changeAngle(float angle) {
	aimAngle += angle;
	if (aimAngle > 45) aimAngle = 45;
	if (aimAngle < -45) aimAngle = -45;
}

Projectile* Soldier::fireWeapon() {
	int slot = (inventory[1] && inventory[1]->hasAmmo()) ? 1 : 0;
	if (!inventory[slot]) return nullptr;

	float fireX = facingRight ? (x + width) : x;
	float fireY = y + height / 2;

	return inventory[slot]->fire(fireX, fireY, aimAngle, facingRight);
}

void Soldier::respawn(float spawnX, float spawnY) {
	x = spawnX;
	y = spawnY;
	velocityX = 0;
	velocityY = 0;
	currentHp = maxHp;
	velocityX = 0.f;
	velocityY = 0.f;
	currentHp = maxHp;
	isActive = true;
	currLives--;
}

void Soldier::setWeapon(int slot, Weapon* w) {
	if (slot < 0 || slot>1) return;
	delete inventory[slot];
	inventory[slot] = w;
}
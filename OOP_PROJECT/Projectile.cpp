#include "Projectile.h"

void Projectile::update(float frameTime, const World& w) {

	y += G * weight * frameTime;

	if (Dy > 1) Dy = 1;

	x += Dx * speed * frameTime;
	y += Dy * speed * frameTime;


	float midX = x + width / 2.f;
	float midY = y + height / 2.f;
	if (isTouchingBlock(w)) {
		isActive = false;
		return;
}


	//checking world bounds
	if (x < -100.f ||
		x > World::WIDTH * World::CELL + 100.f ||
		y < -100.f ||
		y > World::HEIGHT * World::CELL + 100.f)
	{
		isActive = false;
	}



}
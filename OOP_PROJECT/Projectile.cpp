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

	if (x < -100.f ||
		x > 110 * 64 + 100.f ||
		y < -100.f ||
		y > 14 * 64 + 100.f)
	{
		isActive = false;
	}



}
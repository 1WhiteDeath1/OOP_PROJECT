#include "WeaponsEach.h"


//bullet
Texture Bullet::tex;
bool Bullet::texIsLoaded = false;

void Bullet::applyDamage(DamagableEntity* target) {
	target->takeDamage(damage);
}
void Bullet::render(RenderWindow& w, const Camera& cam) {
	// point the sprite the way the bullet is flying
	sprite.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
	sprite.setRotation(std::atan2(Dy, Dx) * 180.f / 3.14159f);
	sprite.setPosition(cam.toScreenX(x + width / 2.f), cam.toScreenY(y + height / 2.f));
	w.draw(sprite);
}

//Rocket
Texture Rocket::tex;
bool Rocket::texIsLoaded = false;

void Rocket::applyDamage(DamagableEntity* target) {
	target->takeDamage(damage);
}

void Rocket::render(RenderWindow& w, const Camera& cam) {
	sprite.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
	sprite.setRotation(std::atan2(Dy, Dx) * 180.f / 3.14159f);
	sprite.setPosition(cam.toScreenX(x + width / 2.f), cam.toScreenY(y + height / 2.f));
	w.draw(sprite);
}

//firestream

Texture FireStream::tex;
bool FireStream::texIsLoaded = false;
void FireStream::update(float frameTime, const World& w) {
	Projectile::update(frameTime, w);
	life += frameTime;
	if (life >= lifeTime) {
		isActive = false;
	}
}
void FireStream::applyDamage(DamagableEntity* target) {
	target->takeDamage(damage);
}

void  FireStream::render(RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(x), cam.toScreenY(y));
	w.draw(sprite);
}

//laser
Texture LaserBeam::tex;
bool LaserBeam::texIsLoaded = false;

void LaserBeam::applyDamage(DamagableEntity* target) {
	target->takeDamage(damage);
	pierce += 1;
	if (pierce >= max) {
		isActive = false;
	}
}

void LaserBeam::render(sf::RenderWindow& w, const Camera& cam)
{
	sprite.setPosition(cam.toScreenX(x), cam.toScreenY(y));
	w.draw(sprite);
}

//normal gernade
Texture NormalGrenade::tex;
bool NormalGrenade::texIsLoaded = false;

void NormalGrenade::update(float frameTime, const World& w) {
	if (exploded) return;

	fuseTimer -= frameTime;
	if (fuseTimer <= 0) {
		explode();
		return;
	}


	Dy += 0.8f * frameTime;
	if (Dy > 5.f) Dy = 5.f;

	x += Dx * speed * frameTime;
	y += Dy * speed * frameTime;

	if (isTouchingGround(w)) {
		float bottomY = y + height;
		int blockR = (int)(bottomY / World::CELL);
		y = (float)(blockR * World::CELL) - height;

		//dampeing
		Dy = -Dy * 0.4;
		Dx = Dx * 0.6;
	}
	if (isTouchingLeftWall(w) || isTouchingRightWall(w)) {
		Dx = -Dx * 0.4;
	}
	if (isTouchingCeiling(w)) {
		Dy = Dy * 0.3;
	}

}

void NormalGrenade::explode() {
	exploded = true;
	isActive = false;
}

void NormalGrenade::applyDamage(DamagableEntity* target) {
	target->takeDamage(damage);
}
void NormalGrenade::render(RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(x), cam.toScreenY(y));
	w.draw(sprite);
}


//firebomb
void FireBombGrenade::update(float frameTime, const World& w) {
	if (exploded) return;

	fuseTimer -= frameTime;
	if (fuseTimer <= 0) {
		explode();
		return;
	}

	Dy += 0.3f * frameTime;
	if (Dy > 3.f) Dy = 3.f;

	x += Dx * speed * frameTime;
	y += Dy * speed * frameTime;

	if (isTouchingGround(w) ||
		isTouchingLeftWall(w) ||
		isTouchingRightWall(w) ||
		isTouchingCeiling(w))
	{
		explode();
		return;
	}

	
}


void FireBombGrenade::explode() {
	exploded = true;
	isActive = false;
}

void FireBombGrenade::applyDamage(DamagableEntity* target) {
	target->takeDamage(damage);
}
void FireBombGrenade::render(RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(x), cam.toScreenY(y));
	w.draw(sprite);
}
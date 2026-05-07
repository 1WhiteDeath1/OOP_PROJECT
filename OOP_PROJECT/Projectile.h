#pragma once

#include "Entity.h"
#include "DamagableEntity.h"
class Projectile:public Entity
{
protected:
	int damage;
	bool isPlayerOwned;
	float Dx, Dy;
	float speed;
	float weight;

	bool exploded = false;
	float blastRadius = 0;

	static const int G = 500;

public:
	Projectile(float x, float y, float w, float h, int damage, bool playerO, float dx, float dy, float v, float we) : Entity(x, y, w, h),
		damage(damage), isPlayerOwned(playerO), Dx(dx), Dy(dy), speed(v), weight(we) {
	}
	void update(float frameTime, const World& w) override;
	virtual void applyDamage(DamagableEntity* target) = 0;
	virtual void render(RenderWindow& w, const Camera& cam) = 0;

	bool isFromPlayer() const {
		return isPlayerOwned;
	}
	int getDamage() const {
		return damage;
	}
	float getBlastRadius() const {
		return blastRadius;
	}
	bool didExplode() const {
		return exploded;
	}

	virtual ~Projectile() = default;
};


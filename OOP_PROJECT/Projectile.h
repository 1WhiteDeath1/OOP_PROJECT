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
	float lastX, lastY; // where it was at the start of this frame

	static const int G = 500;

public:
	Projectile(float x, float y, float w, float h, int damage, bool playerO, float dx, float dy, float v, float we) : Entity(x, y, w, h),
		damage(damage), isPlayerOwned(playerO), Dx(dx), Dy(dy), speed(v), weight(we), lastX(x), lastY(y) {
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
	virtual bool diesOnHit() const { return true; }
	float getDirX() const { return Dx; }

	// called before moving: a 900px/s bullet moves 15-30px a frame, so checking only where it ends up
	// could jump over the edge of an enemy, instead check the whole stretch it moved this frame
	void rememberPosition() { lastX = x; lastY = y; }
	bool hitOnTheWay(const Entity& e) const {
		float left = lastX < x ? lastX : x;
		float right = (lastX > x ? lastX : x) + width;
		float top = lastY < y ? lastY : y;
		float bottom = (lastY > y ? lastY : y) + height;
		return left < e.getX() + e.getWidth() && e.getX() < right &&
			top < e.getY() + e.getHeight() && e.getY() < bottom;
	}

	virtual ~Projectile() = default;
};


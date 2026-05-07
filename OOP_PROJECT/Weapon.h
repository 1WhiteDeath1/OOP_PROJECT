#pragma once
#include "Projectile.h"

class Weapon
{
protected:
	float fireRate;
	float coolDown = 0;
	int currAmmo;
	int maxAmmo;
	bool infiniteAmmo;
public:
	Weapon(float rate, int ammo, bool i): fireRate(rate), currAmmo(ammo), maxAmmo(ammo), infiniteAmmo(i){}

	virtual Projectile* fire(float x, float y, float angle, bool facingRight) = 0;

	void update(float frameTime) {
		if (coolDown >= 0) {
			coolDown -= frameTime;
		}
	}


	bool hasAmmo() const {
		return infiniteAmmo || currAmmo > 0;
	}
	bool canFire() const {
		return coolDown <= 0 && hasAmmo();
	}
	int getAmmo() const {
		return currAmmo;
	}
	bool isInfinite() const {
		return infiniteAmmo;
	}

	void addAmmo(int n) {
		currAmmo += n;
		if (currAmmo > maxAmmo) currAmmo = maxAmmo;
	}


	virtual ~Weapon() = default;
};


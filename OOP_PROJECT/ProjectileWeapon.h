#pragma once
#include "Weapon.h"
class ProjectileWeapon: public Weapon
{
public:
	ProjectileWeapon(float rate, int ammo, bool infinite) :
		Weapon(rate, ammo, infinite) {
	}
	static void getDirection(float angle, bool facingRight, float& x, float& y);

protected:
	static void lookUp(float deg, float& s, float& c); // sin and cos of -45..45 degrees, between whole degrees too

	static const float sin_values[91];
	static const float cos_values[91];

	};


#pragma once

#include "StraightProjectile.h"
#include "BallisticProjectile.h"
#include "DamagableEntity.h"
#include "ProjectileWeapon.h"
using namespace sf;




//projectiles
class Bullet :public StraightProjectile
{
private:
	static Texture tex;
	static bool texIsLoaded;
public:
	Bullet(float x, float y, float dx, float dy, bool fromPlayer, int damage = 20) : StraightProjectile(x, y, 10, 5, damage, fromPlayer, dx, dy, 900) {
		if (!texIsLoaded)
		{
			tex.loadFromFile("Sprites/bullet.png");
			texIsLoaded = true;
		}
		sprite.setTexture(tex);
		sprite.setScale(
			20.f / 2816,
			10.f / 1536
		);

	}
	void applyDamage(DamagableEntity* target) override;
	void render(RenderWindow& w, const Camera& cam) override;
};




class Rocket :public StraightProjectile {
private:
	static Texture tex;
	static bool texIsLoaded;

	float blastRadius = 100;
public:
	Rocket(float x, float y, float dx, float dy, bool fromPlayer, int damage = 80) : StraightProjectile(x, y, 20, 10, damage, fromPlayer, dx, dy, 700) {
		if (!texIsLoaded)
		{
			tex.loadFromFile("Sprites/rocket.png");
			texIsLoaded = true;
		}
		sprite.setTexture(tex);
		sprite.setScale(
			30.f / tex.getSize().x,
			20.f / tex.getSize().y
		);
	}

	void applyDamage(DamagableEntity* target) override;
	void render(sf::RenderWindow& w, const Camera& cam) override;


};

class FireStream :public StraightProjectile {
	static Texture tex;
	static bool texIsLoaded;
	float life = 0;
	float lifeTime = 0.4;
public:
	FireStream(float x, float y, float dx, float dy, bool fromPlayer, int damage = 8) : StraightProjectile(x, y, 12.f, 12.f,
		damage,
		fromPlayer,
		dx, dy,
		350.f)
	{
		if (!texIsLoaded)
		{
			tex.loadFromFile("Sprites/fire.png");
			texIsLoaded = true;
		}
		sprite.setTexture(tex);
		sprite.setScale(
			12.f / tex.getSize().x,
			12.f / tex.getSize().y
		);
	}

	void update(float dt, const World& w) override;

	void applyDamage(DamagableEntity* target) override;
	void render(sf::RenderWindow& w, const Camera& cam) override;

};

class LaserBeam : public StraightProjectile {
	static Texture tex;
	static bool texIsLoaded;
	int pierce = 0;
	int max = 3;
public:
	LaserBeam(float x, float y, float dx, float dy, bool fromPlayer) : StraightProjectile(x, y, 30.f, 4.f,
		60,          // damage — high
		fromPlayer,
		dx, dy,
		1800.f)      // speed — very fast, near instant
	{
		if (!texIsLoaded)
		{
			tex.loadFromFile("Sprites/laser.png");
			texIsLoaded = true;
		}
		sprite.setTexture(tex);
		sprite.setScale(
			30.f / tex.getSize().x,
			4.f / tex.getSize().y
		);
	}

	void applyDamage(DamagableEntity* target) override;
	void render(sf::RenderWindow& w, const Camera& cam) override;

};

class NormalGrenade :public BallisticProjectile {
private:
	static Texture tex;
	static bool texIsLoaded;
	float fuseTimer = 2.5;
	bool exploded = false;

	void explode();
public:
	NormalGrenade(float x, float y, float dx, float dy, bool fromPlayer, int damage=3) : BallisticProjectile(x, y, 10, 5, 60, fromPlayer, dx, dy, 400, 3) {
		blastRadius = 100;

		if (!texIsLoaded)
		{
			tex.loadFromFile("Sprites/grenade.png");
			texIsLoaded = true;
		}
		sprite.setTexture(tex);
		sprite.setScale(
			16.f / tex.getSize().x,
			16.f / tex.getSize().y
		);
	}


	void update(float frameTime, const World& w) override;
	void applyDamage(DamagableEntity* target) override;
	void render(sf::RenderWindow& w, const Camera& cam) override;

	float getBlastRadiud() const { return blastRadius; }
	bool didExplode() const { return exploded; }
};

class FireBombGrenade : public BallisticProjectile {
private:
	static Texture tex;
	static bool texIsLoaded;
	float fuseTimer = 3;
	bool exploded = false;

	void explode();
public:

	FireBombGrenade(float x, float y, float dx, float dy, bool fromPlayer) : BallisticProjectile(x, y, 10, 5, 40, fromPlayer, dx, dy, 360, 2.5) {
		blastRadius = 100;

		if (!texIsLoaded)
		{
			tex.loadFromFile("Sprites/grenade.png");
			texIsLoaded = true;
		}
		sprite.setTexture(tex);
		sprite.setScale(
			16.f / tex.getSize().x,
			16.f / tex.getSize().y
		);
	}


	void update(float frameTime, const World& w) override;
	void applyDamage(DamagableEntity* target) override;
	void render(sf::RenderWindow& w, const Camera& cam) override;

	float getBlastRadiud() const { return blastRadius; }
	bool didExplode() const { return exploded; }
};








//weaponsfasdlf;asdjsdl
class Pistol : public ProjectileWeapon {
public:
	Pistol() : ProjectileWeapon(0.25, 0, true) {};

	Projectile* fire(float x, float y, float angle, bool facingRight) override {
		if (!canFire()) return nullptr;
		coolDown = fireRate;
		float dx, dy;
		getDirection(angle, facingRight, dx, dy);
		return new Bullet(x, y, dx, dy, true, 3);
	}

};


class HeavyMachineGun : public ProjectileWeapon {
public: 
	HeavyMachineGun(int ammo) :ProjectileWeapon(0.125, ammo, false) {}

	Projectile* fire(float x, float y, float angle, bool facingRight) override {
		if (!canFire()) return nullptr;

		coolDown = fireRate;
		currAmmo -= 1;
		float dx, dy;
		getDirection(angle, facingRight, dx, dy);
		return new Bullet(x, y, dx, dy, true, 3);
	}
};

class RocketLauncher : public ProjectileWeapon {
public: 
	RocketLauncher(int ammo) : ProjectileWeapon(2, ammo, false) {}

	Projectile* fire(float x, float y, float angle, bool facingRight) override{
		if (!canFire()) return nullptr;

		coolDown = fireRate;
		currAmmo -= 1;
		float dx, dy;
		getDirection(angle, facingRight, dx, dy);
		return new Rocket(x, y, dx, dy, true, 5);
	}
};

class FlameShot : public ProjectileWeapon {
public:
	FlameShot(int ammo) : ProjectileWeapon(0.05, ammo, false) {}

	Projectile* fire(float x, float y, float angle, bool facingRight) override {
		if (!canFire()) return nullptr;

		coolDown = fireRate;
		currAmmo -= 1;
		float dx, dy;
		getDirection(angle, facingRight, dx, dy);
		return new FireStream(x, y, dx, dy, true, 2);
	}
};

class LaserGun : public ProjectileWeapon {
public:
	LaserGun(int ammo) : ProjectileWeapon(0.5, ammo, false) {}

	Projectile* fire(float x, float y, float angle, bool facingRight) override {
		if (!canFire()) return nullptr;

		coolDown = fireRate;
		currAmmo -= 1;
		float dx, dy;
		getDirection(angle, facingRight, dx, dy);
		return new LaserBeam(x, y, dx, dy, true);
	}
};

















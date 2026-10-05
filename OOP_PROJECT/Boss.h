#pragma once
#include "Enemy.h"

// the final boss: a big rebel tank. it is an Enemy, so bullets, grenades, score and touching
// damage all work through the normal enemy code; only its movement, attacks and drawing are its own.
// attacks: a burst of 3 aimed cannon shells, a volley of 4 lobbed grenades, and once it is
// below half health it also charges at the player
class BossTank : public Enemy
{
	static Texture driveTex, shootTex, wreckTex;
	static bool texIsLoaded;
	static const int DRIVE_FRAMES = 6, SHOOT_FRAMES = 4;

	float cannonTimer = 3;   // next cannon burst
	float mortarTimer = 7;   // next grenade volley
	float chargeTimer = 10;  // next charge (only below half health)
	int burstLeft = 0;       // cannon shells left in the current burst
	int mortarLeft = 0;      // grenades left in the current volley
	float stepTimer = 0;     // time to the next shell / grenade of a burst
	float chargeTime = 0;    // seconds left of the current charge
	float shootAnim = 0;     // shows the shooting frames for a moment after firing
	bool roared = false;     // played the roar when the player first came close
	float blastTimer = 0;    // while dying: time to the next explosion

public:
	static const int BOSS_W = 240, BOSS_H = 190;
	BossTank(float x, float y);
	void update(float dt, const World& w) override;
	void render(RenderWindow& w, const Camera& cam) override;
	void throwProjectile() override; // one cannon shell aimed at the player
	void die() override;
	bool isBoss() const override { return true; }
	bool takeExplosion(float& ex, float& ey) override;
};

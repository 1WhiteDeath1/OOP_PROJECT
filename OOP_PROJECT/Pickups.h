#pragma once
#include "Entity.h"

// an item dropped by a killed enemy: falls to the ground and waits to be picked up.
// it blinks and disappears if nobody takes it in time
class ItemDrop : public Entity
{
public:
	enum Type { AMMO, GRENADES, HEALTH, TYPE_COUNT };
private:
	static Texture tex[TYPE_COUNT];
	static bool texIsLoaded;
	Type type;
	float life = 10; // seconds before it disappears
public:
	ItemDrop(float x, float y, Type type);
	void update(float dt, const World& w) override;
	void render(RenderWindow& window, const Camera& cam) override;
	Type getType() const { return type; }
};


// a tied-up prisoner (P.O.W.). touching him sets him free: he runs off and the player
// gets points and a weapon (done by the EntityManager)
class Prisoner : public Entity
{
	static Texture tiedTex, runTex;
	static bool texIsLoaded;
	static const int RUN_FRAMES = 12;
	bool freed = false;
	float runTimer = 0;
public:
	Prisoner(float x, float y);
	void update(float dt, const World& w) override;
	void render(RenderWindow& window, const Camera& cam) override;
	void free() { freed = true; }
	bool isFreed() const { return freed; }
};

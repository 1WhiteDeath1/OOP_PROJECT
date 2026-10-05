#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Entity.h"
#include "World.h"
#include "Projectile.h"
#include "Player.h"
#include "WeaponCollectible.h"
#include "Vehicle.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Score.h"
#include "Pickups.h"

using namespace sf;


class EntityManager
{
	static const int MAX_PROJECTILES = 100;
	static const int MAX_ENEMIES = 100;
	static const int MAX_VEHICLES = 10;
	static const int MAX_COLLECTIBLES = 15;
	static const int MAX_EXPLOSIONS = 20;
	static const int MAX_DROPS = 30;
	static const int MAX_PRISONERS = 10;


	Projectile* projectiles[MAX_PROJECTILES];
	Enemy* enemies[MAX_ENEMIES];
	Player* player = nullptr;
	Score* score = nullptr; // where kills are added (owned by PlayState)
	WeaponCollectible* collectibles[MAX_COLLECTIBLES];
	Vehicle* vehicles[MAX_VEHICLES];
	Explosion explosions[MAX_EXPLOSIONS]; // plain objects, reused when they finish
	ItemDrop* drops[MAX_DROPS];
	Prisoner* prisoners[MAX_PRISONERS];

	int pCount = 0;
	int eCount = 0;
	int sCount = 0;
	int wCount = 0;
	int vCount = 0;
	float coolDown = 0;
	float shakeRequest = 0; // strongest screen shake asked for this frame
public:
	EntityManager();
	~EntityManager();
	void addProjectile(Projectile* p);
	void addEnemy(Enemy* e);
	void addVehicle(Vehicle* v);
	void addWeapon(WeaponCollectible* wC);
	void addExplosion(float x, float y, float height);
	void addDrop(ItemDrop* d);
	void addPrisoner(Prisoner* p);
	void setPlayer(Player* p) { player = p; }
	void setScore(Score* s) { score = s; }

	void update(float frameTime, const World& w);
	void render(RenderWindow& w, const Camera& cam);

	int getEnemyCount() const { return eCount; }
	float takeShake() { float s = shakeRequest; shakeRequest = 0; return s; }

	void checkProjectileWorldCollisions(World& w);
	void checkProjectileCollisions();
	void checkEnemyPlayerCollisions();
	void checkMeleeCollisions();
	void checkCollectiblesCollisions();
	void checkEnemyProjectilePlayerCollisions();
	void checkVehicleEntry();

	void checkGrenadeBlast();
	void checkPickups(); // item drops and prisoners touched by the player

	static bool overlaps(const Entity& a, const Entity& b);
};


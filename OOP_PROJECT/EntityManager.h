#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Entity.h"
#include "World.h"
#include "Projectile.h"
#include "Player.h"
using namespace sf;


class EntityManager
{
	static const int MAX_PROJECTILES = 100;
	static const int MAX_ENEMIES = 100;
	static const int MAX_SOLDIERS = 4;

	Projectile* projectiles[MAX_PROJECTILES];
	Enemy* enemies[MAX_ENEMIES];
	Player* player = nullptr;

	int pCount = 0;
	int eCount = 0;
	int sCount = 0;
	float coolDown = 0;
public:
	EntityManager();
	~EntityManager();
	void addProjectile(Projectile* p);
	void addEnemy(Enemy* e);
	void setPlayer(Player* p) { player = p; }

	void update(float frameTime, const World& w);
	void render(RenderWindow& w, const Camera& cam);

	void checkProjectileWorldCollisions(World& w);
	void checkProjectileCollisions();
	void checkEnemyPlayerCollisions();
	void checkMeleeCollisions();

	void checkGrenadeBlast();
};


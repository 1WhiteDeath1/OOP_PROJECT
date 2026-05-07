#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Entity.h"
#include "World.h"
#include "Projectile.h"
using namespace sf;


class EntityManager
{
	static const int MAX_PROJECTILES = 100;
	static const int MAX_ENEMIES = 100;
	static const int MAX_SOLDIERS = 4;

	Projectile* projectiles[MAX_PROJECTILES];
	Enemy* enemies[MAX_ENEMIES];
	Soldier* soldiers[MAX_SOLDIERS];

	int pCount = 0;
	int eCount = 0;
	int sCount = 0;
	float coolDown = 0;
public:
	EntityManager();
	~EntityManager();
	void addProjectile(Projectile* p);
	void addEnemy(Enemy* e);
	void addSoldier(Soldier* s);

	void update(float frameTime);
	void render(RenderWindow& w, Camera& cam);

	void checkProjectileWorldCollisions(World& w);
	void checkProjectileCollisions();
	void checkEnemyPlayerCollisions();
	
	Soldier* getSoldierCurr(int index) const;
};


#pragma once
#include "World.h"
#include "EntityManager.h"
#include "GroundVehicle.h"
#include "AerialVehicle.h"
#include "AquaticVehicle.h"

struct spawn {
	int type;
	float x, y;
};

class Level
{
	int level;

	spawn enemySpawn[20];
	spawn vehicleSpawn[20];
	spawn weaponSpawn[15];


	int eCount = 0;
	int vCount = 0;
	int wCount = 0;

	void level1(const World& w);

	float isOnGround(const World& w, int col, int eHeight) const;
public:
	Level(int level, const World& w);

	void setUP(EntityManager& eManager) const;

};


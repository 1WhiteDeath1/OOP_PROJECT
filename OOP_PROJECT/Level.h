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
	spawn prisonerSpawn[10];


	int eCount = 0;
	int vCount = 0;
	int wCount = 0;
	int pCount = 0;

	int goalColumn = 0; // reaching this column finishes the mission (0 = no goal flag)

	void enemyAt(int type, int col, const World& w);
	void vehicleAt(int type, int col, const World& w);
	void weaponAt(int type, int col, const World& w);
	void prisonerAt(int col, const World& w);

	void level1(const World& w);
	void level2(const World& w);
	void level3(const World& w);

	float isOnGround(const World& w, int col, int eHeight, int eWidth = World::CELL) const;
public:
	Level(int level, const World& w);

	void setUP(EntityManager& eManager) const;
	int getGoalColumn() const { return goalColumn; }

};


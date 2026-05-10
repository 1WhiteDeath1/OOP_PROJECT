#include "Level.h"


float Level::isOnGround(const World& w, int col) const {
	float x = col * World::CELL;
	for (int row = 0; row < World::HEIGHT;row++) {
		float y = row * World::CELL;
		if (w.isSolid(x, y)) {
			y= y - World::CELL;
			return y;
		}
	}
	return 0;
}

Level::Level(int level, const World& w) : level(level) {
	level1(w);
}

void Level::level1(const World& w) {

	//enemies spawn
	



	//vehicles
	vehicleSpawn[0] = { 0, (float)(90 * World::CELL), isOnGround(w,90) };
	vehicleSpawn[1] = { 3, (float)(160 * World::CELL), isOnGround(w,90) };

	vCount += 2;


	//weapons
}

void Level::setUP(EntityManager& em) const {
	for (int i = 0; i < eCount;i++) {
		if (vehicleSpawn->type==1){
			em.addEnemy();
		}
		else if (vehicleSpawn->type == 1) {
			em.addEnemy();
		}
		else if (vehicleSpawn->type == 1) {
			em.addEnemy();
		}
		else if (vehicleSpawn->type == 1) {
			em.addEnemy();
		}
		
	}
	for (int i = 0; i < vCount;i++) {
		em.addVehicle();
	}
	for (int i = 0; i < wCount;i++) {
		em.addWeapon();
	}
}
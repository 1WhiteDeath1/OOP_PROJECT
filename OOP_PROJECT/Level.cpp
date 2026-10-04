#include "Level.h"
#include "Enemy.h"

float Level::isOnGround(const World& w,int col, int eHeight, int eWidth) const {
	// wide things (vehicles) cover several columns, so stand them on the highest one
	int lastCol = col + (eWidth - 1) / World::CELL;
	for (int row = 0; row < World::HEIGHT;row++) {
		float y = row * World::CELL;
		for (int c = col; c <= lastCol; c++) {
			if (w.isSolid(c * World::CELL, y)) {
				y = y - eHeight;
				return y;
			}
		}
	}
	return 0;
}

Level::Level(int level, const World& w) : level(level) {
	level1(w);
}

int EnemyHeight = 96;
int EnemyWidth = 64;
int weaponHeight = 32;

// hitbox size of each vehicle type (0=MetalSlug 1=SlugFlyer 2=SlugMariner 3=FlyingTara 4=EnemySub 5=M15Bradley 6=AmphibiousSlug)
// = picture size x 2.4 (Entity::PIXEL_SCALE) so vehicles keep the right size next to the 96px tall soldiers
// (the slug flyer picture was already 4x so it gets 0.6, the slug mariner uses the sub picture made a bit smaller)
float vehicleWidth[7]  = { 146, 190, 149, 192, 223, 192, 146 };
float vehicleHeight[7] = { 134, 125,  78,  91, 118, 187, 134 };

void Level::level1(const World& w) {
	//enemies
	enemySpawn[eCount++] = { 0, 20 * World::CELL, isOnGround(w,20, EnemyHeight) };//rebel 
	enemySpawn[eCount++] = { 0, 35 * World::CELL, isOnGround(w,35, EnemyHeight) };//rebel 
	enemySpawn[eCount++] = { 1, 50 * World::CELL, isOnGround(w,50, EnemyHeight) };//rebel 
	enemySpawn[eCount++] = { 2, 70 * World::CELL, isOnGround(w,70, EnemyHeight) };//rebel 
	enemySpawn[eCount++] = { 3, 85 * World::CELL, isOnGround(w,85, EnemyHeight) };//rebel 
	enemySpawn[eCount++] = { 0, 100 * World::CELL, isOnGround(w,100, EnemyHeight) };//rebel 
	enemySpawn[eCount++] = { 1, 115 * World::CELL, isOnGround(w,115, EnemyHeight) };//rebel 


	//vehicles

	vehicleSpawn[vCount++] = { 1, 6 * World::CELL, isOnGround(w, 6, vehicleHeight[1], vehicleWidth[1]) };  // SlugFlyer
	vehicleSpawn[vCount++] = { 0,  60 * World::CELL, isOnGround(w, 60, vehicleHeight[0], vehicleWidth[0]) };  // MetalSlug
	vehicleSpawn[vCount++] = { 3, 70 * World::CELL, isOnGround(w,70, vehicleHeight[3], vehicleWidth[3]) - 300 };  // FlyingTara, hovers above the ground
	vehicleSpawn[vCount++] = { 5, 110 * World::CELL, isOnGround(w, 110, vehicleHeight[5], vehicleWidth[5]) };  // M15Bradley
	vehicleSpawn[vCount++] = { 6, 128 * World::CELL, isOnGround(w, 128, vehicleHeight[6], vehicleWidth[6]) };  // AmphibiousSlug
	vehicleSpawn[vCount++] = { 2, 140 * World::CELL, isOnGround(w, 140, vehicleHeight[2], vehicleWidth[2]) };  // SlugMariner, resting on the sea bed
	vehicleSpawn[vCount++] = { 4, 175 * World::CELL, (World::seaLEVEL + 2) * World::CELL };  // EnemySub

	//weapons collectibles

	//   0=HeavyMachineGun  1=RocketLauncher  2=FlameShot  3=LaserGun
	weaponSpawn[wCount++] = { 0, 15 * World::CELL, isOnGround(w, 15, weaponHeight) };   // HeavyMachineGun
	weaponSpawn[wCount++] = { 2, 45 * World::CELL, isOnGround(w, 45, weaponHeight) };   // FlameShot
	weaponSpawn[wCount++] = { 1, 80 * World::CELL, isOnGround(w, 80, weaponHeight) };   // RocketLauncher
	weaponSpawn[wCount++] = { 3, 100 * World::CELL, isOnGround(w, 100, weaponHeight) };   // LaserGun

}

void Level::setUP(EntityManager& em) const {
	for (int i = 0; i < eCount;i++) {

		Enemy* e = nullptr;
		const spawn& sp = enemySpawn[i];

		if (sp.type == 0) {
			e = new RebelSoldier(sp.x, sp.y, EnemyWidth, EnemyHeight, 30);
		}
		else if (sp.type == 1) {
			e = new ShieldedSoldier(sp.x, sp.y, EnemyWidth, EnemyHeight, 30);
		}
		else if (sp.type == 2) {
			e = new BazookaSoldier(sp.x, sp.y, EnemyWidth, EnemyHeight, 25);
		}
		else if (sp.type == 3) {
			e = new GrenadeSoldier(sp.x, sp.y, EnemyWidth, EnemyHeight ,25);
		}

		if (e) em.addEnemy(e);
	}

	//vehicles
	for (int i = 0; i < vCount;i++) {

		Vehicle* v = nullptr;
		const spawn& sp = vehicleSpawn[i];
		float vw = vehicleWidth[sp.type], vh = vehicleHeight[sp.type];

		if (sp.type == 0) {
			v = new MetalSlug(sp.x, sp.y, vw, vh, 40, 0.6f, 1);
		}
		else if (sp.type == 1) {
			v = new SlugFlyer(sp.x, sp.y, vw, vh, 30, 1, sp.y);
		}
		else if (sp.type == 2) {
			v = new SlugMariner(sp.x, sp.y, vw, vh, 30, 0.3f, 1);
		}
		else if (sp.type == 3) {
			v = new FlyingTara(sp.x, sp.y, vw, vh, 10, 3.f, 1, sp.y);
		}
		else if (sp.type == 4) {
			v = new EnemySub(sp.x, sp.y, vw, vh, 2.f, 1);
		}
		else if (sp.type == 5) {
			v = new M15Bradley(sp.x, sp.y, vw, vh, 2.f, 1);
		}
		else if (sp.type == 6) {
			v = new AmphibiousSlug(sp.x, sp.y, vw, vh, 40, 0.6f, 1);
		}


		if (v) em.addVehicle(v);
	}
	for (int i = 0; i < wCount;i++) {
		const spawn& sp = weaponSpawn[i];
		Weapon* w = nullptr;
		const char* texture = "";

		if (sp.type == 0) {
			w = new HeavyMachineGun(60);
			texture = "25I-0504_25I-0644_Assets/hmg.png";
		}
		else if (sp.type == 1) {
			w = new RocketLauncher(5);
			texture = "25I-0504_25I-0644_Assets/RocketLauncher.png";
		}
		else if (sp.type == 2) {
			w = new FlameShot(100);
			texture = "25I-0504_25I-0644_Assets/flameShot.png";
		}
		else if (sp.type == 3) {
			w = new LaserGun(10);
			texture = "25I-0504_25I-0644_Assets/laserGun.png";
		}

		if (w) em.addWeapon(new WeaponCollectible(sp.x, sp.y, w, texture));
	}
}
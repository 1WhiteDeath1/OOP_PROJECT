#include "Level.h"
#include "Enemy.h"

float Level::isOnGround(const World& w,int col, int eHeight) const {
	float x = col * World::CELL;
	for (int row = 0; row < World::HEIGHT;row++) {
		float y = row * World::CELL;
		if (w.isSolid(x, y)) {
			y = y - eHeight;
			return y;
		}
	}
	return 0;
}

Level::Level(int level, const World& w) : level(level) {
	level1(w);
}

int EnemyHeight = 96;
int EnemyWidth = 64;
int vehicleHeight = 96;
int weaponHeight = 20;

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

	vehicleSpawn[vCount++] = { 0,  60 * World::CELL, isOnGround(w, 60, vehicleHeight) };  // MetalSlug
	vehicleSpawn[vCount++] = { 3, 70 * World::CELL, isOnGround(w,70, vehicleHeight) };  // FlyingTara

	//weapons collectibles

	//   0=HeavyMachineGun  1=RocketLauncher  2=FlameShot  3=LaserGun
	weaponSpawn[wCount++] = { 0, 45 * World::CELL, isOnGround(w, 45, weaponHeight) };   // HeavyMachineGun
	weaponSpawn[wCount++] = { 1, 80 * World::CELL, isOnGround(w, 80, weaponHeight) };   // RocketLauncher

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

		if (sp.type == 0) {
			v = new MetalSlug(sp.x, sp.y, 64, 32, 20, 5, 1);
		}
		else if (sp.type == 1) {
			v = new SlugFlyer(sp.x, sp.y, 64, 32, 15, 1, sp.y);
		}
		else if (sp.type == 2) {
			v = new SlugMariner(sp.x, sp.y, 64, 32, 15, 1, 1);
		}
		else if (sp.type == 3) {
			v = new FlyingTara(sp.x, sp.y, 64, 32, 10, 1, 1, sp.y);
		}
		else if (sp.type == 4) {
			v = new EnemySub(sp.x, sp.y, 64, 32, 1, 1);
		}


		if (v) em.addVehicle(v);
	}
	for (int i = 0; i < wCount;i++) {
		const spawn& sp = weaponSpawn[i];
		Weapon* w = nullptr;
		const char* texture = "";

		if (sp.type == 0) {
			w = new HeavyMachineGun(60);
			texture = "Sprites/hmg.png";
		}
		else if (sp.type == 1) {
			w = new RocketLauncher(5);
			texture = "Sprites/rocketLauncher.png";
		}
		else if (sp.type == 2) {
			w = new FlameShot(100);
			texture = "Sprites/flameShot.png";
		}
		else if (sp.type == 3) {
			w = new LaserGun(10);
			texture = "Sprites/laserGun.png";
		}

		if (w) em.addWeapon(new WeaponCollectible(sp.x, sp.y, w, texture));
	}
}
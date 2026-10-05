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
	if (level == 1) level1(w);
	else if (level == 2) level2(w);
	else level3(w);
}

int EnemyHeight = 96;
int EnemyWidth = 64;
int weaponHeight = 32;

// hitbox size of each vehicle type (0=MetalSlug 1=SlugFlyer 2=SlugMariner 3=FlyingTara 4=EnemySub 5=M15Bradley 6=AmphibiousSlug)
// = picture size x 2.4 (Entity::PIXEL_SCALE) so vehicles keep the right size next to the 96px tall soldiers
// (the slug flyer picture was already 4x so it gets 0.6, the slug mariner uses the sub picture made a bit smaller)
float vehicleWidth[7]  = { 146, 190, 149, 192, 223, 192, 146 };
float vehicleHeight[7] = { 134, 125,  78,  91, 118, 187, 134 };

// small helpers so the spawn lists below stay short: put something on the ground at a column
void Level::enemyAt(int type, int col, const World& w) {
	if (eCount < 20) enemySpawn[eCount++] = { type, col * (float)World::CELL, isOnGround(w, col, EnemyHeight) };
}
void Level::vehicleAt(int type, int col, const World& w) {
	if (vCount < 20) vehicleSpawn[vCount++] = { type, col * (float)World::CELL, isOnGround(w, col, (int)vehicleHeight[type], (int)vehicleWidth[type]) };
}
void Level::weaponAt(int type, int col, const World& w) {
	if (wCount < 15) weaponSpawn[wCount++] = { type, col * (float)World::CELL, isOnGround(w, col, weaponHeight) };
}
void Level::prisonerAt(int col, const World& w) {
	if (pCount < 10) prisonerSpawn[pCount++] = { 0, col * (float)World::CELL, isOnGround(w, col, 70) };
}

// enemy types: 0 = rebel, 1 = shield soldier, 2 = bazooka, 3 = grenade soldier
// vehicles: 0 MetalSlug, 1 SlugFlyer, 2 SlugMariner, 3 FlyingTara, 4 EnemySub, 5 M15Bradley, 6 AmphibiousSlug
// weapons: 0 HeavyMachineGun, 1 RocketLauncher, 2 FlameShot, 3 LaserGun

// mission 1, mountain pass: rocky hills (0-90) then plains, goal at the far right
void Level::level1(const World& w) {
	goalColumn = 195;
	enemyAt(0, 18, w); enemyAt(3, 26, w); enemyAt(2, 34, w); enemyAt(0, 42, w); enemyAt(1, 50, w); enemyAt(0, 58, w);
	enemyAt(2, 72, w); enemyAt(3, 84, w); enemyAt(0, 100, w); enemyAt(1, 112, w); enemyAt(2, 125, w); enemyAt(0, 140, w);
	enemyAt(3, 150, w); enemyAt(1, 165, w); enemyAt(2, 178, w); enemyAt(0, 186, w);

	vehicleAt(1, 6, w);     // slug flyer near the start
	vehicleAt(0, 64, w);    // metal slug in the hills
	vehicleSpawn[vCount++] = { 3, 120 * (float)World::CELL, isOnGround(w, 120, (int)vehicleHeight[3]) - 300 }; // flying tara, hovers
	vehicleAt(5, 160, w);   // bradley tank guarding the end

	weaponAt(0, 12, w); weaponAt(1, 70, w); weaponAt(2, 130, w);
	prisonerAt(30, w); prisonerAt(95, w); prisonerAt(150, w);
}

// mission 2, coastline: plains (0-110) then the sea, goal at the end of the sea bed
void Level::level2(const World& w) {
	goalColumn = 196;
	enemyAt(0, 15, w); enemyAt(2, 22, w); enemyAt(1, 30, w); enemyAt(3, 38, w); enemyAt(0, 46, w); enemyAt(2, 55, w);
	enemyAt(3, 63, w); enemyAt(1, 72, w); enemyAt(0, 80, w); enemyAt(2, 90, w); enemyAt(3, 100, w);
	enemyAt(0, 130, w); enemyAt(2, 150, w); enemyAt(0, 170, w); enemyAt(1, 185, w);

	vehicleAt(0, 8, w);     // metal slug at the start
	vehicleAt(5, 60, w);    // bradley tank
	vehicleSpawn[vCount++] = { 3, 85 * (float)World::CELL, isOnGround(w, 85, (int)vehicleHeight[3]) - 300 };
	vehicleAt(6, 104, w);   // amphibious slug at the shore
	vehicleAt(2, 115, w);   // slug mariner on the sea bed
	vehicleSpawn[vCount++] = { 4, 140 * (float)World::CELL, (World::seaLEVEL + 2) * (float)World::CELL }; // enemy subs
	vehicleSpawn[vCount++] = { 4, 175 * (float)World::CELL, (World::seaLEVEL + 2) * (float)World::CELL };

	weaponAt(3, 50, w); weaponAt(1, 95, w);
	prisonerAt(40, w); prisonerAt(98, w); prisonerAt(160, w);
}

// mission 3, rebel base: flat desert at night, the boss waits here
void Level::level3(const World& w) {
	goalColumn = 0; // no flag, the mission ends when the boss is destroyed
	enemyAt(0, 12, w); enemyAt(1, 20, w); enemyAt(2, 26, w); enemyAt(3, 34, w);

	vehicleAt(0, 5, w);     // a metal slug to fight the boss with

	weaponAt(0, 8, w); weaponAt(1, 15, w);
	prisonerAt(10, w);
}

void Level::setUP(EntityManager& em) const {
	for (int i = 0; i < eCount;i++) {

		Enemy* e = nullptr;
		const spawn& sp = enemySpawn[i];

		if (sp.type == 0) {
			e = new RebelSoldier(sp.x, sp.y, EnemyWidth, EnemyHeight, 20);
		}
		else if (sp.type == 1) {
			e = new ShieldedSoldier(sp.x, sp.y, EnemyWidth, EnemyHeight, 40);
		}
		else if (sp.type == 2) {
			e = new BazookaSoldier(sp.x, sp.y, EnemyWidth, EnemyHeight, 20);
		}
		else if (sp.type == 3) {
			e = new GrenadeSoldier(sp.x, sp.y, EnemyWidth, EnemyHeight ,20);
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
			v = new FlyingTara(sp.x, sp.y, vw, vh, 60, 3.f, 1, sp.y);
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
			w = new RocketLauncher(10);
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
	for (int i = 0; i < pCount; i++)
		em.addPrisoner(new Prisoner(prisonerSpawn[i].x, prisonerSpawn[i].y));
}
#include "EntityManager.h"
#include "Soldier.h"
#include "Enemy.h"

//#define DEBUG_HITBOXES


EntityManager::EntityManager() {
	for (int i = 0; i < MAX_PROJECTILES; i++)
		projectiles[i] = nullptr;
	for (int i = 0; i < MAX_ENEMIES; i++)
		enemies[i] = nullptr;
	for (int i = 0; i < MAX_VEHICLES; i++)
		vehicles[i] = nullptr;
	for (int i = 0; i < MAX_COLLECTIBLES; i++)
		collectibles[i] = nullptr;

}


void EntityManager::addVehicle(Vehicle* v) {
	if (vCount >= MAX_VEHICLES) return;
	for (int i = 0; i < MAX_VEHICLES; i++) {
		if (!vehicles[i]) {
			vehicles[i] = v;
			vCount += 1;
			return;
		}
	}
}


void EntityManager::addWeapon(WeaponCollectible* wC) {
	if (wCount >= MAX_COLLECTIBLES) return;
	for (int i = 0; i < MAX_COLLECTIBLES;i++) {
		if (!collectibles[i]) {
			collectibles[i] = wC;
			wCount += 1;
			return;
		}
	}
}


void EntityManager::addExplosion(float x, float y, float height) {
	// use the first explosion that isn't playing; if all 20 are busy this one is just skipped
	for (int i = 0; i < MAX_EXPLOSIONS; i++) {
		if (!explosions[i].isActive()) {
			explosions[i].start(x, y, height);
			float shake = height / 25; // 150px explosion = 6px shake, vehicle = 10px
			if (shake > shakeRequest) shakeRequest = shake;
			return;
		}
	}
}

void EntityManager::addProjectile(Projectile* p) {
	if (pCount >= MAX_PROJECTILES) { delete p; return; }
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (!projectiles[i]) {
			projectiles[i] = p;
			pCount += 1;
			return;
		}
	}
}
void EntityManager::addEnemy(Enemy* e) {
	if (eCount >= MAX_ENEMIES) {
		return;
	}
	for (int i = 0; i < MAX_ENEMIES;i++) {
		if (!enemies[i]) {
			enemies[i] = e;
			eCount += 1;
			return;
		}
	}
}

void EntityManager::update(float frameTime, const World& w) {

	checkMeleeCollisions();
	if (player) {
		player->update(frameTime, w);

		//making the player the target for the enemies
		PlayerSoldier* active = player->getActive();
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (enemies[i]) enemies[i]->target = active;
		}
		for (int i = 0; i < MAX_VEHICLES; i++) {
			if (vehicles[i] && vehicles[i]->isEnemy()) vehicles[i]->setTarget(active);
		}
	}


	//updating enemies
	for (int i = 0; i < MAX_ENEMIES;i++) {
		if (!enemies[i]) continue;
		if (enemies[i]->getActive()) {
			enemies[i]->update(frameTime, w);

			Projectile* ep = enemies[i]->getProjectile();
			if (ep) addProjectile(ep);
		}


	
	}

	//updating vehicles
	for (int i = 0; i < MAX_VEHICLES;i++) {
		if (vehicles[i] && vehicles[i]->getActive()) {
			vehicles[i]->update(frameTime, w);

			Projectile* vp = vehicles[i]->getProjectile();
			if (vp) addProjectile(vp);
		}
	}

	//updating weapons collectibles
	for (int i = 0; i < MAX_COLLECTIBLES;i++) {
		if (collectibles[i]) collectibles[i]->update();
	}

	//projectiles
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (!projectiles[i]) continue;

		if (projectiles[i]->getActive()) {
			projectiles[i]->update(frameTime, w);
		}
	}

	checkVehicleEntry();
	if (player && player->isPiloting() && !player->getVehicle()->isVehicleOccupied())
		player->dismountVehicle();

	checkGrenadeBlast();
	checkProjectileCollisions();
	checkEnemyPlayerCollisions();
	checkCollectiblesCollisions();
	checkEnemyProjectilePlayerCollisions();

	//updating enemies
	for (int i = 0; i < MAX_ENEMIES;i++) {
		if (enemies[i] && !enemies[i]->getActive()) {
			delete enemies[i];
			enemies[i] = nullptr;
			eCount -= 1;
		}
	}

	//vehicels
	for (int i = 0; i < MAX_VEHICLES;i++) {
		if (vehicles[i] && !vehicles[i]->getActive()) {
			// a destroyed vehicle throws the pilot out before it is deleted
			if (player && player->getVehicle() == vehicles[i])
				player->dismountVehicle();
			// big explosion where the vehicle was
			addExplosion(vehicles[i]->getX() + vehicles[i]->getWidth() / 2.f, vehicles[i]->getY() + vehicles[i]->getHeight(), 250);
			delete vehicles[i];
			vehicles[i] = nullptr;
			vCount -= 1;
		}
	}

	//projectiles
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (projectiles[i] && !projectiles[i]->getActive()) {
			// grenades and rockets (anything with a blast radius) explode when they are used up
			if (projectiles[i]->getBlastRadius() > 0)
				addExplosion(projectiles[i]->getX() + projectiles[i]->getWidth() / 2.f, projectiles[i]->getY() + projectiles[i]->getHeight(), 150);
			delete projectiles[i];
			projectiles[i] = nullptr;
			pCount -= 1;
		}
	}

	for (int i = 0; i < MAX_EXPLOSIONS; i++)
		explosions[i].update(frameTime);

	if (coolDown > 0) {
		coolDown -= frameTime;
	}
}

void EntityManager::render(RenderWindow& w, const Camera& cam) {
	for (int i = 0; i < MAX_ENEMIES; i++)
		if (enemies[i] && enemies[i]->getActive())
			enemies[i]->render(w, cam);

#ifdef DEBUG_HITBOXES
		sf::RectangleShape box;
		box.setFillColor(sf::Color::Transparent);
		box.setOutlineColor(sf::Color::Red);
		box.setOutlineThickness(1.f);
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (!enemies[i] || !enemies[i]->getActive()) continue;
			box.setSize(sf::Vector2f(enemies[i]->getWidth(), enemies[i]->getHeight()));
			box.setPosition(cam.toScreenX(enemies[i]->getX()), cam.toScreenY(enemies[i]->getY()));
			w.draw(box);
		}
#endif


	for (int i = 0; i < MAX_VEHICLES;i++) {
		if (vehicles[i] && vehicles[i]->getActive()) {
			vehicles[i]->render(w, cam);
		}
	}

	for (int i = 0; i < MAX_COLLECTIBLES;i++) {
		if (collectibles[i]) {
			collectibles[i]->render(w, cam);
		}
	}




	if (player) {
		player->render(w, cam);

		for (int i = 0; i < MAX_PROJECTILES; i++) {
			if (projectiles[i] && projectiles[i]->getActive())
				projectiles[i]->render(w, cam);
		}
	}

	// explosions last so they are on top of everything
	for (int i = 0; i < MAX_EXPLOSIONS; i++)
		explosions[i].render(w, cam);
}

void EntityManager::checkProjectileWorldCollisions(World& w) {
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (!projectiles[i] || !projectiles[i]->getActive()) {
			continue;
		}

		float midX = projectiles[i]->getX() + projectiles[i]->getWidth() / 2.f;
		float midY = projectiles[i]->getY() + projectiles[i]->getHeight() / 2.f;

		if (w.isSolid(midX, midY))
			projectiles[i]->setActive(false);
	}
}

void EntityManager::checkProjectileCollisions() {
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (!projectiles[i] || !projectiles[i]->getActive() || !projectiles[i]->isFromPlayer()) continue;

		for (int j = 0; j < MAX_ENEMIES;j++) {
			if (!enemies[j] || !enemies[j]->getActive()) {
				continue;
			}

			if (projectiles[i]->collision(*enemies[j])) {
				enemies[j]->onHitByProjectile(projectiles[i]);
				if (!projectiles[i]->getActive()) break;
			}

		}

		for (int j = 0; j < MAX_VEHICLES && projectiles[i]->getActive(); j++) {
			if (!vehicles[j] || !vehicles[j]->getActive() || !vehicles[j]->isEnemy()) continue;

			if (overlaps(*projectiles[i], *vehicles[j])) {
				projectiles[i]->applyDamage(vehicles[j]);
				if (projectiles[i]->diesOnHit()) projectiles[i]->setActive(false);
			}
		}


	}
}


void EntityManager::checkEnemyProjectilePlayerCollisions() {
	if (!player) return;
	// while piloting, the vehicle takes the hits instead of the soldier
	DamagableEntity* curr = player->isPiloting() ? (DamagableEntity*)player->getVehicle() : (DamagableEntity*)player->getActive();
	if (!curr || !curr->getActive()) return;

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (!projectiles[i] || !projectiles[i]->getActive() || projectiles[i]->isFromPlayer()) continue;

		if (overlaps(*projectiles[i], *curr)) {
			projectiles[i]->applyDamage(curr);
			if (projectiles[i]->diesOnHit()) projectiles[i]->setActive(false);
		}
	}
}

bool EntityManager::overlaps(const Entity& a, const Entity& b) {
	return a.getX() < b.getX() + b.getWidth() && b.getX() < a.getX() + a.getWidth() &&
		a.getY() < b.getY() + b.getHeight() && b.getY() < a.getY() + a.getHeight();
}


void EntityManager::checkVehicleEntry() {
	if (!player || player->isPiloting()) return;

	PlayerSoldier* curr = player->getActive();
	if (!curr) return;

	for (int i = 0; i < MAX_VEHICLES; i++) {
		if (!vehicles[i] || !vehicles[i]->getActive() || vehicles[i]->isVehicleOccupied() || vehicles[i]->isEnemy()) continue;

		if (curr->collision(*vehicles[i]) && Keyboard::isKeyPressed(Keyboard::E)) {
			vehicles[i]->enterVehicle(curr);
			player->mountVehicle(vehicles[i]);
			return;
		}
	}
}



void EntityManager::checkGrenadeBlast() {
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (!projectiles[i] || !projectiles[i]->didExplode() || projectiles[i]->getBlastRadius() <= 0) {
			continue;
		}

		//blast locaiton
		float bx = projectiles[i]->getX() + projectiles[i]->getWidth() / 2.f;
		float by = projectiles[i]->getY() + projectiles[i]->getHeight() / 2.f;
		float bradius = projectiles[i]->getBlastRadius();
		int   dmg = projectiles[i]->getDamage();

		// a grenade only hurts the other side
		if (!projectiles[i]->isFromPlayer()) {
			if (player && !player->isPiloting()) {
				PlayerSoldier* currCharacter = player->getActive();
				float sx = currCharacter->getX() + currCharacter->getWidth() / 2.f;
				float sy = currCharacter->getY() + currCharacter->getHeight() / 2.f;
				if ((bx - sx) * (bx - sx) + (by - sy) * (by - sy) < bradius * bradius)
					currCharacter->takeDamage(dmg / 2);
			}
			else if (player && player->getVehicle()) {
				Vehicle* v = player->getVehicle();
				float sx = v->getX() + v->getWidth() / 2.f;
				float sy = v->getY() + v->getHeight() / 2.f;
				if ((bx - sx) * (bx - sx) + (by - sy) * (by - sy) < bradius * bradius)
					v->takeDamage(dmg / 2);
			}
			continue;
		}

		for (int j = 0; j < MAX_VEHICLES; j++) {
			if (!vehicles[j] || !vehicles[j]->getActive() || !vehicles[j]->isEnemy()) continue;
			float vx = vehicles[j]->getX() + vehicles[j]->getWidth() / 2.f;
			float vy = vehicles[j]->getY() + vehicles[j]->getHeight() / 2.f;
			if ((bx - vx) * (bx - vx) + (by - vy) * (by - vy) < bradius * bradius)
				vehicles[j]->takeDamage(dmg);
		}

		for (int j = 0; j < MAX_ENEMIES;j++) {
			if (!enemies[j] || !enemies[j]->getActive() || !enemies[j]->isAlive()) {
				continue;
			}

			//finding the distance from the bomb
			float ex = enemies[j]->getX() + enemies[j]->getWidth() / 2.f;
			float ey = enemies[j]->getY() + enemies[j]->getHeight() / 2.f;

			float diffX = bx - ex;
			float diffY = by - ey;

			float modSquared = (diffX * diffX) + (diffY * diffY);

			if (modSquared < (bradius * bradius)) {
				enemies[j]->onHitByProjectile(projectiles[i]);
			}

		}
	}
}

void EntityManager::checkMeleeCollisions() {
	if (!player) return;
	PlayerSoldier* currCharacter = player->getActive();
	if (!currCharacter || !currCharacter->isMeleeActive())
	{
		return;
	}
	for (int j = 0; j < MAX_ENEMIES;j++) {
		if (!enemies[j] || !enemies[j]->getActive()) {
			continue;
		}
		if (currCharacter->checkMeleeCollision(*enemies[j])) {
			enemies[j]->takeDamage(50); // knife
		}
	}
}

void EntityManager::checkEnemyPlayerCollisions() {
	if (coolDown > 0) return;
	if (!player) return;

	PlayerSoldier* currCharacter = player->getActive();
	if (!currCharacter || !currCharacter->getActive()) return;

	for (int j = 0; j < MAX_ENEMIES; j++) {
		if (!enemies[j] || !enemies[j]->getActive()) continue;

		if (currCharacter->collision(*enemies[j])) {
			//int type = enemies[j]->getEnemyType();
			//if (type == 1)
			//	currCharacter->infect(1);
			//else if (type == 2)
			//	currCharacter->infect(2);
			//else
			currCharacter->takeDamage(enemies[j]->getDamage());
			coolDown = 1.f;
		}
	}
	
}


void EntityManager::checkCollectiblesCollisions() {
	if (!player) return;
	PlayerSoldier* currCHARACTER = player->getActive();
	if (!currCHARACTER) return; //no one alive

	for (int i = 0; i < MAX_COLLECTIBLES; i++) {
		if (!collectibles[i] || collectibles[i]->isCollected()) {
			continue;
		}
		if (collectibles[i]->collision(currCHARACTER->getX(), currCHARACTER->getY(), currCHARACTER->getWidth(), currCHARACTER->getHeight())) {
			collectibles[i]->collect(currCHARACTER);
		}
	}

}



EntityManager::~EntityManager() {
	for (int i = 0; i < MAX_PROJECTILES; i++)
	{
		delete projectiles[i];
		projectiles[i] = nullptr;
	}
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		delete enemies[i];
		enemies[i] = nullptr;
	}
	for (int i = 0; i < MAX_VEHICLES; i++) {
		delete vehicles[i];
		vehicles[i] = nullptr;
	}

	for (int i = 0; i < MAX_COLLECTIBLES;i++) {
		delete collectibles[i];
		collectibles[i] = nullptr;
	}
}
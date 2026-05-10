#include "EntityManager.h"
#include "Soldier.h"
#include "Enemy.h"

#define DEBUG_HITBOXES


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


void EntityManager::addProjectile(Projectile* p) {
	if (pCount >= MAX_PROJECTILES) return;
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
			delete vehicles[i];
			vehicles[i] = nullptr;
			vCount -= 1;
		}
	}

	//projectiles
	for (int i = 0; i < MAX_PROJECTILES;i++) {
		if (projectiles[i] && !projectiles[i]->getActive()) {
			delete projectiles[i];
			projectiles[i] = nullptr;
			pCount -= 1;
		}
	}

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
	if (coolDown > 0) return;

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


	}
}


void EntityManager::checkEnemyProjectilePlayerCollisions() {
	if (!player) return;
	PlayerSoldier* curr = player->getActive();
	if (!curr || !curr->getActive()) return;

	float currx = curr->getX(), curry = curr->getY();
	float currw = curr->getWidth(), currh = curr->getHeight();

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (!projectiles[i] || !projectiles[i]->getActive() || projectiles[i]->isFromPlayer()) continue;

		float px = projectiles[i]->getX(), py = projectiles[i]->getY();
		float pw = projectiles[i]->getWidth(), ph = projectiles[i]->getHeight();


		bool touchX = abs(px - currx)
			< ((pw / 2.f + currw / 2.f));
		bool touchY = abs(py - curry)
			< ((ph / 2.f + currh / 2.f));

		bool collision=touchX && touchY;


		if (collision) {
			projectiles[i]->applyDamage(curr);
			if (projectiles[i]->diesOnHit()) projectiles[i]->setActive(false);
		}
	}
}


void EntityManager::checkVehicleEntry() {
	if (!player || player->isPiloting()) return;

	PlayerSoldier* curr = player->getActive();
	if (!curr) return;

	for (int i = 0; i < MAX_VEHICLES; i++) {
		if (!vehicles[i] || !vehicles[i]->getActive() || vehicles[i]->isVehicleOccupied()) continue;

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

		//check for the soldiers too
		if (player) {
			PlayerSoldier* currCharacter = player->getActive();

			//finding the distance from the bomb
			float sx = currCharacter->getX() + currCharacter->getWidth() / 2.f;
			float sy = currCharacter->getY() + currCharacter->getHeight() / 2.f;

			float diffX = bx - sx;
			float diffY = by - sy;

			float modSquared = (diffX * diffX) + (diffY * diffY);

			if (modSquared < (bradius * bradius)) {
				currCharacter->takeDamage(dmg / 2);
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
			enemies[j]->takeDamage(30);
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
		collectibles[i] = nullptr;
	}
}
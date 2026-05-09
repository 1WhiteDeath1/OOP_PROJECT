#include "Player.h"
#include "EntityManager.h"
void Player::switchCharacter() {
	int attempts = 0;
	do {
		activeIndex = (activeIndex + 1) % 4;
		attempts += 1;
	} while (characters[activeIndex]->getIsDead() && attempts < 4);
}

void Player::update(float frameTime, const World& w) {
	if (isPiloting()) {
		vehicle->update(frameTime, w);
	}
	else {
		characters[activeIndex]->update(frameTime, w);
	}
}


void Player::handleInput(float frameTime, const World& w, EntityManager& eManager) {
	if (Keyboard::isKeyPressed(Keyboard::Z)) {
		switchCharacter();
	}

	PlayerSoldier* curr = characters[activeIndex];

	if (isPiloting()) {
		vehicle->handleInput(frameTime);
	}
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			curr->moveLeft(frameTime);
			curr->setFacing(false);
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			curr->moveRight(frameTime);
			curr->setFacing(true);
		}
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			curr->jump();
		}

		curr->crouch(Keyboard::isKeyPressed(Keyboard::S));


		//firing
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			Projectile* p=curr->fire();
			if (p) eManager.addProjectile(p);
		}
		//gernading
		if (Keyboard::isKeyPressed(Keyboard::E)) {
			Projectile* g = curr->throwGrenade();
			if (g) eManager.addProjectile(g);
		}
		//meleeing
		if (Keyboard::isKeyPressed(Keyboard::R)) {
			curr->meleeAttack();
		}

		//power up
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			curr->activePowerUp();
		}

	}

void Player::render(RenderWindow& window, const Camera& cam) {
	if (isPiloting()) {
		vehicle->render(window, cam);
	}

	characters[activeIndex]->render(window, cam);
}

void Player::mountVehicle(PlayerVehicle* v) {
	vehicle = v;
	characters[activeIndex]->setPiloting(true);
}

void Player::dismountVehicle() {
	if (vehicle) {
		characters[activeIndex]->setPiloting(false);
	}
}

bool Player::allDead() const {
	for (int i = 0; i < 4; i++)
		if (!characters[i]->getIsDead()) return false;
	return true;
}
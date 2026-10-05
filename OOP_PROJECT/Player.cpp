#include "Player.h"
#include "EntityManager.h"
void Player::switchCharacter() {

	float currentX = characters[activeIndex]->getX();
	float currentY = characters[activeIndex]->getY();
	int attempts = 0;
	do {
		activeIndex = (activeIndex + 1) % 4;
		attempts += 1;
	} while (characters[activeIndex]->getIsDead() && attempts < 4);

	characters[activeIndex]->setPosition(currentX, currentY);
}

void Player::update(float frameTime, const World& w) {
	// vehicles are updated by the EntityManager
	if (!isPiloting()) {
		characters[activeIndex]->update(frameTime, w);
		if (characters[activeIndex]->getIsDead() && !allDead())
			switchCharacter();
	}
}


void Player::handleInput(float frameTime, const World& w, EntityManager& eManager) {
	bool switchKey = Keyboard::isKeyPressed(Keyboard::Z);
	if (switchKey && !switchHeld && !isPiloting()) {
		switchCharacter();
	}
	switchHeld = switchKey;

	PlayerSoldier* curr = characters[activeIndex];

	if (isPiloting()) {
		// vehicles read their own controls in their update, U gets out of any of them
		if (Keyboard::isKeyPressed(Keyboard::U))
			vehicle->exitVehicle();
	}
	else {
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
		//chaning the angle brother afsd fa
		if (Keyboard::isKeyPressed(Keyboard::Up))
			curr->changeAngle(20.f * frameTime); 

		if (Keyboard::isKeyPressed(Keyboard::Down))
			curr->changeAngle(-20.f * frameTime);

		//firing
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			Projectile* p = curr->fire();
			if (p) {
				eManager.addProjectile(p);
				// Tarma's spread shot adds two more bullets with every shot
				Projectile* extra[2];
				curr->spreadShots(extra);
				for (int i = 0; i < 2; i++) if (extra[i]) eManager.addProjectile(extra[i]);
			}
		}
		//gernading
		bool grenadeKey = Keyboard::isKeyPressed(Keyboard::T);
		if (grenadeKey && !grenadeHeld) {
			Projectile* g = curr->throwGrenade();
			if (g) eManager.addProjectile(g);
		}
		grenadeHeld = grenadeKey;
		//meleeing
		bool meleeKey = Keyboard::isKeyPressed(Keyboard::R);
		if (meleeKey && !meleeHeld) {
			curr->meleeAttack();
		}
		meleeHeld = meleeKey;

		//power up
		bool powerKey = Keyboard::isKeyPressed(Keyboard::Q);
		if (powerKey && !powerHeld) {
			curr->activePowerUp();
		}
		powerHeld = powerKey;
	}

}

void Player::render(RenderWindow& window, const Camera& cam) {
	// while piloting, the vehicle is drawn by the EntityManager
	if (!isPiloting()) {
		characters[activeIndex]->render(window, cam);
	}
}

void Player::mountVehicle(Vehicle* v) {
	vehicle = v;
	characters[activeIndex]->setPiloting(true);
}

void Player::dismountVehicle() {
	if (vehicle) {
		vehicle->exitVehicle();
		vehicle = nullptr; // still owned by the EntityManager, so it is not deleted here
		characters[activeIndex]->setPiloting(false);
	}
}

bool Player::allDead() const {
	for (int i = 0; i < 4; i++)
		if (!characters[i]->getIsDead()) return false;
	return true;
}
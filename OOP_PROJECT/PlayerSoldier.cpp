#include "PlayerSoldier.h"


void PlayerSoldier::update(float frameTime, const World& w) {
	if (currState->getType() != 0) {
		stateTimer -= frameTime;
		if (stateTimer <= 0) {
			delete currState;
			currState = new NormalState();
			stateTimer = 0;
		}
	}

	walkSpeed = 300 * speedMultiplier * currState->getSpeedMultiplier();

	if (powerUPActive) {
		powerUPTimer -= frameTime;
		if (powerUPTimer <= 0) {
			powerUPActive = false;
		}
	}

	movement(frameTime, w);
}

void PlayerSoldier::infect(int type) {
	if(currState->getType() == type) return;

	delete currState;
	stateTimer = 10;

	if (type == 1) {
		currState = new UndeadState();
	}
	else if (type == 2) {
		currState = new MummyState();
	}
}

void PlayerSoldier::activePowerUp() {
	if (powerUPActive) return;

	powerUPActive = true;

	if (characterType == 0) powerUPTimer = 10.f;
	else if (characterType == 1) powerUPTimer = 20.f;
	else if (characterType == 2) powerUPTimer = 10.f;
	else if (characterType == 3) powerUPTimer = 10.f;
}

Projectile* PlayerSoldier::fire() {
	if (!canShoot()) return nullptr;
	return fireWeapon();
}

const char* PlayerSoldier::getName() const
{
	if (characterType == 0) return "Marco";
	if (characterType == 1) return "Tarma";
	if (characterType == 2) return "Eri";
	if (characterType == 3) return "Fio";
	return "Unknown";
}

void PlayerSoldier::render(sf::RenderWindow& window, Camera& cam)
{
	window.draw(sprite);
}

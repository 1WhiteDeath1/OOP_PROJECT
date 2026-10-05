#include "PlayerSoldier.h"
#include "SoundManager.h"


void PlayerSoldier::update(float frameTime, const World& w) {
	meleeActive = false;
	if (currState->getType() != 0) {
		stateTimer -= frameTime;
		if (stateTimer <= 0) {
			delete currState;
			currState = new NormalState();
			stateTimer = 0;
		}
	}

	walkSpeed = 600 * speedMultiplier * currState->getSpeedMultiplier();

	if (powerUPActive) {
		powerUPTimer -= frameTime;
		if (powerUPTimer <= 0) {
			powerUPActive = false;
		}
	}

	movement(frameTime, w);

	//for weapon to cooldown, and keep updateing (faster cooldown = higher fire rate, power up doubles it)
	float fireRate = fireRateMultiplier * (powerUPActive ? 2.f : 1.f);
	for (int i = 0; i < 2; i++)
		if (inventory[i]) inventory[i]->update(frameTime * fireRate);
}

void PlayerSoldier::infect(int type) {
	if (currState->getType() == type) return;

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

void PlayerSoldier::die() {
	// use up a life and get back up, the character is only out once all lives are gone
	if (currLives > 0) {
		respawn(x, y);
		return;
	}
	isDead = true;
	isActive = false;
}

Projectile* PlayerSoldier::fire() {
	if (!canShoot()) return nullptr;
	Projectile* p = fireWeapon();
	if (p) {
		muzzleFrames = 3; // a shot was fired: show the flash for 3 frames
		SoundManager::play(p->getBlastRadius() > 0 ? SoundManager::ROCKET : SoundManager::SHOOT, 60);
	}
	return p;
}

const char* PlayerSoldier::getName() const
{
	if (characterType == 0) return "Marco";
	if (characterType == 1) return "Tarma";
	if (characterType == 2) return "Eri";
	if (characterType == 3) return "Fio";
	return "Unknown";
}

void PlayerSoldier::render(sf::RenderWindow& window, const Camera& cam)
{
	// the pictures face right, so flip them when walking left; bounce while running
	float lift, lean;
	walkBounce(touchingGround, lift, lean);
	drawSprite(window, cam, drawScale, !facingRight, lift, lean);
	drawMuzzleFlash(window, cam, facingRight ? x + width + 8 : x - 8, y + 56); // at the gun barrel
}

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
			powerUPCooldown = 20; // ready again in 20 seconds
		}
	}
	else if (powerUPCooldown > 0) powerUPCooldown -= frameTime;

	movement(frameTime, w);

	//for weapon to cooldown, and keep updateing (faster cooldown = higher fire rate, Marco's power up doubles it)
	float fireRate = fireRateMultiplier * ((powerUPActive && characterType == 0) ? 2.f : 1.f);
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
	if (powerUPActive || powerUPCooldown > 0) return;

	// every character has their own power up (see getPowerUpName), each lasts a few seconds
	// and can be used again 20 seconds after it ends
	powerUPActive = true;
	if (characterType == 0) powerUPTimer = 10.f;      // Marco: double fire rate
	else if (characterType == 1) powerUPTimer = 10.f; // Tarma: 3-way spread shot
	else if (characterType == 2) powerUPTimer = 8.f;  // Eri: unlimited grenades
	else powerUPTimer = 6.f;                          // Fio: shield, no damage
	SoundManager::play(SoundManager::PICKUP);
}

const char* PlayerSoldier::getPowerUpName() const {
	if (characterType == 0) return "RAPID FIRE";
	if (characterType == 1) return "SPREAD SHOT";
	if (characterType == 2) return "GRENADE STORM";
	return "SHIELD";
}

void PlayerSoldier::takeDamage(int amount) {
	if (powerUPActive && characterType == 3) return; // Fio's shield is up
	Soldier::takeDamage(amount);
}

void PlayerSoldier::spreadShots(Projectile* extra[2]) {
	extra[0] = extra[1] = nullptr;
	if (!powerUPActive || characterType != 1) return;
	float fireX = facingRight ? (x + width) : x;
	float fireY = y + height / 2;
	for (int i = 0; i < 2; i++) {
		float dx, dy;
		ProjectileWeapon::getDirection(aimAngle + (i == 0 ? 15.f : -15.f), facingRight, dx, dy);
		extra[i] = new Bullet(fireX, fireY, dx, dy, true, 10);
	}
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

	// Fio's shield: a see-through blue bubble around her while it is up
	if (powerUPActive && characterType == 3) {
		CircleShape bubble(70.f);
		bubble.setOrigin(70.f, 70.f);
		bubble.setPosition(cam.toScreenX(x + width / 2), cam.toScreenY(y + height / 2));
		bubble.setFillColor(Color(80, 160, 255, 60));
		bubble.setOutlineColor(Color(140, 200, 255, 200));
		bubble.setOutlineThickness(3);
		window.draw(bubble);
	}
	drawMuzzleFlash(window, cam, facingRight ? x + width + 8 : x - 8, y + 56); // at the gun barrel
}

#include "PlayerSoldier.h"


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

	//for weapon to cooldown, and keep updateing
	for (int i = 0; i < 2; i++)
		if (inventory[i]) inventory[i]->update(frameTime);
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

void PlayerSoldier::render(sf::RenderWindow& window, const Camera& cam)
{
	if (facingRight) {
		sprite.setScale(( width/ texture.getSize().x), height / texture.getSize().y);
		sprite.setPosition(cam.toScreenX(x), cam.toScreenY(y));
	}
	else {
		
		sprite.setScale((-width / texture.getSize().x), height / texture.getSize().y);
		sprite.setPosition(cam.toScreenX(x) + width, cam.toScreenY(y));
	}
	window.draw(sprite);

	// aim direction dots
	float gunX = cam.toScreenX(facingRight ? x + width : x);
	float gunY = cam.toScreenY(y + height / 3.f);
	float rad = aimAngle * 3.14159f / 180.f;
	float dx = std::cos(rad) * (facingRight ? 1.f : -1.f);
	float dy = -std::sin(rad);

	for (int i = 1; i <= 6; i++) {
		sf::CircleShape dot(3.f);
		dot.setPosition(gunX + dx * i * 12.f, gunY + dy * i * 12.f);
		dot.setFillColor(sf::Color::Yellow);
		window.draw(dot);
	}

	// DEBUG: show hitbox
		sf::RectangleShape box(sf::Vector2f(width, height));
	box.setPosition(cam.toScreenX(x), cam.toScreenY(y));
	box.setFillColor(sf::Color::Transparent);
	box.setOutlineColor(sf::Color::Red);
	box.setOutlineThickness(2.f);
	window.draw(box);
}

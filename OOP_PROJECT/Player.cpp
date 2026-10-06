#include "Player.h"
#include "EntityManager.h"
#include "Aim.h"
#include <cmath>
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


void Player::handleInput(float frameTime, const World& w, EntityManager& eManager, const Camera& cam) {
	bool switchKey = Keyboard::isKeyPressed(Keyboard::Z);
	if (switchKey && !switchHeld && !isPiloting()) {
		switchCharacter();
	}
	switchHeld = switchKey;

	PlayerSoldier* curr = characters[activeIndex];

	// with the mouse on, the gun points at the mouse and the soldier (or vehicle) faces that way
	curr->setMouseAiming(Aim::usingMouse());
	if (Aim::usingMouse()) aimWithMouse(cam);

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
		// aiming with the arrows: up/down turn the gun (120 degrees a second, so all the way up takes
		// under half a second), left/right turn around to shoot the other way without walking
		if (!Aim::usingMouse()) {
			if (Keyboard::isKeyPressed(Keyboard::Up))
				curr->changeAngle(120.f * frameTime);
			if (Keyboard::isKeyPressed(Keyboard::Down))
				curr->changeAngle(-120.f * frameTime);
			if (Keyboard::isKeyPressed(Keyboard::Left))
				curr->setFacing(false);
			if (Keyboard::isKeyPressed(Keyboard::Right))
				curr->setFacing(true);
		}

		//firing (space, or the left mouse button while aiming with the mouse)
		if (Aim::firePressed()) {
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

void Player::getGunPosition(float& gx, float& gy) const {
	if (isPiloting()) {
		vehicle->getGunPosition(gx, gy);
		return;
	}
	// same spot Soldier::fireWeapon shoots from
	const PlayerSoldier* curr = characters[activeIndex];
	gx = curr->isFacingRight() ? curr->getX() + curr->getWidth() : curr->getX();
	gy = curr->getY() + curr->getHeight() / 2;
}

void Player::aimWithMouse(const Camera& cam) {
	PlayerSoldier* curr = characters[activeIndex];
	// the mouse in world coordinates
	float mx = Aim::mouse.x + cam.x;
	float my = Aim::mouse.y + cam.y;

	// face the mouse: compare with the middle of the soldier or of the vehicle being driven
	const Entity* body = isPiloting() ? (const Entity*)vehicle : (const Entity*)curr;
	bool right = mx >= body->getX() + body->getWidth() / 2;
	curr->aimAt(right, curr->getAimAngle());

	// angle from the gun to the mouse, up is positive (screen y grows downwards): -90 straight down to
	// 90 straight up, so with the facing the mouse aims all the way round. forward is how far in front of
	// the gun the mouse is; when it is right above or below the soldier that is 0, so straight up / down
	float gx, gy;
	getGunPosition(gx, gy);
	float forward = right ? mx - gx : gx - mx;
	if (forward < 0) forward = 0;
	float angle = std::atan2(gy - my, forward) * 180.f / 3.14159f;
	curr->aimAt(right, angle);
}

void Player::renderAim(RenderWindow& window, const Camera& cam, const World& w) const {
	if (allDead()) return;
	const PlayerSoldier* curr = characters[activeIndex];
	float gx, gy, dx, dy;
	getGunPosition(gx, gy);
	ProjectileWeapon::getDirection(curr->getAimAngle(), curr->isFacingRight(), dx, dy);

	// dots every 26px along the line of fire, fading out, stopping at the first solid block
	const int dots = 16;
	const float gap = 26;
	float endX = gx, endY = gy;
	CircleShape dot(3.f);
	dot.setOrigin(3.f, 3.f);
	for (int i = 1; i <= dots; i++) {
		float px = gx + dx * gap * i;
		float py = gy + dy * gap * i;
		if (w.isSolid(px, py)) break;
		endX = px; endY = py;
		Uint8 a = (Uint8)(230 - i * 10);
		dot.setFillColor(Color(255, 240, 120, a));
		dot.setOutlineColor(Color(0, 0, 0, a));
		dot.setOutlineThickness(1);
		dot.setPosition(cam.toScreenX(px), cam.toScreenY(py));
		window.draw(dot);
	}

	// a small target ring with a cross where the line ends
	float sx = cam.toScreenX(endX), sy = cam.toScreenY(endY);
	CircleShape ring(10.f);
	ring.setOrigin(10.f, 10.f);
	ring.setPosition(sx, sy);
	ring.setFillColor(Color::Transparent);
	ring.setOutlineColor(Color(255, 80, 60, 220));
	ring.setOutlineThickness(2);
	window.draw(ring);
	RectangleShape bar(Vector2f(24, 2));
	bar.setOrigin(12, 1);
	bar.setFillColor(Color(255, 80, 60, 220));
	bar.setPosition(sx, sy);
	window.draw(bar);
	bar.setRotation(90);
	window.draw(bar);
}

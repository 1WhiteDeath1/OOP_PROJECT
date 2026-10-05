#include "Boss.h"
#include "WeaponsEach.h"
#include "SoundManager.h"
#include <cstdlib>

Texture BossTank::driveTex;
Texture BossTank::shootTex;
Texture BossTank::wreckTex;
bool BossTank::texIsLoaded = false;

// 1000 HP, touching it does 30 damage, enemy type 4 (worth 5000 points), no AI state: it has its own update
BossTank::BossTank(float x, float y) : Enemy(x, y, BOSS_W, BOSS_H, 1000, 4, 30, 1, nullptr) {
	if (!texIsLoaded) {
		driveTex.loadFromFile("25I-0504_25I-0644_Assets/boss_drive.png");
		shootTex.loadFromFile("25I-0504_25I-0644_Assets/boss_shoot.png");
		wreckTex.loadFromFile("25I-0504_25I-0644_Assets/boss_wreck.png");
		texIsLoaded = true;
	}
	sprite.setTexture(driveTex);
	drawScale = 3.4f; // a lot bigger than everything else
}

void BossTank::throwProjectile() {
	if (projectile || !target) return; // only one new projectile per frame
	float dx, dy;
	aimAtTarget(dx, dy);
	bool facingLeft = target->getX() < x;
	projectile = new Rocket(facingLeft ? x + 10 : x + width - 30, y + 45, dx, dy, false, 20);
	shootAnim = 0.3f;
}

void BossTank::die() {
	// no knock back for a tank: it stops and blows up for 2.5 seconds
	dying = true;
	deathTimer = 2.5f;
	velocityX = 0;
	SoundManager::play(SoundManager::EXPLOSION);
}

bool BossTank::takeExplosion(float& ex, float& ey) {
	// while dying, an explosion somewhere on the tank every 0.2 seconds
	if (!dying || blastTimer > 0) return false;
	blastTimer = 0.2f;
	ex = x + rand() % (int)width;
	ey = y + height - rand() % (int)(height / 2);
	return true;
}

void BossTank::update(float dt, const World& w) {
	if (dying) {
		deathTimer -= dt;
		blastTimer -= dt;
		applyGravity(dt);
		changeXandY(dt, w);
		if (deathTimer <= 0) isActive = false;
		return;
	}
	if (!target) return;

	float px = target->getX();
	float distance = px > x ? px - x : x - px;
	if (!roared && distance < 1100) {
		roared = true;
		SoundManager::play(SoundManager::BOSS);
	}

	// timers for the three attacks
	cannonTimer -= dt; mortarTimer -= dt; chargeTimer -= dt; stepTimer -= dt;
	if (shootAnim > 0) shootAnim -= dt;
	bool angry = currentHp < maxHp / 2; // below half health it gets faster and starts charging

	if (cannonTimer <= 0) { burstLeft = 3; cannonTimer = angry ? 2.5f : 3.5f; }
	if (mortarTimer <= 0) { mortarLeft = 4; mortarTimer = angry ? 5.f : 7.f; }
	if (angry && chargeTimer <= 0 && chargeTime <= 0) { chargeTime = 1.4f; chargeTimer = 8; }

	// fire the shells / grenades of a burst one at a time
	if (stepTimer <= 0 && burstLeft > 0) {
		throwProjectile();
		burstLeft--;
		stepTimer = 0.35f;
	}
	else if (stepTimer <= 0 && mortarLeft > 0 && !projectile) {
		// lob a grenade towards the player, each one a different distance
		float dir = px < x ? -1.f : 1.f;
		float reach = 0.25f + 0.12f * mortarLeft;
		projectile = new NormalGrenade(x + width / 2, y + 20, dir * reach, -1.1f, false);
		mortarLeft--;
		stepTimer = 0.25f;
		shootAnim = 0.3f;
	}

	// movement: charge at the player, or keep about 480px away from them
	if (chargeTime > 0) {
		chargeTime -= dt;
		velocityX = px < x ? -380.f : 380.f;
	}
	else {
		float want = (x > px) ? px + 480 : px - 480 - width;
		float diff = want - x;
		float speed = angry ? 160.f : 110.f;
		if (distance > 900) speed = 240; // far away: drive over quickly
		if (diff > 20) velocityX = speed;
		else if (diff < -20) velocityX = -speed;
		else velocityX = 0;
	}

	float wantedVelocity = velocityX;
	applyGravity(dt);
	changeXandY(dt, w);

	// a bump in the sand stopped it (the wall check set the speed to 0): hop over it next frame
	if (wantedVelocity != 0 && velocityX == 0 && touchingGround) velocityY = -420;
}

void BossTank::render(RenderWindow& w, const Camera& cam) {
	bool mirrored = target != nullptr && target->getX() > x; // the pictures face left

	if (dying) {
		sprite.setTexture(wreckTex, true);
		if ((int)(deathTimer * 10) % 2 == 0) hitFrames = 1; // flash while burning
		drawSprite(w, cam, drawScale, mirrored);
		return;
	}
	if (shootAnim > 0) {
		sprite.setTexture(shootTex, true);
		int fw = shootTex.getSize().x / SHOOT_FRAMES;
		int frame = (int)((0.3f - shootAnim) / 0.3f * SHOOT_FRAMES);
		if (frame >= SHOOT_FRAMES) frame = SHOOT_FRAMES - 1;
		sprite.setTextureRect(IntRect(frame * fw, 0, fw, shootTex.getSize().y));
	}
	else {
		// tracks roll while it moves: next frame every 10px
		sprite.setTexture(driveTex, true);
		int fw = driveTex.getSize().x / DRIVE_FRAMES;
		int frame = velocityX != 0 ? (int)(std::abs(x) / 10) % DRIVE_FRAMES : 0;
		sprite.setTextureRect(IntRect(frame * fw, 0, fw, driveTex.getSize().y));
	}
	drawSprite(w, cam, drawScale, mirrored);
}

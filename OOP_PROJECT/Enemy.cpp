#include "Enemy.h"
#include "WeaponsEach.h"
EnemyAiState* RoamingAround::update(Enemy* enemy, float dt, const World& w) {
	if (enemy->target == nullptr)
		return nullptr;
	float coordinateX = enemy->getX();
	float coordinateY = enemy->getY();
	float playerX = enemy->target->getX();
	float playerY = enemy->target->getY();

	if (((coordinateX - playerX < 0 ? playerX - coordinateX : coordinateX - playerX) < detectionRange) && (coordinateY - playerY < 0 ? playerY - coordinateY : coordinateY - playerY) < detectionRange)
		return new runningState();

	timerToSwitchDirection -= dt;
	if (timerToSwitchDirection <= 0)
	{
		directionSign *= -1;
		timerToSwitchDirection = 4;
	}

	enemy->setVelocityX(directionSign * roamingAroundSpeed);
	enemy->applyGravity(dt);
	enemy->changeXandY(dt, w);




	return nullptr;
}
EnemyAiState* AttackingState::update(Enemy* enemy, float dt, const World& w) {
	if (enemy->target == nullptr)
		return nullptr;
	float coordinateX = enemy->getX();
	float coordinateY = enemy->getY();
	float playerX = enemy->target->getX();
	float playerY = enemy->target->getY();
	bool right = playerX > coordinateX;
	// too far away on either axis (|| not &&): stop shooting and chase again
	if (((coordinateX - playerX < 0 ? playerX - coordinateX : coordinateX - playerX) > losingRange) || (coordinateY - playerY < 0 ? playerY - coordinateY : coordinateY - playerY) > losingRange)
		return new runningState();

	attackCoolDown -= dt;
	if (attackCoolDown <= 0) {
		attackCoolDown = enemy->getNormalFireRate();
		enemy->throwProjectile();
	}

	enemy->setVelocityX(0);
	enemy->applyGravity(dt);
	enemy->changeXandY(dt, w);
	return nullptr;
}
EnemyAiState* runningState::update(Enemy* enemy, float dt, const World& w) {
	if (enemy->target == nullptr)
		return nullptr;

	float coordinateX = enemy->getX();
	float coordinateY = enemy->getY();
	float playerX = enemy->target->getX();
	float playerY = enemy->target->getY();
	bool right = playerX > coordinateX;

	if (((coordinateX - playerX < 0 ? playerX - coordinateX : coordinateX - playerX) < attackingRange) && (coordinateY - playerY < 0 ? playerY - coordinateY : coordinateY - playerY) < attackingRange)
		return new AttackingState();
	else
		if (((coordinateX - playerX < 0 ? playerX - coordinateX : coordinateX - playerX) > losingRange) || (coordinateY - playerY < 0 ? playerY - coordinateY : coordinateY - playerY) > losingRange)
			return new RoamingAround();

	enemy->setVelocityX(right ? runningSpeed : -runningSpeed);
	//jumop
	bool blockCollision = right ? enemy->isTouchingRightWall(w) : enemy->isTouchingLeftWall(w);
	if (blockCollision) {
		enemy->setVelocityY(-600);
	}
	enemy->applyGravity(dt);
	enemy->changeXandY(dt, w);
	return nullptr;
}



EnemyAiState* Descending::update(Enemy* enemy, float dt, const World& w) {
	enemy->setVelocityX(0);
	enemy->setVelocityY(descendingSpeed);
	enemy->changeXandY(dt, w);
	if (enemy->isTouchingGround(w))
		return new RoamingAround();
	return nullptr;
}





void Enemy::die() {
	dying = true;
	deathTimer = DEATH_TIME;
	// thrown back away from the player and up into the air
	bool playerOnRight = target != nullptr && target->getX() > x;
	velocityX = playerOnRight ? -180.f : 180.f;
	velocityY = -380.f;
	touchingGround = false;
}

void Enemy::update(float dt, const World& w) {
	if (dying) {
		// no AI while dying, just fly back and fall
		deathTimer -= dt;
		applyGravity(dt);
		changeXandY(dt, w);
		if (touchingGround) velocityX *= 0.8f; // slide to a stop on the ground
		if (deathTimer <= 0) isActive = false; // now the EntityManager deletes it
		return;
	}
	if (!currentAiState)
		return;
	EnemyAiState* next = currentAiState->update(this, dt, w);
	if (projectile && typeOfEnemy != 3) muzzleFrames = 3; // it just fired (grenades have no flash)
	if (next)
	{
		delete currentAiState;
		currentAiState = next;
		currentAiState->enterState(this);
	}
}

void Enemy::onHitByProjectile(Projectile* p) {
	// explosives and normal shots go through different functions so the shielded soldier can block bullets
	if (p->getBlastRadius() > 0)
		takeExplosionDamage(p);
	else
		TakeNormalDamage(p);
	if (p->diesOnHit())
		p->setActive(false);
}
void Enemy::aimAtTarget(float& dx, float& dy) const {
	// direction from the gun (30px below the top of the enemy) to the middle of the target,
	// divided by its length so it is 1 long (the projectile speed does the rest)
	dx = (target->getX() + target->getWidth() / 2) - (x + width / 2);
	dy = (target->getY() + target->getHeight() / 2) - (y + 30);
	float length = std::sqrt(dx * dx + dy * dy);
	if (length < 1) { dx = 1; dy = 0; return; }
	dx /= length;
	dy /= length;
}

void Enemy::setPictures(const Texture& stand, const char* walkFile, int frames) {
	standTex = &stand;
	if (walkFile && walkTex.loadFromFile(walkFile)) walkFrames = frames;
}

void Enemy::drawEnemy(RenderWindow& w, const Camera& cam, bool pictureFacesLeft) {
	if (dying) {
		// how far into the animation: 0 at the start, 1 at the end
		float t = 1 - deathTimer / DEATH_TIME;
		alpha = (Uint8)(255 * (1 - t));                           // fade out
		if ((int)(t * 12) % 2 == 1) alpha /= 3;                   // and blink
		float spin = (velocityX > 0 ? 1 : -1) * 80 * t;           // tip over backwards
		sprite.setTexture(*standTex, true);
		bool flyingRight = velocityX > 0;
		drawSprite(w, cam, drawScale, pictureFacesLeft ? !flyingRight : flyingRight, 0, spin);
		return;
	}
	bool walking = touchingGround && std::abs(velocityX) > 10;

	// face the way it walks, or the player when standing still; flip the picture if it faces the other way
	bool faceRight = walking ? velocityX > 0 : (target != nullptr && target->getX() > getX());
	bool mirrored = pictureFacesLeft ? faceRight : !faceRight;

	if (walking && walkFrames > 0) {
		// pick the walking picture from how far it has walked: next frame every 8px
		sprite.setTexture(walkTex, true);
		int frameW = walkTex.getSize().x / walkFrames;
		int frame = (int)(std::abs(x) / 8) % walkFrames;
		sprite.setTextureRect(IntRect(frame * frameW, 0, frameW, walkTex.getSize().y));
		drawSprite(w, cam, PIXEL_SCALE, mirrored); // the strips are original size pictures
	}
	else {
		sprite.setTexture(*standTex, true);
		float lift = 0, lean = 0;
		if (walkFrames == 0) walkBounce(touchingGround, lift, lean); // no strip: bounce instead
		drawSprite(w, cam, drawScale, mirrored, lift, lean);
	}
	bool gunRight = target != nullptr && target->getX() > getX();
	drawMuzzleFlash(w, cam, gunRight ? x + width + 8 : x - 8, y + 30);
}

void Enemy::changeXandY(float dt, const World& w) {
	x += velocityX * dt;
	y += velocityY * dt;
	checkGroundCollisions(w);
}
Projectile* Enemy::getProjectile() {
	Projectile* p = projectile;
	projectile = nullptr;
	return p;
}
void RebelSoldier::throwProjectile() {
	float dx, dy;
	aimAtTarget(dx, dy);
	projectile = new Bullet(getX() + width / 2, getY() + 30, dx, dy, false, 10);
}
void RebelSoldier::render(sf::RenderWindow& w, const Camera& cam) {
	drawEnemy(w, cam, true); // the rebel pictures face left
}

void ShieldedSoldier::throwProjectile() {
	float dx, dy;
	aimAtTarget(dx, dy);
	projectile = new Bullet(getX() + width / 2, getY() + 30, dx, dy, false, 10);
}
void ShieldedSoldier::TakeNormalDamage(Projectile* p) {
	bool bulletIsRight = p->getX() > getX();
	bool enemyIsRight = (target != nullptr) && (target->getX() > getX());
	if (bulletIsRight != enemyIsRight)
		p->applyDamage(this);
}
void ShieldedSoldier::takeExplosionDamage(Projectile* p) { p->applyDamage(this); }
void ShieldedSoldier::render(sf::RenderWindow& w, const Camera& cam) {
	drawEnemy(w, cam, false); // the knight picture faces right (shield in front)
}

void BazookaSoldier::throwProjectile() {
	float dx, dy;
	aimAtTarget(dx, dy);
	projectile = new Rocket(getX() + width / 2, getY() + 30, dx, dy, false, 20);
}
void BazookaSoldier::render(sf::RenderWindow& w, const Camera& cam) {
	drawEnemy(w, cam, true); // the rebel pictures face left
}

void GrenadeSoldier::throwProjectile() {
	bool right = target->getX() > getX();
	projectile = new NormalGrenade(getX(), getY() + 5, right ? 0.45f : -0.45f, -0.7f, false); // lob lands about 300px away, the attack range
}
void GrenadeSoldier::render(sf::RenderWindow& w, const Camera& cam) {
	drawEnemy(w, cam, true); // the rebel pictures face left
}
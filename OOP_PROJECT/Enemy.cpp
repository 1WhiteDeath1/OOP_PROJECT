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
	if (((coordinateX - playerX < 0 ? playerX - coordinateX : coordinateX - playerX) > losingRange) && (coordinateY - playerY < 0 ? playerY - coordinateY : coordinateY - playerY) > losingRange)
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
		if (((coordinateX - playerX < 0 ? playerX - coordinateX : coordinateX - playerX) > losingRange) && (coordinateY - playerY < 0 ? playerY - coordinateY : coordinateY - playerY) > losingRange)
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





void Enemy::update(float dt, const World& w) {
	if (!currentAiState)
		return;
	EnemyAiState* next = currentAiState->update(this, dt, w);
	if (next)
	{
		delete currentAiState;
		currentAiState = next;
		currentAiState->enterState(this);
	}
}

void Enemy::onHitByProjectile(Projectile* p) {
	p->applyDamage(this);
	if (p->diesOnHit())
		p->setActive(false);
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
	bool right = target->getX() > getX();
	projectile = new Bullet(getX(), getY() + 5, right ? 1 : -1, 0, false, 3);
}
void RebelSoldier::render(sf::RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(getX()), cam.toScreenY(getY()));
	w.draw(sprite);
}

void ShieldedSoldier::throwProjectile() {
	bool right = target->getX() > getX();
	projectile = new Bullet(getX(), getY() + 5, right ? 1 : -1, 0, false, 3);
}
void ShieldedSoldier::TakeNormalDamage(Projectile* p) {
	bool bulletIsRight = p->getX() > getX();
	bool enemyIsRight = (target != nullptr) && (target->getX() > getX());
	if (bulletIsRight != enemyIsRight)
		takeDamage(p->getDamage());
}
void ShieldedSoldier::takeExplosionDamage(Projectile* p) { takeDamage(p->getDamage()); }
void ShieldedSoldier::render(sf::RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(getX()), cam.toScreenY(getY()));
	w.draw(sprite);
}

void BazookaSoldier::throwProjectile() {
	bool right = target->getX() > getX();
	projectile = new Rocket(getX(), getY() + 5, right ? 0.8f : -0.8f, -0.6f, false, 5);
}
void BazookaSoldier::render(sf::RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(getX()), cam.toScreenY(getY()));
	w.draw(sprite);
}

void GrenadeSoldier::throwProjectile() {
	bool right = target->getX() > getX();
	projectile = new NormalGrenade(getX(), getY() + 5, right ? 0.7f : -0.7f, -0.7f, false);
}
void GrenadeSoldier::render(sf::RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(getX()), cam.toScreenY(getY()));
	w.draw(sprite);


	
}
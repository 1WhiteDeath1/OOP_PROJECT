#pragma once
#include "DamagableEntity.h"
#include "Soldier.h"
class Soldier;

class Vehicle :public DamagableEntity {
protected:
float normalFireRate; // seconds between shots
float currentTime=0;

int vehicleDurability;
bool playerInVehicle;
Soldier* playerInside=nullptr;
Projectile* projectile=nullptr;

bool facingRight=true;       // which way the vehicle is going / aiming
bool pictureFacesRight=true; // which way the loaded picture looks (the Bradley and the enemy sub look left)
float turn=1;                // 1 = facing right, -1 = left, in between while turning around

public:
Vehicle(float x, float y, float wd, float ht, int hp,float Fr, int vd):DamagableEntity(x,y,wd,ht,hp),normalFireRate(Fr),vehicleDurability(vd),playerInVehicle(false){
playerInside=nullptr;
}

// stretch the loaded picture over the vehicle's hitbox
void fitSprite() {
	if (texture.getSize().x > 0)
		sprite.setScale(width / texture.getSize().x, height / texture.getSize().y);
}
// every frame: face the pilot's way (or the way it drives when nobody is inside) and turn around
// smoothly, the picture squeezes to a thin line and opens up the other way, a full turn takes 0.3s
void updateFacing(float dt) {
	if (playerInside) facingRight = playerInside->isFacingRight();
	else if (velocityX > 5) facingRight = true;
	else if (velocityX < -5) facingRight = false;
	float target = facingRight ? 1.f : -1.f;
	float step = 6.5f * dt;
	if (turn < target) turn = (turn + step > target) ? target : turn + step;
	else if (turn > target) turn = (turn - step < target) ? target : turn - step;
}
// draws the picture stretched over the hitbox, mirrored when the vehicle faces the other way
void drawVehicle(sf::RenderWindow& w, const Camera& cam) {
	if (texture.getSize().x == 0) return;
	float sx = width / texture.getSize().x;
	float sy = height / texture.getSize().y;
	float flip = pictureFacesRight ? turn : -turn;
	sprite.setOrigin(texture.getSize().x / 2.f, 0); // flip around the middle so it turns on the spot
	sprite.setScale(sx * flip, sy);
	sprite.setPosition(cam.toScreenX(x + width / 2.f), cam.toScreenY(y));
	applyHitTint();
	w.draw(sprite);
}
// where the shots come out (used for the aim marker), the vehicles fire from here
virtual void getGunPosition(float& gx, float& gy) const {
	bool right = playerInside ? playerInside->isFacingRight() : facingRight;
	gx = x + (right ? width : -20);
	gy = y + 20;
}

// keeps the pilot (and so the camera) on the vehicle
void carryPilot() {
	if (playerInside)
		playerInside->setPosition(x, y);
}


virtual void attack()=0;
virtual void move(float dt, const World&w)=0;// we need world for collision and dt so game runs smoothly irrespective of fps
virtual void update(float dt, const World&w)=0;
virtual void render(sf::RenderWindow&w, const Camera&cam)=0;

// take damage and die functions are in damagableEntity
void enterVehicle(Soldier* soldier) {
	if(soldier==nullptr)
	return ;
	playerInside=soldier;
	playerInVehicle=true;
	playerInside->setActive(false);
	playerInside->setPosition(x,y);//+10 so player appears after vehicle not on it

}
void exitVehicle() {
if(playerInside==nullptr)
return;
playerInVehicle=false;
playerInside->setActive(true);
playerInside->setPosition(x+10,y);
playerInside=nullptr;

}
bool isVehicleOccupied()const {
	return playerInVehicle;
}
bool playerVehicle()const {
if(playerInside==nullptr)
return false;
else
return true;

}
Soldier* getPlayer()const {
	return playerInside;
}

// enemy vehicles cannot be entered, they chase the target instead
virtual bool isEnemy() const { return false; }
virtual void setTarget(Soldier* s) {}

// hands the projectile fired this frame (if any) to the EntityManager
Projectile* getProjectile() {
	Projectile* p = projectile;
	projectile = nullptr;
	return p;
}
virtual ~Vehicle(){ delete projectile; }

};




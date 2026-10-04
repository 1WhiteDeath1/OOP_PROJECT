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

public:
Vehicle(float x, float y, float wd, float ht, int hp,float Fr, int vd):DamagableEntity(x,y,wd,ht,hp),normalFireRate(Fr),vehicleDurability(vd),playerInVehicle(false){
playerInside=nullptr;
}

// stretch the loaded picture over the vehicle's hitbox
void fitSprite() {
	if (texture.getSize().x > 0)
		sprite.setScale(width / texture.getSize().x, height / texture.getSize().y);
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




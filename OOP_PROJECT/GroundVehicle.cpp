#include "GroundVehicle.h"
#include"Projectile.h"
#include "ProjectileWeapon.h"
#include "World.h"
#include "Camera.h"
#include "Soldier.h"
#include "WeaponsEach.h"
#include "Aim.h"

float MetalSlug::metalSlugFireRate=1;
void M15Bradley::move(float dt, const World& w) {
applyGravity(dt);
if(playerToHit==nullptr)
{
x+=velocityX*dt;
y+=velocityY*dt;
checkGroundCollisions(w);
return;
}

	float xCoordinate=playerToHit->getX();
	if(x>xCoordinate)
	velocityX-=200*dt;

	else
velocityX+=200*dt;
if(velocityX>=120.0f)
velocityX=120.0f;
if(velocityX<-120.0f)
velocityX=-120.0f;

// only chase once the player is close enough to notice
if((xCoordinate-x<0?x-xCoordinate:xCoordinate-x)<20 || (xCoordinate-x<0?x-xCoordinate:xCoordinate-x)>800)
velocityX=0;
x+=velocityX*dt;
y+=velocityY*dt;
checkGroundCollisions(w);

}

void M15Bradley::attack() {
	if(playerToHit==nullptr)
	return;
	float xCoordinate=playerToHit->getX();
	float yCoordinate=playerToHit->getY();
	if( ( (x-xCoordinate)<0?-(x-xCoordinate):x-xCoordinate )>500.0||( (y-yCoordinate)<0?-(y-yCoordinate):y-yCoordinate )>300.0 )
	return;
	int angle=0;
	if( (yCoordinate-y<0?y-yCoordinate:yCoordinate-y)<15)
	angle=0;
	else if( ( (x-xCoordinate)<0?-(x-xCoordinate):x-xCoordinate )>30.0||( (y-yCoordinate)<0?-(y-yCoordinate):y-yCoordinate )>30.0 )
	angle=45;
	else
	angle=30;

	bool right=playerToHit->getX()>x;
	bool up=yCoordinate<y;
	float dx,dy;
	if(angle ==45)
	{ 
	dx=right?0.707:-0.707;
	dy=up?-0.707:0.707;
	}
	else if (angle == 30) {
		dx=right?0.866:-0.866;
		dy=up?-0.5:0.5;

	}
	else
	{
		dx=right?1:-1;
		dy=0;
	}
	projectile=new Rocket(x+(right?width:-20),y+20,dx,dy,false,20);




}
void M15Bradley::update(float dt, const World& w) {
currentTime+=dt;
if (currentTime > normalFireRate) {
	currentTime=0;
	attack();
}

move(dt, w);
}
void M15Bradley::render(sf::RenderWindow& w, const Camera& cam) {
	drawVehicle(w, cam);
}

void MetalSlug::move(float dt, const World& w) {
applyGravity(dt);
if(playerInside)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
	velocityX=-250;
	playerInside->setFacing(false);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
	velocityX=250;
	playerInside->setFacing(true);
	}
	else
	velocityX=0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && touchingGround)
	velocityY=-600;

	// aim the cannon (120 degrees a second, the mouse aims instead while it is on)
	if (!Aim::usingMouse() && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	playerInside->changeAngle(120*dt);
	if (!Aim::usingMouse() && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	playerInside->changeAngle(-120*dt);
}
else
velocityX=0;
x+=velocityX*dt;
y+=velocityY*dt;
checkGroundCollisions(w);
if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
exitVehicle();




}
void MetalSlug::update(float dt,const World& w) {
currentTime+=dt;
if (currentTime > normalFireRate && playerInside && Aim::firePressed()) {
	currentTime=0;
attack();
}


move(dt, w);
carryPilot();
}
void MetalSlug::attack() {
if(!playerInside)
return;

bool right=playerInside->isFacingRight();
float angle=playerInside->getAimAngle();


float dx, dy;
ProjectileWeapon::getDirection(angle, right, dx, dy);
projectile=new Rocket(x+(right?width:-20),y+20,dx,dy,true,40);


}


void MetalSlug::render(sf::RenderWindow& w, const Camera& cam) {
	drawVehicle(w, cam);
}


void AmphibiousSlug::changeForm(const World& w) {
    float middleX=x+width/2.0;
    float middleY=y+height/2.0;
    if (w.isSolid(middleX, middleY)) {
        currentForm=0;
    }
   else if (w.isWater(middleX, middleY)) {
        currentForm=1;
        return;
    }
  
    else
    currentForm=2;
    return;
}
void AmphibiousSlug::update(float dt, const World& w) {
currentTime+=dt;
if (currentTime > normalFireRate && Aim::firePressed()) {
	currentTime=0;
	attack();
}

move(dt, w);
carryPilot();
}
void AmphibiousSlug::attack() {
if(playerInside==nullptr)
return;
bool right=playerInside->isFacingRight();
float angle=playerInside->getAimAngle();





float dx, dy;
ProjectileWeapon::getDirection(angle, right, dx, dy);
projectile=new Rocket(x+(right?width:-20),y+20,dx,dy,true,40);

}
void AmphibiousSlug::render(sf::RenderWindow& w, const Camera& cam) {
	drawVehicle(w, cam);
}
void AmphibiousSlug::move(float dt, const World& w) {
if(playerInside==nullptr)
{
// parked: just sit on the ground
applyGravity(dt);
velocityX=0;
x+=velocityX*dt;
y+=velocityY*dt;
checkGroundCollisions(w);
return;
}
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
playerInside->setFacing(true);
else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
playerInside->setFacing(false);
changeForm(w);
if(currentForm==0)
{ 
applyGravity(dt);

if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
velocityX+=speedOnLand*dt;
else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
velocityX-=speedOnLand*dt;

if(velocityX>speedOnLand)
velocityX=speedOnLand;
if(velocityX<-speedOnLand)
velocityX=-speedOnLand;

	
x+=velocityX*dt;
y+=velocityY*dt;
checkGroundCollisions(w);
}
else if (currentForm == 1) {
velocityY-=30*dt;

if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
velocityX+=speedInWater*dt;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
velocityX-=speedInWater*dt;


if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
velocityY-=speedInWater*dt;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
velocityY+=speedInWater*dt;


if(velocityX>speedInWater)
velocityX=speedInWater;
if(velocityX<-speedInWater)
velocityX=-speedInWater;


if(velocityY>speedInWater)
velocityY=speedInWater;
if(velocityY<-speedInWater)
velocityY=-speedInWater;

x+=velocityX*dt;
y+=velocityY*dt;
checkGroundCollisions(w);

}
else if (currentForm == 2) {



if(velocityY<700.0f)
velocityY+=500*dt;




if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
velocityX+=speedInAir*dt;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
velocityX-=speedInAir*dt;


if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
velocityY-=speedInAir*dt;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
velocityY+=speedInAir*dt;

if(velocityX>speedInAir)
velocityX=speedInAir;
if(velocityX<-speedInAir)
velocityX=-speedInAir;


if(velocityY>speedInAir)
velocityY=speedInAir;
if(velocityY<-speedInAir)
velocityY=-speedInAir;

x+=velocityX*dt;
y+=velocityY*dt;
checkGroundCollisions(w);
}
}
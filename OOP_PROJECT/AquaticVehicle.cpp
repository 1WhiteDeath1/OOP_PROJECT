#include"AquaticVehicle.h"
#include "WeaponsEach.h"
#include "Aim.h"

void SlugMariner::attack() {
if(playerInside==nullptr)
return;
bool right=playerInside->isFacingRight();
float angle=playerInside->getAimAngle();


float dx, dy;
ProjectileWeapon::getDirection(angle, right, dx, dy);
float fireX=x+(right?width:-20);
float fireY=y+height/2;

// one shot per call: the missiles take priority over the gun
if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && horizontalMissileAmmo>0)
{
projectile=new Rocket(fireX,fireY,right?1:-1,0,true,40);
horizontalMissileAmmo--;
}
else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && verticalMissileAmmo>0)
{
projectile=new Rocket(x+width/2,y-20,0,-1,true,40);
verticalMissileAmmo--;
}
else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && reverseProjectileAmmo>0)
{
projectile=new Rocket(right?x-20:x+width,fireY,right?-1:1,0,true,40);
reverseProjectileAmmo--;
}
else if (Aim::firePressed())
{
projectile=new Bullet(fireX,fireY,dx,dy,true,10);
}
}


void SlugMariner::move(float dt, const World& w) {
    if(playerInside==nullptr)
    return;
    float speedInWater=100;
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
    {
        velocityX+=speedInWater*dt;
        playerInside->setFacing(true);

    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
    {
        velocityX-=speedInWater*dt;
        playerInside->setFacing(false);

    }
     if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
    {
        velocityY-=speedInWater*dt;
    }
     if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
    {
        velocityY+=speedInWater*dt;
    }
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
// a submarine stays under the water line
if(y<World::seaLEVEL*World::CELL)
y=World::seaLEVEL*World::CELL;

}
void SlugMariner::update(float dt,const World& w) {
currentTime+=dt;
if (currentTime > normalFireRate) {
	currentTime=0;
attack();
}


move(dt, w);
carryPilot();
}
void SlugMariner::render(sf::RenderWindow& w, const Camera& cam) {
	drawVehicle(w, cam);
}
void EnemySub::move(float dt, const World& w) {
if(playerToHit==nullptr)
return;
float speedInWater=100;
float xCoordinate=playerToHit->getX();
float yCoordinate=playerToHit->getY();
// only chase once the player is close enough to notice
if((x-xCoordinate<0?xCoordinate-x:x-xCoordinate)>800)
return;
if(x>xCoordinate)
	velocityX-=speedInWater*dt;

	else
velocityX+=speedInWater*dt;

if(y>yCoordinate)
	velocityY-=speedInWater*dt;

	else
velocityY+=speedInWater*dt;
if(velocityX>=speedInWater)
velocityX=speedInWater;
if(velocityX<-speedInWater)
velocityX=-speedInWater;
if(velocityY>=speedInWater)
velocityY=speedInWater;
if(velocityY<-speedInWater)
velocityY=-speedInWater;

x+=velocityX*dt;
y+=velocityY*dt;
checkGroundCollisions(w);
if(y<World::seaLEVEL*World::CELL)
y=World::seaLEVEL*World::CELL;


}
void EnemySub::update(float dt,const World& w) {
currentTime+=dt;
if (currentTime > normalFireRate) {
	currentTime=0;
attack();
}
move(dt, w);
}
void EnemySub::render(sf::RenderWindow& w, const Camera& cam) {
	drawVehicle(w, cam);
}
void EnemySub::attack() {
    if(playerToHit==nullptr)
    return;
    float xCoordinate=playerToHit->getX();
	float yCoordinate=playerToHit->getY();
	if( ( (x-xCoordinate)<0?-(x-xCoordinate):x-xCoordinate )>400.0||( (y-yCoordinate)<0?-(y-yCoordinate):y-yCoordinate )>300.0 )
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
	projectile=new Rocket(x+(right?width:-20),y+height/2,dx,dy,false,20);
}




















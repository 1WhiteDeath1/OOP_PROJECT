#include "AerialVehicle.h"
#include "WeaponsEach.h"
#include "Aim.h"
void FlyingTara::attack() {
    if (playerToHit == nullptr)
return;
float coordinateX=playerToHit->getX();
if ((x-coordinateX< 0?coordinateX-x:x-coordinateX)<60)
{
	projectile=new NormalGrenade(x+width/2,y+height,0,1,false);
}
}
void FlyingTara::update(float dt, const World& w) {
currentTime+=dt;
if (currentTime > normalFireRate) {
	currentTime=0;
	attack();
}

move(dt, w);
}
void FlyingTara::render(sf::RenderWindow& w, const Camera& cam) {
	drawVehicle(w, cam);
}
void FlyingTara::move(float dt, const World& w) {
    if (playerToHit == nullptr) return;
float coordinateX=playerToHit->getX();
// only chase once the player is close enough to notice
if((x-coordinateX<0?coordinateX-x:x-coordinateX)>800)
return;
if(x>coordinateX)
velocityX-=100*dt;
else
velocityX+=100*dt;

if(velocityX>100)
velocityX=100;
if(velocityX<-100)
velocityX=-100;
x+=velocityX*dt;
y=hoveringY;



}
void FlyingTara::playerInSight(Soldier* s) {
	playerToHit=s;
}
void SlugFlyer::update(float dt, const World& w) {
currentTime+=dt;
missileCountDown-=dt;
if (currentTime>normalFireRate) {
	currentTime=0;
	attack();
}

move(dt, w);
if(playerInside!=nullptr)
playerInside->setPosition(x,y);

}

void SlugFlyer::attack() {
if(playerInside==nullptr)
return;
bool right=playerInside->isFacingRight();
float angle=playerInside->getAimAngle();


float dx, dy;
ProjectileWeapon::getDirection(angle, right, dx, dy);
float fireX=x+(right?width:-20);
float fireY=y+height/2;

if ((sf::Keyboard::isKeyPressed(sf::Keyboard::R)) && missileCountDown<=0 && missileCount>0)
{
projectile=new Rocket(fireX,fireY,dx,dy,true,40);
missileCount--;
missileCountDown=1;

}
else if (Aim::firePressed())
{
projectile=new Bullet(fireX,fireY,dx,dy,true,10);
}



}
void SlugFlyer::render(sf::RenderWindow& w, const Camera& cam) {
	drawVehicle(w, cam);
}




void SlugFlyer::move(float dt,const World& w)
{
if(playerInside==nullptr)
return;
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
velocityX+=150*dt;
playerInside->setFacing(true);
}
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
velocityX-=150*dt;
playerInside->setFacing(false);
}
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
velocityY-=150*dt;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
velocityY+=150*dt;
if(velocityX>200)velocityX=200;
if(velocityX<-200)velocityX=-200;
if(velocityY>200)velocityY=200;
if(velocityY<-200)velocityY=-200;
x+=velocityX*dt;
y+=velocityY*dt;
checkGroundCollisions(w);
if(y<0)y=0;
playerInside->setPosition(x,y);
}


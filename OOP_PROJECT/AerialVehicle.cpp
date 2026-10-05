#include "AerialVehicle.h"
#include "WeaponsEach.h"
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
    sprite.setPosition(cam.toScreenX(x), cam.toScreenY(y));
    w.draw(sprite);
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


const float sin_values[91] = {
    -0.7071f, // -45
    -0.6947f, // -44
    -0.6820f, // -43
    -0.6691f, // -42
    -0.6561f, // -41
    -0.6428f, // -40
    -0.6293f, // -39
    -0.6157f, // -38
    -0.6018f, // -37
    -0.5878f, // -36
    -0.5736f, // -35
    -0.5592f, // -34
    -0.5446f, // -33
    -0.5299f, // -32
    -0.5150f, // -31
    -0.5000f, // -30
    -0.4848f, // -29
    -0.4695f, // -28
    -0.4540f, // -27
    -0.4384f, // -26
    -0.4226f, // -25
    -0.4067f, // -24
    -0.3907f, // -23
    -0.3746f, // -22
    -0.3584f, // -21
    -0.3420f, // -20
    -0.3256f, // -19
    -0.3090f, // -18
    -0.2924f, // -17
    -0.2756f, // -16
    -0.2588f, // -15
    -0.2419f, // -14
    -0.2250f, // -13
    -0.2079f, // -12
    -0.1908f, // -11
    -0.1736f, // -10
    -0.1564f, // -9
    -0.1392f, // -8
    -0.1219f, // -7
    -0.1045f, // -6
    -0.0872f, // -5
    -0.0698f, // -4
    -0.0523f, // -3
    -0.0349f, // -2
    -0.0175f, // -1
     0.0000f, //  0
     0.0175f, //  1
     0.0349f, //  2
     0.0523f, //  3
     0.0698f, //  4
     0.0872f, //  5
     0.1045f, //  6
     0.1219f, //  7
     0.1392f, //  8
     0.1564f, //  9
     0.1736f, // 10
     0.1908f, // 11
     0.2079f, // 12
     0.2250f, // 13
     0.2419f, // 14
     0.2588f, // 15
     0.2756f, // 16
     0.2924f, // 17
     0.3090f, // 18
     0.3256f, // 19
     0.3420f, // 20
     0.3584f, // 21
     0.3746f, // 22
     0.3907f, // 23
     0.4067f, // 24
     0.4226f, // 25
     0.4384f, // 26
     0.4540f, // 27
     0.4695f, // 28
     0.4848f, // 29
     0.5000f, // 30
     0.5150f, // 31
     0.5299f, // 32
     0.5446f, // 33
     0.5592f, // 34
     0.5736f, // 35
     0.5878f, // 36
     0.6018f, // 37
     0.6157f, // 38
     0.6293f, // 39
     0.6428f, // 40
     0.6561f, // 41
     0.6691f, // 42
     0.6820f, // 43
     0.6947f, // 44
     0.7071f  // 45
};

const float cos_values[91] = {
     0.7071f, // -45
     0.7193f, // -44
     0.7314f, // -43
     0.7431f, // -42
     0.7547f, // -41
     0.7660f, // -40
     0.7771f, // -39
     0.7880f, // -38
     0.7986f, // -37
     0.8090f, // -36
     0.8192f, // -35
     0.8290f, // -34
     0.8387f, // -33
     0.8480f, // -32
     0.8572f, // -31
     0.8660f, // -30
     0.8746f, // -29
     0.8829f, // -28
     0.8910f, // -27
     0.8988f, // -26
     0.9063f, // -25
     0.9135f, // -24
     0.9205f, // -23
     0.9272f, // -22
     0.9336f, // -21
     0.9397f, // -20
     0.9455f, // -19
     0.9511f, // -18
     0.9563f, // -17
     0.9613f, // -16
     0.9659f, // -15
     0.9703f, // -14
     0.9744f, // -13
     0.9781f, // -12
     0.9816f, // -11
     0.9848f, // -10
     0.9877f, // -9
     0.9903f, // -8
     0.9925f, // -7
     0.9945f, // -6
     0.9962f, // -5
     0.9976f, // -4
     0.9986f, // -3
     0.9994f, // -2
     0.9998f, // -1
     1.0000f, //  0
     0.9998f, //  1
     0.9994f, //  2
     0.9986f, //  3
     0.9976f, //  4
     0.9962f, //  5
     0.9945f, //  6
     0.9925f, //  7
     0.9903f, //  8
     0.9877f, //  9
     0.9848f, // 10
     0.9816f, // 11
     0.9781f, // 12
     0.9744f, // 13
     0.9703f, // 14
     0.9659f, // 15
     0.9613f, // 16
     0.9563f, // 17
     0.9511f, // 18
     0.9455f, // 19
     0.9397f, // 20
     0.9336f, // 21
     0.9272f, // 22
     0.9205f, // 23
     0.9135f, // 24
     0.9063f, // 25
     0.8988f, // 26
     0.8910f, // 27
     0.8829f, // 28
     0.8746f, // 29
     0.8660f, // 30
     0.8572f, // 31
     0.8480f, // 32
     0.8387f, // 33
     0.8290f, // 34
     0.8192f, // 35
     0.8090f, // 36
     0.7986f, // 37
     0.7880f, // 38
     0.7771f, // 39
     0.7660f, // 40
     0.7547f, // 41
     0.7431f, // 42
     0.7314f, // 43
     0.7193f, // 44
     0.7071f  // 45
};
int index=angle+45;

float dx=right?cos_values[index]:( - cos_values[index]);
float dy=-sin_values[index];
float fireX=x+(right?width:-20);
float fireY=y+height/2;

if ((sf::Keyboard::isKeyPressed(sf::Keyboard::R)) && missileCountDown<=0 && missileCount>0)
{
projectile=new Rocket(fireX,fireY,dx,dy,true,40);
missileCount--;
missileCountDown=1;

}
else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)))
{
projectile=new Bullet(fireX,fireY,dx,dy,true,10);
}



}
void SlugFlyer::render(sf::RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(x),cam.toScreenY(y));
	w.draw(sprite);

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


#include "GroundVehicle.h"
#include"Projectile.h"
#include "ProjectileWeapon.h"
#include "World.h"
#include "Camera.h"
#include "Soldier.h"
//#include "Rocket.h"
//#include "NormalGrenade.h"

int MetalSlug::metalSlugFireRate=2;
void M15Bradley::move(float dt, const World& w) {
if(playerToHit==nullptr)
return;
applyGravity(dt);

	float xCoordinate=playerToHit->getX();
	if(x>xCoordinate)
	velocityX-=200*dt;

	else
velocityX+=200*dt;
if(velocityX>=120.0f)
velocityX=120.0f;
if(velocityX<-120.0f)
velocityX=-120.0f;

if(xCoordinate-x<20)
velocityX=0;
x+=velocityX*dt;
y+=velocityY*dt;
checkGroundCollisions(w);

}

void M15Bradley::attack() {
	float xCoordinate=playerToHit->getX();
	float yCoordinate=playerToHit->getY();
	if( ( (x-xCoordinate)<0?-(x-xCoordinate):x-xCoordinate )>70.0||( (y-yCoordinate)<0?-(y-yCoordinate):y-yCoordinate )>70.0 )
	return;
	int angle=0;
	if( (yCoordinate-y<0?y-yCoordinate:yCoordinate-y)<15)
	angle=0;
	else if( ( (x-xCoordinate)<0?-(x-xCoordinate):x-xCoordinate )>30.0||( (y-yCoordinate)<0?-(y-yCoordinate):y-yCoordinate )>30.0 )
	angle=45;
	else
	angle=30;

	bool right=playerToHit->getX()>x;
	float dx,dy;
	if(angle ==45)
	{ 
	dx=right?0.707:-0.707;
	dy=0.707;
	}
	else if (angle == 30) {
		dx=right?0.866:-0.866;
		dy=0.866;

	}
	else
	{
		dx=right?1:-1;
		dy=0;
	}
	//projectile=new Rocket(x+(right?+5:-5),y+5,dx,dy,false);




}
void M15Bradley::update(float dt, const World& w) {
currentTime+=0.0167;
if (currentTime > normalFireRate) {
	currentTime=0;
	attack();
}

move(dt, w);
}
void M15Bradley::render(sf::RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(x),cam.toScreenY(y));
	w.draw(sprite);

}

void MetalSlug::move(float dt, const World& w) {
if(!playerInside)
return;
else
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
	playerInside->changeAngle(2);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
	playerInside->changeAngle(-2);

	}
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
exitVehicle();




}
void MetalSlug::update(float dt,const World& w) {
currentTime+=0.0167;
if (currentTime > normalFireRate) {
	currentTime=0;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
attack();
}


move(dt, w);
}
void MetalSlug::attack() {
if(!playerInside)
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
float dy=sin_values[index];
//projectile=new Rocket(x+(right?+5:-5),y+5,dx,dy,false);


}


void MetalSlug::render(sf::RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(x),cam.toScreenY(y));
	w.draw(sprite);

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
currentTime+=0.0167;
if (currentTime > normalFireRate) {
	currentTime=0;
	
}
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
attack();

move(dt, w);
}
void AmphibiousSlug::attack() {
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
float dy=sin_values[index];
//projectile=new Rocket(x+(right?+5:-5),y+5,dx,dy,false);

}
void AmphibiousSlug::render(sf::RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(x),cam.toScreenY(y));
	w.draw(sprite);

}
void AmphibiousSlug::move(float dt, const World& w) {
if(playerInside==nullptr)
return;
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
velocityY+=speedInWater*dt;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
velocityX-=speedInWater*dt;


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
velocityY+=speedInAir*dt;
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
velocityX-=speedInAir*dt;

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
#include"AquaticVehicle.h"

void SlugMariner::attack() {
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
if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)))
{
//projectile=new Bullet(x+(right?+5:-5),y+5,dx,dy,true);
}

if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
{if(horizontalMissileAmmo<1)
return;
//projectile=new Rocket(x+(right?+5:-5),y+5,right?1:-1,0,true);
horizontalMissileAmmo--;

}
if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
{ if(verticalMissileAmmo<1)
return;
//projectile=new Bullet(x+(right?+5:-5),y+5,0,-1,true);
verticalMissileAmmo--;
}
if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
{if(reverseProjectileAmmo<1)
return;

//projectile=new Bullet(x+(right?+5:-5),y+5,right?-1:1,0,true);
reverseProjectileAmmo--;
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

}
void SlugMariner::update(float dt,const World& w) {
currentTime+=0.0167;
if (currentTime > normalFireRate) {
	currentTime=0;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
attack();
}


move(dt, w);
}
void SlugMariner::render(sf::RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(x),cam.toScreenY(y));
	w.draw(sprite);
}
void EnemySub::move(float dt, const World& w) {
if(playerToHit==nullptr)
return;
float speedInWater=100;
float xCoordinate=playerInside->getX();
float yCoordinate=playerInside->getY();
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


}
void EnemySub::update(float dt,const World& w) {
currentTime+=0.0167;
if (currentTime > normalFireRate) {
	currentTime=0;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
attack();
}
}
void EnemySub::render(sf::RenderWindow& w, const Camera& cam) {
	sprite.setPosition(cam.toScreenX(x),cam.toScreenY(y));
	w.draw(sprite);
}
void EnemySub::attack() {
    if(playerToHit==nullptr)
    return;
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




















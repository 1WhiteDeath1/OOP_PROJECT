#include "PlayState.h"
#include <iostream>
using namespace std;
void PlayState::update(float frameTime) {
	player.handleInput(frameTime, world, entityManager);
	entityManager.update(frameTime, world);

	PlayerSoldier* currCharacter = player.getActive();
	cout << "HP: " << currCharacter->getHp() << "\n";
	camera.follow(currCharacter->getX(), currCharacter->getY());

	//camera clamping
	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > World::WIDTH * World::CELL - Camera::screenW)
		camera.x = World::WIDTH * World::CELL - Camera::screenW;
	if (camera.y > World::HEIGHT * World::CELL - Camera::screenH)
		camera.y = World::HEIGHT * World::CELL - Camera::screenH;

}


void PlayState::render(RenderWindow& w) {
	world.render(w, camera);
	entityManager.render(w, camera);
}
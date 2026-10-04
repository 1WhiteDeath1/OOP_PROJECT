#include "PlayState.h"
#include "MenuState.h"
#include <string>
using namespace std;

void PlayState::enter() {
	levelManager.loadLevel(1, world, entityManager);

	font.loadFromFile("TEXT/font1.ttf");
	hudText.setFont(font);
	hudText.setCharacterSize(22);
	hudText.setFillColor(Color::White);
	hudText.setOutlineColor(Color::Black);
	hudText.setOutlineThickness(2);
	hudText.setPosition(16, 12);

	bannerText.setFont(font);
	bannerText.setCharacterSize(72);
	bannerText.setFillColor(Color(255, 210, 40));
	bannerText.setOutlineColor(Color::Black);
	bannerText.setOutlineThickness(4);
}

void PlayState::update(float frameTime) {
	if (endTimer > 0) {
		endTimer -= frameTime;
		if (endTimer <= 0) gsManager.changeState(new MenuState(gsManager));
		return;
	}

	player.handleInput(frameTime, world, entityManager);
	entityManager.update(frameTime, world);

	PlayerSoldier* currCharacter = player.getActive();
	camera.follow(currCharacter->getX(), currCharacter->getY());

	//camera clamping
	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > World::WIDTH * World::CELL - Camera::screenW)
		camera.x = World::WIDTH * World::CELL - Camera::screenW;
	if (camera.y > World::HEIGHT * World::CELL - Camera::screenH)
		camera.y = World::HEIGHT * World::CELL - Camera::screenH;

	if (player.allDead()) {
		bannerText.setString("GAME OVER");
		endTimer = 3;
	}
	else if (levelManager.isLevelFinished(entityManager)) {
		bannerText.setString("MISSION COMPLETE");
		endTimer = 3;
	}

	//hud
	string hud = string(currCharacter->getName()) +
		"   HP " + to_string(currCharacter->getHp()) +
		"   Lives " + to_string(currCharacter->getLives()) +
		"   Grenades " + to_string(currCharacter->getGrenadeCount());
	Weapon* special = currCharacter->getActiveWeaponSlot(1);
	if (special && special->hasAmmo()) hud += "   Ammo " + to_string(special->getAmmo());
	if (currCharacter->getPowerUpActive()) hud += "   POWER UP!";
	if (player.isPiloting()) hud += "   Vehicle HP " + to_string(player.getVehicle()->getHp());
	hud += "   Enemies " + to_string(entityManager.getEnemyCount());
	hud += "\nA/D move  W jump  Space fire  Up/Down aim  T grenade  R knife  Q power up  Z switch  E enter vehicle  U exit";
	hudText.setString(hud);
}


void PlayState::render(RenderWindow& w) {
	RectangleShape sky(Vector2f((float)Camera::screenW, (float)Camera::screenH));
	sky.setFillColor(Color(120, 190, 235));
	w.draw(sky);

	world.render(w, camera);
	entityManager.render(w, camera);
	w.draw(hudText);

	if (endTimer > 0) {
		FloatRect b = bannerText.getLocalBounds();
		bannerText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
		bannerText.setPosition(Camera::screenW / 2.f, Camera::screenH / 2.f);
		w.draw(bannerText);
	}
}

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

void PlayState::handleInput() {
	// escape leaves the game and goes back to the main menu
	if (Keyboard::isKeyPressed(Keyboard::Escape))
		gsManager.changeState(new MenuState(gsManager));

	// P pauses / unpauses (only once per press, and not on the game over screen)
	bool pauseKey = Keyboard::isKeyPressed(Keyboard::P);
	if (pauseKey && !pauseHeld && endTimer <= 0) {
		paused = !paused;
		bannerText.setString(paused ? "PAUSED" : "");
	}
	pauseHeld = pauseKey;
}

void PlayState::update(float frameTime) {
	if (paused) return; // nothing moves while paused

	if (endTimer > 0) {
		endTimer -= frameTime;
		if (endTimer <= 0) gsManager.changeState(new MenuState(gsManager));
		return;
	}

	dayNight.update(frameTime);
	weather.update(frameTime);
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
	hud += "   " + string(dayNight.getTimeName()) + ", " + weather.getWeather().getName();
	hud += "\nA/D move  W jump  Space fire  Up/Down aim  T grenade  R knife  Q power up  Z switch  E enter vehicle  U exit  P pause  Esc menu";
	hudText.setString(hud);
}


void PlayState::render(RenderWindow& w) {
	RectangleShape sky(Vector2f((float)Camera::screenW, (float)Camera::screenH));
	// clouds: pull the sky colour towards grey (its own average), works for both day and night
	Color skyColor = dayNight.getSkyColor();
	float cloudiness = weather.getWeather().getCloudiness();
	float grey = (skyColor.r + skyColor.g + skyColor.b) / 3.f;
	skyColor.r = (Uint8)(skyColor.r + (grey - skyColor.r) * cloudiness);
	skyColor.g = (Uint8)(skyColor.g + (grey - skyColor.g) * cloudiness);
	skyColor.b = (Uint8)(skyColor.b + (grey - skyColor.b) * cloudiness);
	sky.setFillColor(skyColor);
	w.draw(sky);
	if (cloudiness == 0) dayNight.drawSunAndMoon(w); // hidden behind the clouds when it rains or snows, drawn before the level so hills cover it

	world.render(w, camera);
	entityManager.render(w, camera);
	weather.render(w); // rain in front of the level, the night layer below darkens it too

	// darken everything at night (drawn before the hud so the text stays readable)
	RectangleShape night(Vector2f((float)Camera::screenW, (float)Camera::screenH));
	night.setFillColor(dayNight.getNightOverlay());
	w.draw(night);

	w.draw(hudText);

	if (endTimer > 0 || paused) {
		FloatRect b = bannerText.getLocalBounds();
		bannerText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
		bannerText.setPosition(Camera::screenW / 2.f, Camera::screenH / 2.f);
		w.draw(bannerText);
	}
}

#include "PlayState.h"
#include "MenuState.h"
#include "SoundManager.h"
#include <string>
using namespace std;

void PlayState::enter() {
	levelManager.loadLevel(1, world, entityManager);
	SoundManager::playMusic();

	font.loadFromFile("TEXT/font1.ttf");
	hudText.setFont(font);
	hudText.setCharacterSize(22);
	hudText.setFillColor(Color::White);
	hudText.setOutlineColor(Color::Black);
	hudText.setOutlineThickness(2);
	hudText.setPosition(16, 12);

	scoreText.setFont(font);
	scoreText.setCharacterSize(40);
	scoreText.setFillColor(Color(255, 230, 80));
	scoreText.setOutlineColor(Color::Black);
	scoreText.setOutlineThickness(3);

	popupText.setFont(font);
	popupText.setCharacterSize(24);
	popupText.setOutlineThickness(2);

	bannerText.setFont(font);
	bannerText.setCharacterSize(72);
	bannerText.setFillColor(Color(255, 210, 40));
	bannerText.setOutlineColor(Color::Black);
	bannerText.setOutlineThickness(4);
}

void PlayState::exit() {
	SoundManager::stopMusic(); // leaving the game (menu / next state): music off
}

void PlayState::handleInput() {
	// escape leaves the game and goes back to the main menu (the score still counts for the high score)
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Score::saveHighScore(score.getPoints());
		gsManager.changeState(new MenuState(gsManager));
	}

	// P pauses / unpauses (only once per press, and not on the game over screen)
	bool pauseKey = Keyboard::isKeyPressed(Keyboard::P);
	if (pauseKey && !pauseHeld && endTimer <= 0) {
		paused = !paused;
		bannerText.setString(paused ? "PAUSED" : "");
	}
	pauseHeld = pauseKey;

	// M turns all sound on / off
	bool muteKey = Keyboard::isKeyPressed(Keyboard::M);
	if (muteKey && !muteHeld) SoundManager::toggleMute();
	muteHeld = muteKey;
}

void PlayState::update(float frameTime) {
	if (paused) return; // nothing moves while paused

	if (endTimer > 0) {
		endTimer -= frameTime;
		if (endTimer <= 0) gsManager.changeState(new MenuState(gsManager));
		return;
	}

	dayNight.update(frameTime);
	score.update(frameTime);
	weather.update(frameTime);
	player.handleInput(frameTime, world, entityManager);
	entityManager.update(frameTime, world);

	PlayerSoldier* currCharacter = player.getActive();
	camera.follow(currCharacter->getX(), currCharacter->getY());
	float shake = entityManager.takeShake();
	if (shake > 0) camera.shake(shake, 0.35f);
	if (lastHp >= 0 && currCharacter->getHp() < lastHp) {
		camera.shake(4, 0.2f); // small shake and a sound when you get hit
		SoundManager::play(SoundManager::HURT, 70);
	}
	lastHp = currCharacter->getHp();
	camera.updateShake(frameTime);

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
		Score::saveHighScore(score.getPoints());
		SoundManager::stopMusic();
		SoundManager::play(SoundManager::GAME_OVER);
	}
	else if (levelManager.isLevelFinished(entityManager)) {
		bannerText.setString("MISSION COMPLETE");
		endTimer = 3;
		Score::saveHighScore(score.getPoints());
		SoundManager::stopMusic();
		SoundManager::play(SoundManager::MISSION_COMPLETE);
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
	// score in the top right, with the combo multiplier while a combo is going
	std::string scoreLine = "SCORE " + to_string(score.getPoints());
	if (score.comboActive()) scoreLine += "   x" + to_string(score.getMultiplier()) + " COMBO!";
	scoreText.setString(scoreLine);
	FloatRect sb = scoreText.getLocalBounds();
	scoreText.setPosition(Camera::screenW - sb.width - 30, 60);

	hud += "\nA/D move  W jump  Space fire  Up/Down aim  T grenade  R knife  Q power up  Z switch  E enter vehicle  U exit  P pause  M mute  Esc menu";
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

	score.renderPopups(w, camera, popupText);
	w.draw(hudText);
	w.draw(scoreText);

	if (endTimer > 0 || paused) {
		FloatRect b = bannerText.getLocalBounds();
		bannerText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
		bannerText.setPosition(Camera::screenW / 2.f, Camera::screenH / 2.f);
		w.draw(bannerText);
	}
}

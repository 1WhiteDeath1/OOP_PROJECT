#include "PlayState.h"
#include "MenuState.h"
#include "SoundManager.h"
#include <string>
using namespace std;

void PlayState::enter() {
	levelManager.loadLevel(mission, world, entityManager);
	// mission 1 in the morning, mission 2 in the late afternoon (sunset comes soon), the boss at night
	if (mission == 1) dayNight.setTime(5);
	else if (mission == 2) dayNight.setTime(38);
	else dayNight.setTime(48); // the boss fight starts at sunset and goes into the night
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

	introText.setFont(font);
	introText.setCharacterSize(56);
	introText.setFillColor(Color::White);
	introText.setOutlineColor(Color::Black);
	introText.setOutlineThickness(4);
	const char* names[3] = { "MOUNTAIN PASS", "COASTLINE", "REBEL BASE" };
	introText.setString("MISSION " + to_string(mission) + "\n" + names[mission - 1]);
	FloatRect ib = introText.getLocalBounds();
	introText.setOrigin(ib.left + ib.width / 2.f, ib.top + ib.height / 2.f);
	introText.setPosition(Camera::screenW / 2.f, 300);

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
		if (endTimer <= 0) {
			if (missionDone) gsManager.changeState(new PlayState(gsManager, mission + 1, score.getPoints())); // next mission, keep the score
			else gsManager.changeState(new MenuState(gsManager));
		}
		return;
	}

	if (introTimer > 0) introTimer -= frameTime;
	dayNight.update(frameTime);
	score.update(frameTime);
	weather.update(frameTime);
	player.handleInput(frameTime, world, entityManager);
	entityManager.update(frameTime, world);
	levelManager.update(frameTime, player.getActive()->getX(), world, entityManager); // enemy waves

	PlayerSoldier* currCharacter = player.getActive();
	// look ahead: the camera slides to show more of the screen in front of the player, so enemies
	// are seen earlier. it eases over (a third of the way each 0.1s) instead of jumping when turning
	float lookTarget = currCharacter->isFacingRight() ? 250.f : -250.f;
	float ease = frameTime * 3;
	if (ease > 1) ease = 1;
	lookAhead += (lookTarget - lookAhead) * ease;
	camera.follow(currCharacter->getX() + lookAhead, currCharacter->getY());
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
	else if (levelManager.isLevelFinished(entityManager, currCharacter->getX())) {
		missionDone = levelManager.nextLevelAvalaible();
		if (missionDone) bannerText.setString("MISSION " + to_string(mission) + " COMPLETE");
		else bannerText.setString("YOU WIN!\nFINAL SCORE " + to_string(score.getPoints()));
		endTimer = missionDone ? 3.f : 6.f;
		Score::saveHighScore(score.getPoints());
		SoundManager::stopMusic();
		SoundManager::play(SoundManager::MISSION_COMPLETE);
	}

	//hud
	string hud = "MISSION " + to_string(mission) + "   " + string(currCharacter->getName()) +
		"   HP " + to_string(currCharacter->getHp()) +
		"   Lives " + to_string(currCharacter->getLives()) +
		"   Grenades " + to_string(currCharacter->getGrenadeCount());
	Weapon* special = currCharacter->getActiveWeaponSlot(1);
	if (special && special->hasAmmo()) hud += "   Ammo " + to_string(special->getAmmo());
	// power up: its name while it is on, otherwise ready / seconds until it is ready again
	if (currCharacter->getPowerUpActive()) hud += "   " + string(currCharacter->getPowerUpName()) + "!";
	else if (currCharacter->getPowerUpCooldown() > 0) hud += "   Power up in " + to_string((int)currCharacter->getPowerUpCooldown() + 1) + "s";
	else hud += "   Q: " + string(currCharacter->getPowerUpName()) + " ready";
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


void PlayState::drawGoalFlag(RenderWindow& w) {
	int col = levelManager.getGoalColumn();
	if (col <= 0) return; // the boss mission has no flag
	float groundY = world.surfaceY(col);
	float poleX = camera.toScreenX(col * (float)World::CELL + 32);

	RectangleShape pole(Vector2f(8, 220));
	pole.setFillColor(Color(200, 200, 200));
	pole.setPosition(poleX, camera.toScreenY(groundY - 220));
	w.draw(pole);

	// a red triangle flag waving at the top of the pole
	ConvexShape flag(3);
	flag.setPoint(0, Vector2f(0, 0));
	flag.setPoint(1, Vector2f(90, 30));
	flag.setPoint(2, Vector2f(0, 60));
	flag.setFillColor(Color(220, 40, 40));
	flag.setPosition(poleX + 8, camera.toScreenY(groundY - 215));
	w.draw(flag);
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
	drawGoalFlag(w);
	entityManager.render(w, camera);
	weather.render(w); // rain in front of the level, the night layer below darkens it too

	// darken everything at night (drawn before the hud so the text stays readable)
	RectangleShape night(Vector2f((float)Camera::screenW, (float)Camera::screenH));
	night.setFillColor(dayNight.getNightOverlay());
	w.draw(night);

	score.renderPopups(w, camera, popupText);
	w.draw(hudText);
	w.draw(scoreText);

	// boss health bar across the top
	const Enemy* boss = entityManager.getBoss();
	if (boss) {
		RectangleShape back(Vector2f(604, 28)), bar(Vector2f(600.f * boss->getHp() / boss->getMaXHp(), 24));
		back.setFillColor(Color(0, 0, 0, 180));
		back.setPosition(Camera::screenW / 2.f - 302, 110);
		bar.setFillColor(Color(220, 40, 30));
		bar.setPosition(Camera::screenW / 2.f - 300, 112);
		w.draw(back);
		w.draw(bar);
		popupText.setString("REBEL TANK");
		popupText.setFillColor(Color::White);
		popupText.setOutlineColor(Color::Black);
		FloatRect nb = popupText.getLocalBounds();
		popupText.setOrigin(nb.width / 2, 0);
		popupText.setPosition(Camera::screenW / 2.f, 78);
		w.draw(popupText);
	}

	if (introTimer > 0 && endTimer <= 0) w.draw(introText);

	if (endTimer > 0 || paused) {
		FloatRect b = bannerText.getLocalBounds();
		bannerText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
		bannerText.setPosition(Camera::screenW / 2.f, Camera::screenH / 2.f);
		w.draw(bannerText);
	}
}

#include "PlayState.h"
#include "MenuState.h"
#include "SoundManager.h"
#include "Aim.h"
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

	helpText.setFont(font);
	helpText.setCharacterSize(18);
	helpText.setOutlineColor(Color::Black);
	helpText.setOutlineThickness(2);

	barText.setFont(font);
	barText.setCharacterSize(20);
	barText.setFillColor(Color::White);
	barText.setOutlineColor(Color::Black);
	barText.setOutlineThickness(2);

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

	// Tab switches aiming between the arrow keys and the mouse
	bool aimKey = Keyboard::isKeyPressed(Keyboard::Tab);
	if (aimKey && !aimToggleHeld) Aim::mouseMode = !Aim::mouseMode;
	aimToggleHeld = aimKey;
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
	barBlink += frameTime;
	if (helpTimer > 0) helpTimer -= frameTime;
	weather.update(frameTime);
	player.handleInput(frameTime, world, entityManager, camera);
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
		"   Lives " + to_string(currCharacter->getLives()) +
		"   Grenades " + to_string(currCharacter->getGrenadeCount());
	Weapon* special = currCharacter->getActiveWeaponSlot(1);
	if (special && special->hasAmmo()) hud += "   Ammo " + to_string(special->getAmmo());
	// power up: its name while it is on, otherwise ready / seconds until it is ready again
	if (currCharacter->getPowerUpActive()) hud += "   " + string(currCharacter->getPowerUpName()) + "!";
	else if (currCharacter->getPowerUpCooldown() > 0) hud += "   Power up in " + to_string((int)currCharacter->getPowerUpCooldown() + 1) + "s";
	else hud += "   Q: " + string(currCharacter->getPowerUpName()) + " ready";
	hud += "   Enemies " + to_string(entityManager.getEnemyCount());
	hud += "   " + string(dayNight.getTimeName()) + ", " + weather.getWeather().getName();
	// score in the top right, with the combo multiplier while a combo is going
	std::string scoreLine = "SCORE " + to_string(score.getPoints());
	if (score.comboActive()) scoreLine += "   x" + to_string(score.getMultiplier()) + " COMBO!";
	scoreText.setString(scoreLine);
	FloatRect sb = scoreText.getLocalBounds();
	scoreText.setPosition(Camera::screenW - sb.width - 30, 60);

	hud += Aim::mouseMode ? "   Aim: MOUSE" : "   Aim: ARROWS";
	hudText.setString(hud);
	if (Aim::mouseMode)
		helpText.setString("A/D move   W jump   MOUSE aim   CLICK/SPACE fire   T grenade   R knife   Q power up   Z switch   E/U vehicle   TAB arrow aim   P pause   M mute   ESC menu");
	else
		helpText.setString("A/D move   W jump   SPACE fire   UP/DOWN aim   LEFT/RIGHT turn   T grenade   R knife   Q power up   Z switch   E/U vehicle   TAB mouse aim   P pause   M mute   ESC menu");
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

// one bar in the top right under the score: dark back, coloured part for the hp left, "label hp/max" on it
void PlayState::drawBar(RenderWindow& w, float y, const std::string& label, int hp, int maxHp, Color full) {
	const float barW = 320, barH = 26;
	float x = Camera::screenW - barW - 30;
	float part = maxHp > 0 ? (float)hp / maxHp : 0;
	if (part < 0) part = 0;
	if (part > 1) part = 1;

	RectangleShape back(Vector2f(barW + 4, barH + 4));
	back.setPosition(x - 2, y - 2);
	back.setFillColor(Color(0, 0, 0, 180));
	back.setOutlineColor(Color(255, 255, 255, 200));
	back.setOutlineThickness(1);
	w.draw(back);

	// the colour goes yellow under half and red under a quarter (and blinks when almost dead)
	Color c = full;
	if (part < 0.25f) c = Color(220, 40, 30);
	else if (part < 0.5f) c = Color(240, 200, 40);
	if (part < 0.25f && (int)(barBlink * 4) % 2 == 0) c = Color(255, 120, 100);
	RectangleShape bar(Vector2f(barW * part, barH));
	bar.setPosition(x, y);
	bar.setFillColor(c);
	w.draw(bar);

	barText.setString(label + "  " + to_string(hp) + " / " + to_string(maxHp));
	FloatRect tb = barText.getLocalBounds();
	barText.setOrigin(tb.left + tb.width / 2, tb.top + tb.height / 2);
	barText.setPosition(x + barW / 2, y + barH / 2);
	w.draw(barText);
}

void PlayState::drawHealthBars(RenderWindow& w) {
	PlayerSoldier* s = player.getActive();
	if (!s) return;
	drawBar(w, 112, s->getName(), s->getHp(), s->getMaXHp(), Color(60, 200, 70));
	// while driving, the vehicle takes the hits, so show its health too
	if (player.isPiloting())
		drawBar(w, 148, "VEHICLE", player.getVehicle()->getHp(), player.getVehicle()->getMaXHp(), Color(70, 150, 230));
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

	// where the next shot will go, drawn over the night layer so it can always be seen
	if (endTimer <= 0) player.renderAim(w, camera, world);

	score.renderPopups(w, camera, popupText);
	// a see-through dark panel behind the hud text so it reads on any sky
	FloatRect hb = hudText.getGlobalBounds();
	RectangleShape hudBack(Vector2f(hb.width + 24, hb.height + 18));
	hudBack.setPosition(hb.left - 12, hb.top - 9);
	hudBack.setFillColor(Color(0, 0, 0, 110));
	w.draw(hudBack);
	w.draw(hudText);

	// the controls along the bottom: the first 10 seconds (fading out over the last one) and while paused
	float helpAlpha = paused ? 1.f : (helpTimer > 1 ? 1.f : (helpTimer > 0 ? helpTimer : 0.f));
	if (helpAlpha > 0 && endTimer <= 0) {
		helpText.setFillColor(Color(255, 255, 255, (Uint8)(255 * helpAlpha)));
		helpText.setOutlineColor(Color(0, 0, 0, (Uint8)(255 * helpAlpha)));
		FloatRect cb = helpText.getLocalBounds();
		helpText.setOrigin(cb.left + cb.width / 2, 0);
		helpText.setPosition(Camera::screenW / 2.f, Camera::screenH - 40.f);
		RectangleShape helpBack(Vector2f(cb.width + 40, 34));
		helpBack.setOrigin((cb.width + 40) / 2, 0);
		helpBack.setPosition(Camera::screenW / 2.f, Camera::screenH - 46.f);
		helpBack.setFillColor(Color(0, 0, 0, (Uint8)(130 * helpAlpha)));
		w.draw(helpBack);
		w.draw(helpText);
	}
	w.draw(scoreText);
	drawHealthBars(w);

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

	if (introTimer > 0 && endTimer <= 0) {
		// the mission title fades in over the first half second and out over the last one
		float a = introTimer > 2.5f ? (3 - introTimer) / 0.5f : (introTimer < 1 ? introTimer : 1.f);
		if (a < 0) a = 0;
		if (a > 1) a = 1;
		introText.setFillColor(Color(255, 255, 255, (Uint8)(255 * a)));
		introText.setOutlineColor(Color(0, 0, 0, (Uint8)(255 * a)));
		w.draw(introText);
	}

	if (endTimer > 0 || paused) {
		FloatRect b = bannerText.getLocalBounds();
		bannerText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
		bannerText.setPosition(Camera::screenW / 2.f, Camera::screenH / 2.f);
		// darken the game behind the banner
		RectangleShape dim(Vector2f((float)Camera::screenW, (float)Camera::screenH));
		dim.setFillColor(Color(0, 0, 0, 120));
		w.draw(dim);
		w.draw(bannerText);
	}

	// aiming with the mouse: hide the arrow and draw a crosshair at the mouse instead
	bool crosshair = Aim::usingMouse() && !paused && endTimer <= 0;
	w.setMouseCursorVisible(!crosshair);
	if (crosshair) {
		float mx = (float)Aim::mouse.x, my = (float)Aim::mouse.y;
		CircleShape ring(13.f);
		ring.setOrigin(13.f, 13.f);
		ring.setPosition(mx, my);
		ring.setFillColor(Color::Transparent);
		ring.setOutlineColor(Color::White);
		ring.setOutlineThickness(2);
		w.draw(ring);
		RectangleShape line(Vector2f(10, 2));
		line.setFillColor(Color::White);
		line.setOrigin(5, 1);
		const float offX[4] = { -20, 20, 0, 0 }, offY[4] = { 0, 0, -20, 20 };
		for (int k = 0; k < 4; k++) {
			line.setRotation(k < 2 ? 0.f : 90.f);
			line.setPosition(mx + offX[k], my + offY[k]);
			w.draw(line);
		}
		CircleShape centre(2.f);
		centre.setOrigin(2.f, 2.f);
		centre.setPosition(mx, my);
		centre.setFillColor(Color(255, 80, 60));
		w.draw(centre);
	}

}

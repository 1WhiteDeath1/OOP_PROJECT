#include "MenuState.h"
#include "PlayState.h"
#include "Score.h"
#include "Aim.h"
#include <string>
#include <cmath>

MenuState::MenuState(GameStateManager& gsm) : gsManager(gsm) {}

void MenuState::enter() {
	bgTex.loadFromFile("25I-0504_25I-0644_Assets/menu_bg.png");
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0, 0);
	bgSprite.setScale(1600.f / bgTex.getSize().x, 900.f / bgTex.getSize().y);

	survivalTex.loadFromFile("25I-0504_25I-0644_Assets/menu_survival.png");
	survivalSprite.setTexture(survivalTex);
	survivalSprite.setOrigin(survivalTex.getSize().x / 2.f, survivalTex.getSize().y / 2.f);

	exitTex.loadFromFile("25I-0504_25I-0644_Assets/menu_exit.png");
	exitSprite.setTexture(exitTex);
	exitSprite.setOrigin(exitTex.getSize().x / 2.f, exitTex.getSize().y / 2.f);

	// best score so far, from highscore.txt
	font.loadFromFile("TEXT/font1.ttf");
	highScoreText.setFont(font);
	highScoreText.setCharacterSize(36);
	highScoreText.setFillColor(Color(255, 230, 80));
	highScoreText.setOutlineColor(Color::Black);
	highScoreText.setOutlineThickness(3);
	highScoreText.setString("HIGH SCORE  " + std::to_string(Score::loadHighScore()));
	FloatRect b = highScoreText.getLocalBounds();
	highScoreText.setOrigin(b.width / 2, 0);
	highScoreText.setPosition(800, 780);

	aimText.setFont(font);
	aimText.setCharacterSize(26);
	aimText.setFillColor(Color::White);
	aimText.setOutlineColor(Color::Black);
	aimText.setOutlineThickness(2);

	hintText.setFont(font);
	hintText.setCharacterSize(18);
	hintText.setFillColor(Color(210, 210, 210));
	hintText.setOutlineColor(Color::Black);
	hintText.setOutlineThickness(1);
	hintText.setString("UP / DOWN or MOUSE  select      ENTER or CLICK  start      ESC  quit");
	FloatRect hb = hintText.getLocalBounds();
	hintText.setOrigin(hb.width / 2, 0);
	hintText.setPosition(800, 872);
	lastMouse = Aim::mouse;


}

void MenuState::handleInput() {
	// Tab: aim with the arrow keys or with the mouse (can also be switched during the game)
	bool tabKey = Keyboard::isKeyPressed(Keyboard::Tab);
	if (tabKey && !tabHeld) Aim::mouseMode = !Aim::mouseMode;
	tabHeld = tabKey;

	if (!keyHeld) {
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			choice = choice <= 0 ? 0 : choice - 1;
			keyHeld = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			choice = choice >= 1 ? 1 : choice + 1;
			keyHeld = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			keyHeld = true;
			gsManager.requestQuit();
		}
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			keyHeld = true;
			choose();
			return;
		}
	}

	// the mouse: moving over a button picks it, clicking it presses it
	bool overSurvival = survivalSprite.getGlobalBounds().contains((float)Aim::mouse.x, (float)Aim::mouse.y);
	bool overExit = exitSprite.getGlobalBounds().contains((float)Aim::mouse.x, (float)Aim::mouse.y);
	if (Aim::focused && Aim::mouse != lastMouse) {
		if (overSurvival) choice = 0;
		else if (overExit) choice = 1;
	}
	lastMouse = Aim::mouse;
	bool click = Aim::focused && Mouse::isButtonPressed(Mouse::Left);
	if (click && !clickHeld && (overSurvival || overExit)) {
		clickHeld = true;
		choose();
		return;
	}
	clickHeld = click;

	if (!Keyboard::isKeyPressed(Keyboard::Up) &&
		!Keyboard::isKeyPressed(Keyboard::Down) &&
		!Keyboard::isKeyPressed(Keyboard::Return) &&
		!Keyboard::isKeyPressed(Keyboard::Escape)) {   // so the escape that left a game doesn't also quit
		keyHeld = false;
	}
}

void MenuState::choose() {
	if (choice == 0) gsManager.changeState(new PlayState(gsManager));
	else gsManager.requestQuit();
}

void MenuState::update(float dt) {
	clock += dt;
	if (fade > 0) fade -= dt * 2; // half a second from black
	if (fade < 0) fade = 0;
	// the picked button grows to double size and gently pulses, the other one shrinks back
	for (int i = 0; i < 2; i++) {
		float target = (choice == i) ? 0.2f * (1 + 0.03f * std::sin(clock * 4)) : 0.1f;
		scales[i] += (target - scales[i]) * (dt * 12 > 1 ? 1 : dt * 12);
	}
}

void MenuState::render(RenderWindow& w) {
	w.setMouseCursorVisible(true);
	w.draw(bgSprite);

	// a dark strip along the bottom so the text there is easy to read on the picture
	VertexArray shade(Quads, 4);
	shade[0].position = Vector2f(0, 700);    shade[0].color = Color(0, 0, 0, 0);
	shade[1].position = Vector2f(1600, 700); shade[1].color = Color(0, 0, 0, 0);
	shade[2].position = Vector2f(1600, 900); shade[2].color = Color(0, 0, 0, 210);
	shade[3].position = Vector2f(0, 900);    shade[3].color = Color(0, 0, 0, 210);
	w.draw(shade);

	survivalSprite.setScale(scales[0], scales[0]);
	exitSprite.setScale(scales[1], scales[1]);
	// the button that isn't picked is a bit darker
	survivalSprite.setColor(choice == 0 ? Color::White : Color(160, 160, 160));
	exitSprite.setColor(choice == 1 ? Color::White : Color(160, 160, 160));


	float choicesXPOS = 800;
	survivalSprite.setPosition(choicesXPOS, 380);
	exitSprite.setPosition(choicesXPOS, 620);

	w.draw(survivalSprite);
	w.draw(exitSprite);
	w.draw(highScoreText);

	aimText.setString(Aim::mouseMode ? "AIM: MOUSE   (Tab to change)" : "AIM: ARROW KEYS   (Tab to change)");
	FloatRect ab = aimText.getLocalBounds();
	aimText.setOrigin(ab.width / 2, 0);
	aimText.setPosition(800, 832);
	w.draw(aimText);
	w.draw(hintText);

	if (fade > 0) {
		RectangleShape black(Vector2f(1600, 900));
		black.setFillColor(Color(0, 0, 0, (Uint8)(255 * fade)));
		w.draw(black);
	}

}
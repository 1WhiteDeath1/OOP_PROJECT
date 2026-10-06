#include "MenuState.h"
#include "PlayState.h"
#include "Score.h"
#include "Aim.h"
#include <string>

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
	highScoreText.setPosition(800, 790);

	aimText.setFont(font);
	aimText.setCharacterSize(26);
	aimText.setFillColor(Color::White);
	aimText.setOutlineColor(Color::Black);
	aimText.setOutlineThickness(2);


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
			if (choice == 0) gsManager.changeState(new PlayState(gsManager));
			else gsManager.requestQuit();
		}
	}

	if (!Keyboard::isKeyPressed(Keyboard::Up) &&
		!Keyboard::isKeyPressed(Keyboard::Down) &&
		!Keyboard::isKeyPressed(Keyboard::Return) &&
		!Keyboard::isKeyPressed(Keyboard::Escape)) {   // so the escape that left a game doesn't also quit
		keyHeld = false;
	}
}

void MenuState::render(RenderWindow& w) {
	w.draw(bgSprite);


	survivalSprite.setScale(choice == 0 ? 0.2 : 0.1, choice == 0 ? 0.2 : 0.1);
	exitSprite.setScale(choice == 1 ? 0.2 : 0.1, choice == 1 ? 0.2 : 0.1);


	float choicesXPOS = 800;
	survivalSprite.setPosition(choicesXPOS, 380);
	exitSprite.setPosition(choicesXPOS, 620);

	w.draw(survivalSprite);
	w.draw(exitSprite);
	w.draw(highScoreText);

	aimText.setString(Aim::mouseMode ? "AIM: MOUSE   (Tab to change)" : "AIM: ARROW KEYS   (Tab to change)");
	FloatRect ab = aimText.getLocalBounds();
	aimText.setOrigin(ab.width / 2, 0);
	aimText.setPosition(800, 845);
	w.draw(aimText);

}
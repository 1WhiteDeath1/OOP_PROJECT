#include "MenuState.h"
#include "PlayState.h"

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


}

void MenuState::handleInput() {
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

}
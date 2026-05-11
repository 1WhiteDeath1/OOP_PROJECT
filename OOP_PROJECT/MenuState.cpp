#include "MenuState.h"
#include "PlayState.h"

MenuState::MenuState(GameStateManager& gsm) : gsManager(gsm) {}

void MenuState::enter() {
	bgTex.loadFromFile("25I-0504_25I-0644_Assets/menu_bg.png");
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0, 0);
	bgSprite.setScale(1600 / 2816, 900 / 1536);

	survivalTex.loadFromFile("25I-0504_25I-0644_Assets/menu_survival.png");
	survivalSprite.setTexture(survivalTex);

	exitTex.loadFromFile("25I-0504_25I-0644_Assets/menu_exit.png");
	exitSprite.setTexture(exitTex);


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
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			keyHeld = true;
			if (choice == 0) gsManager.changeState(new PlayState(gsManager));
			else {
			}
		}
	}

	if (!Keyboard::isKeyPressed(Keyboard::Up) &&
		!Keyboard::isKeyPressed(Keyboard::Down) &&
		!Keyboard::isKeyPressed(Keyboard::Return)) {
		keyHeld = false;
	}
}

void MenuState::render(RenderWindow& w) {
	bgSprite.setScale(0.6, 0.6);
	w.draw(bgSprite);


	survivalSprite.setScale(choice == 0 ? 0.2 : 0.1, choice == 0 ? 0.2 : 0.1);
	exitSprite.setScale(choice == 1 ? 0.2 : 0.1, choice == 1 ? 0.2 : 0.1);


	float choicesXPOS = 800;
	survivalSprite.setPosition(choicesXPOS - 500 / 2, 300);
	exitSprite.setPosition(choicesXPOS - 500 / 2, 550);

	w.draw(survivalSprite);
	w.draw(exitSprite);

}
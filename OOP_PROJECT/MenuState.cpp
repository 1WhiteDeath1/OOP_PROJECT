#include "MenuState.h"
#include "PlayState.h"

MenuState::MenuState(GameStateManager& gsm) : gsManager(gsm) {}

void MenuState::enter() {
	font.loadFromFile("C:/Windows/Fonts/arial.ttf");

	title.setFont(font);
	title.setString("MENTAL SLUG");
	title.setCharacterSize(72);
	title.setFillColor(Color::Yellow);
	title.setPosition(550, 200);

	option1.setFont(font);
	option1.setString("Survival Mode");
	option1.setCharacterSize(36);
	option1.setPosition(620, 390);

	option2.setFont(font);
	option2.setString("Campaign Mode");
	option2.setCharacterSize(36);
	option2.setPosition(620, 490);

}

void MenuState::handleInput() {
	if (!keyHeld) {
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			choice= 0;
			keyHeld = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			choice = 1;
			keyHeld = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			gsManager.changeState(new PlayState(gsManager));
		}
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
		keyHeld = false;
	}
}

void MenuState::render(RenderWindow& w) {
	w.draw(title);
	option1.setFillColor(choice == 0 ? Color::White : Color(150, 150, 150));
	option2.setFillColor(choice == 1 ? Color::White : Color(150, 150, 150));
	w.draw(option1);
	w.draw(option2);

}
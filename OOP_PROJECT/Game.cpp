#include "Game.h"
#include "MenuState.h"
#include "Aim.h"
#include <string>
#include <ctime>
using namespace sf;

Game::Game() : window(VideoMode(1600, 900), "Metal Slug", Style::Close) {
	window.setFramerateLimit(60);
	stateManager.changeState(new MenuState(stateManager));
}

void Game::run() {
	while (window.isOpen()) {
		float frameTime = clock.restart().asSeconds();
		// a long frame (e.g. while a level loads) would make things fall through the ground
		if (frameTime > 1.f / 30.f) frameTime = 1.f / 30.f;

		Event ev;
		bool takeScreenshot = false;
		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed) window.close();
			if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::F12) takeScreenshot = true;
		}

		// escape is handled by the states now (back to menu while playing, quit from the menu)
		if (stateManager.shouldQuit()) {
			window.close();
			break;
		}

		// the mouse position for aiming, turned into game pixels (in case the window is shown at another size)
		Vector2f m = window.mapPixelToCoords(Mouse::getPosition(window));
		Aim::mouse = Vector2i((int)m.x, (int)m.y);
		Aim::focused = window.hasFocus();

		stateManager.handleInput();
		stateManager.update(frameTime);
		window.clear();
		stateManager.render(window);

		// F12 saves what is on the screen as screenshot_<time>.png next to the game
		if (takeScreenshot) {
			Texture shot;
			shot.create(window.getSize().x, window.getSize().y);
			shot.update(window);
			shot.copyToImage().saveToFile("screenshot_" + std::to_string(std::time(nullptr)) + "_" + std::to_string(clock.getElapsedTime().asMicroseconds()) + ".png");
		}
		window.display();
	}
}
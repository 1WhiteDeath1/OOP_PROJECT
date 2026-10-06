#include "Game.h"
#include "MenuState.h"
#include "Aim.h"
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
		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed) window.close();
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
		window.display();
	}
}
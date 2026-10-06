#pragma once
#include <SFML/Window.hpp>

// aiming settings shared by the player, the vehicles and the menu
struct Aim {
	static inline bool mouseMode = false; // Tab switches between aiming with the arrow keys and with the mouse
	static inline sf::Vector2i mouse;     // mouse position in the window, Game sets it every frame
	static inline bool focused = true;    // the game window is the active one (so clicks in other windows don't shoot)

	static bool usingMouse() { return mouseMode && focused; }

	// space always fires, the left mouse button too while aiming with the mouse
	static bool firePressed() {
		return sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
			(usingMouse() && sf::Mouse::isButtonPressed(sf::Mouse::Left));
	}
};

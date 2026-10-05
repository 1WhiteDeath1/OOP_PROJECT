#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include "Game.h"
using namespace sf;
using namespace std;

int main() {
	srand((unsigned)time(nullptr)); // a different map, waves and drops every time the game runs
	Game game;
	game.run();
	return 0;
}

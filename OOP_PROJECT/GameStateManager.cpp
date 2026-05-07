#include "GameStateManager.h"


void GameStateManager::changeState(GameState* newState) {
    if (curr) { curr->exit(); delete curr; }
    curr = newState;
    curr->enter();
}
void GameStateManager::handleInput() { if (curr) curr->handleInput(); }
void GameStateManager::update(float dt) { if (curr) curr->update(dt); }
void GameStateManager::render(sf::RenderWindow& w) { if (curr) curr->render(w); }
GameStateManager::~GameStateManager() { if (curr) { curr->exit(); delete curr; } }

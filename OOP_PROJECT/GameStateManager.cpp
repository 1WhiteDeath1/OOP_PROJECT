#include "GameStateManager.h"


void GameStateManager::changeState(GameState* newState) {
    delete pending;
    pending = newState;
    if (!curr) applyPending();
}
void GameStateManager::applyPending() {
    if (!pending) return;
    if (curr) { curr->exit(); delete curr; }
    curr = pending;
    pending = nullptr;
    curr->enter();
}
void GameStateManager::handleInput() { applyPending(); if (curr) curr->handleInput(); }
void GameStateManager::update(float dt) { if (curr) curr->update(dt); }
void GameStateManager::render(sf::RenderWindow& w) { if (curr) curr->render(w); }
GameStateManager::~GameStateManager() { delete pending; if (curr) { curr->exit(); delete curr; } }

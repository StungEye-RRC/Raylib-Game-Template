#include "GameManager.hpp"

void GameManager::StartGame() {
    gameStarted = true;
}

void GameManager::AddScore(int points) {
    if (gameStarted) {
        score += points;
        timeSinceLastAction = 0.0f;  // Reset idle timer on action
    }
}

void GameManager::UpdateTimer(float deltaTime) {
    if (gameStarted) {
        timeRemaining -= deltaTime;
        if (timeRemaining < 0.0f) {
            timeRemaining = 0.0f;
        }

        // Also update idle timer
        timeSinceLastAction += deltaTime;
    }
}

bool GameManager::IsTimeUp() const {
    return gameStarted && timeRemaining <= 0.0f;
}

bool GameManager::IsRunning() const {
    return gameStarted && timeSinceLastAction < IDLE_TIMEOUT;
}

void GameManager::Reset() {
    // Replace this manager with a brand new one, so every member goes back
    // to its starting value from GameManager.hpp.
    *this = GameManager{};
}

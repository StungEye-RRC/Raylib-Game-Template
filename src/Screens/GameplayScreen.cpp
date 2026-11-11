#include "GameplayScreen.hpp"
#include "MainMenuScreen.hpp"
#include "GameOverScreen.hpp"
#include "PauseOverlay.hpp"
#include "../GameContext.hpp"
#include "../GameManager.hpp"
#include <raylib.h>
#include <iostream>

GameplayScreen::GameplayScreen(GameContext& context)
    : FullScreen{ context } {
    // Every new game starts clean, however we got to this screen.
    GetContext().game.Reset();
}

// Update is acting as the Controller and the GameManager is the Model.
void GameplayScreen::Update() {
    auto& game = GetContext().game;

    // If the game hasn't started yet, wait for SPACE to start
    // or ESC to return to main menu.
    if (!game.HasStarted()) {
        if (IsKeyPressed(KEY_SPACE)) {
            game.StartGame();
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            RequestScreenChange<MainMenuScreen>();
        }

        return; // Don't do anything else until game starts.
    }

    // If we've made it here then the game has started!
    if (IsKeyPressed(KEY_P)) {
        RequestScreenChange<PauseOverlay>();
        return;
    }

    timeSinceFootstep += GetFrameTime();

    if (IsKeyPressed(KEY_SPACE)) {
        game.AddScore(1);  // This resets idle timer internally

        // Play a random footstep, unless the last one was too recent.
        if (timeSinceFootstep >= FOOTSTEP_COOLDOWN) {
            int lastIndex{ static_cast<int>(footsteps.size()) - 1 };
            PlaySound(footsteps[GetRandomValue(0, lastIndex)]);
            // Offset the next footstep by -0.1 to 0.1 seconds. 
            timeSinceFootstep = GetRandomValue(-1,1) / 10.0f;
        }

        // Cycle through running animation frames 1, 2, 3
        if (zombieIndex < 1 || zombieIndex >= 3) {
            zombieIndex = 1;  // Start running animation
        } else {
            zombieIndex++;    // Continue to next frame
        }
    }

    // Return to idle animation (frame 0) if not running
    if (!game.IsRunning()) {
        zombieIndex = 0;
    }

    game.UpdateTimer(GetFrameTime());

    if (game.IsTimeUp()) {
        RequestScreenChange<GameOverScreen>();
    }
}

// Draw is acting as the View for the GameManager.
void GameplayScreen::Draw() const {
    const auto& game = GetContext().game;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGREEN);

    DrawText("GAMEPLAY SCREEN", 50, 50, 40, YELLOW);

    if (!game.HasStarted()) {
        DrawText("Press SPACE to start!", 50, 100, 30, WHITE);
        DrawText("Press ESC to return to menu", 50, 150, 20, WHITE);
    }
    else {
        DrawText("Press SPACE to gain points", 50, 100, 30, WHITE);
        DrawText("Press P to pause", 50, 150, 20, WHITE);

        DrawText(TextFormat("Score: %d", game.GetScore()), GetScreenWidth() / 2 - 200, 350, 100, WHITE);
        DrawText(TextFormat("Time: %.1f", game.GetTimeRemaining()),
            GetScreenWidth() / 2 - 200, 450, 100,
            game.GetTimeRemaining() < 3.0f ? RED : WHITE);

        // The zombie moves right as the score goes up, and stands on the
        // bottom edge of the window whatever the height of its texture.
        const auto& zombie = GetContext().assets.zombieFrames[zombieIndex];
        DrawTexture(zombie,
            static_cast<int>(game.GetScore() * ZOMBIE_STEP_X),
            GetScreenHeight() - zombie.Height(), WHITE);
    }
}

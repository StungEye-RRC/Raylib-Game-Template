#include "GameOverScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../GameContext.hpp"
#include "../GameManager.hpp"
#include <raylib.h>

GameOverScreen::GameOverScreen(GameContext& context)
    : FullScreen{ context } {
	zombieTexture = GetContext().textures.GetOrLoad("zombie_0.png");
}

void GameOverScreen::Update() {
    if (IsKeyPressed(KEY_ENTER)) {
        RequestScreenChange<MainMenuScreen>();
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        RequestExit();
    }
}

void GameOverScreen::Draw() const {
    const auto& game = GetContext().game;
    
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), MAROON);

    int gameOverWidth{ MeasureText("GAME OVER", 80) };
    DrawText("GAME OVER",
             GetScreenWidth() / 2 - gameOverWidth / 2,
             100,
             80,
             YELLOW);

    if (zombieTexture && zombieTexture->isValid()) {
        DrawTexture(zombieTexture->raw(),
            GetScreenWidth() / 2 - gameOverWidth / 2 - zombieTexture->width() - 20,
            40, WHITE);
    }

    const char* scoreText{ TextFormat("Final Score: %d", game.GetScore()) };
    int scoreWidth{ MeasureText(scoreText, 40) };
    DrawText(scoreText,
             GetScreenWidth() / 2 - scoreWidth / 2,
             250,
             40,
             WHITE);

    DrawText("Press ENTER to return to menu", 50, 400, 20, WHITE);
    DrawText("Press ESC to exit", 50, 430, 20, WHITE);
}

#include "MainMenuScreen.hpp"
#include "OptionsOverlay.hpp"
#include "GameplayScreen.hpp"
#include <raylib.h>

MainMenuScreen::MainMenuScreen(GameContext& context)
    : FullScreen{ context }, selectedOption{ 0 } {
}

void MainMenuScreen::Update() {
    // Navigate menu
    if (IsKeyPressed(KEY_UP)) {
        // Adding options.size() before subtracting 1 wraps from the first
        // option to the last without ever going below zero.
        selectedOption = (selectedOption + options.size() - 1) % options.size();
    }
    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption = (selectedOption + 1) % options.size();
    }

    // Select option
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        switch (selectedOption) {
            case 0:  // Play
                RequestScreenChange<GameplayScreen>();
                break;
            case 1:  // Options
                RequestScreenChange<OptionsOverlay>();
                break;
            case 2:  // Exit
                RequestExit();
                break;
        }
    }
}

void MainMenuScreen::Draw() const {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGRAY);

    int titleWidth{ MeasureText("GLUTTONOUS KEY SMASHER", 60) };
    DrawText("GLUTTONOUS KEY SMASHER",
             GetScreenWidth() / 2 - titleWidth / 2,
             50,
             60,
             YELLOW);

    // Draw menu options
    int startY{ 200 };
    int spacing{ 80 };

    for (std::size_t i{ 0 }; i < options.size(); i++) {
        int y{ startY + static_cast<int>(i) * spacing };
        Color textColor{ (i == selectedOption) ? YELLOW : WHITE };
        const char* prefix{ (i == selectedOption) ? "> " : "  " };
        DrawText(prefix, 300, y, 40, textColor);
        DrawText(options[i].c_str(), 350, y, 40, textColor);
    }
}

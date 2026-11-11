#include "ScreenManager.hpp"
#include "GameContext.hpp"
#include "Screens/LogoScreen.hpp"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <raylib.h>

int main()
{
    // Initialize window
    const int SCREEN_WIDTH{ 1280 };
    const int SCREEN_HEIGHT{ 720 };
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gluttonous Key Smasher");
    SetTargetFPS(60);
	SetExitKey(KEY_NULL);  // Disable default ESC key exit

	// Find the resources folder and make it the working directory,
	// so assets can be loaded with bare file names like "zombie_0.png".
	SearchAndSetResourceDir("resources");

	{ // Extra block scope so that the context is destroyed before CloseWindow()
		// Create the context that owns all game-wide systems.
		// This must happen after InitWindow() so that textures can be loaded.
		GameContext context;

		// Create screen manager with context
		ScreenManager screenManager{ context };
		screenManager.SetInitialScreen<LogoScreen>("Gluttonous Key Smasher", 3.0f);

		// Main loop
		while (!WindowShouldClose() && !screenManager.ShouldExit()) {
			// Update
			screenManager.Update();

			// Draw
			BeginDrawing();
			ClearBackground(BLACK);
			screenManager.Draw();
			EndDrawing();
		}
	} // The screen manager and then the context are destroyed here.

    // Cleanup
    CloseWindow();

    return 0;
}

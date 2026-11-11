#include "ScreenManager.hpp"
#include "GameContext.hpp"
#include "Screens/LogoScreen.hpp"
#include "resource_dir.h"   // utility header for SearchAndSetResourceDir
#include <raylib.h>

int main()
{
    const int SCREEN_WIDTH{ 1280 };
    const int SCREEN_HEIGHT{ 720 };
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gluttonous Key Smasher");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);  // Disable default ESC key exit
    InitAudioDevice();     // Needed before any sound can be loaded or played

    // Find the resources folder and make it the working directory,
    // so assets can be loaded with bare file names like "zombie_0.png".
    SearchAndSetResourceDir("resources");

    // Extra scope so that the context is destroyed before CloseAudioDevice()
    // and CloseWindow().
    {
        // The context owns the game-wide systems and assets. It must be
        // created after InitWindow() and InitAudioDevice() so that textures
        // and sounds can be loaded.
        GameContext context;

        ScreenManager screenManager{ context };
        screenManager.SetInitialScreen<LogoScreen>("Gluttonous Key Smasher", 3.0f);

        while (!WindowShouldClose() && !screenManager.ShouldExit()) {
            screenManager.Update();

            BeginDrawing();
            ClearBackground(BLACK);
            screenManager.Draw();
            EndDrawing();
        }
    } // The screen manager and then the context are destroyed here,
      // which unloads every texture and sound in Assets.

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

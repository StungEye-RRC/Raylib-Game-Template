# Gluttonous Key Smasher - Raylib Demo Game

Gluttonous Key Smasher is a silly little game I made using C++20 with [raylib](https://www.raylib.com/) to demonstrate a few game dev architectural patterns. Smash dat space bar as many times as you can in ten seconds sending a zombie stumbling across the screen.

## Build the Example Game

1. Run `build-VisualStudio2026.bat`.
2. Open the newly generated `.slnx` file.

The generated solution and the built exe will be named after the project folder.

## The Codez

```
src/
  main.cpp                The main game loop.
  GameContext.hpp         The state and assets shared between screens.
  GameManager.hpp/.cpp    The game rules and state.
  Assets.hpp              Textures and sounds shared between screens.
  TextureAsset.hpp/.cpp   An RAII wrapper for a Raylib 2D Texture.
  SoundAsset.hpp/.cpp     An RAII wrapper for a Raylib Sound.
  Screen.hpp              Base classes for Screen, FullScreen and Overlay
  ScreenManager.hpp/.cpp  The screen stack manager.
  Screens/                One class per screen.
  resource_dir.h          Finds the resources/ folder (from raylib-quickstart)
resources/                Images and sounds the game loads.
```

### Screens and Overlays

The example game is built out of screens. Each screen class has an `Update()` that can mutate screen state and a `Draw()` that can't mutate the screen state.

The `ScreenManager` keeps the screens in a stack:

- Only the top screen updates or reads input.
- Every screen draws, from the bottom of the stack to the top. (That's how the pause overlay is displayed over top of the paused game play screen.)
- The screen manager automatically calls a screen's `Update()` and `Draw()`.

There are two kinds of screens:

- When loaded, a `FullScreen` replaces everything on the stack. The logo, main menu, gameplay and game-over screens are full screens.
- When loaded, an `Overlay` is pushed on top of the current screen, which stays underneath and carries on once the overlay closes. The options and pause screens are overlays.

Screens can load other screens through requests:

```cpp
RequestScreenChange<GameOverScreen>();  // Go to another screen
RequestClose();                         // Overlays only. Pops screen stack. Return to previous screen.
RequestExit();                          // Quit the game
```

Requests should be made from a screen's `Update()` method.

Here's an example minimal screen. It's a `FullScreen` that requests a transition to the `MainMenuScreen` when the user presses enter.

```cpp
class TitleScreen : public FullScreen {
public:
    TitleScreen(GameContext& context) : FullScreen{ context } {}

    void Update() override {
        if (IsKeyPressed(KEY_ENTER)) {
            RequestScreenChange<MainMenuScreen>();
        }
    }

    void Draw() const override {
        DrawText("My Game", 100, 100, 40, WHITE);
    }
};
```

If we wanted this to be the app's initial screen we could load it like this in `main.cpp`:

```cpp
 GameContext context;
 ScreenManager screenManager{ context };
 screenManager.SetInitialScreen<TitleScreen>();
```

### Screens With Constructor Parameters

A screen's constructor can take extra parameters after the `GameContext`:

```cpp
class LogoScreen : public FullScreen {
public:
    LogoScreen(GameContext& context, const std::string& displayMsg, float duration);

    // Rest of class goes here.
};
```

When you load the screen you only pass the extra arguments. The context is passed in for you:

```cpp
RequestScreenChange<LogoScreen>("Hello World", 3.0f);
// or
SetInitialScreen<LogoScreen>("Greetings Earthlings", 4.2f);
```

### The GameContext

Screens share state with other screens with a `GameContext`, which they access through their `GetContext()` method. In our example game, the `GameContext` holds a `GameManger` (which holds the game logic and game state) and `Assets` (which are preloaded image and sound files):

```cpp
struct GameContext {
    GameManager game;
    Assets assets;
};
```

#### What Goes in the GameContext

The rule for what goes in the `GameContext` is things that outlive a single screen, or that more than one screen needs access to. Anything that only one screen uses should be a member of that screen instead.

In our example game, `GameManager` is in the context because the game-over screen reads its final score after the gameplay screen has been destroyed. `GameManager` is the "model" of the example game. It holds the score, the countdown timer, and whether the zombie is running. `GameplayScreen::Update()` changes it and `GameplayScreen::Draw()` shows it. A new `GameplayScreen` calls `game.Reset()` in its constructor, so every play starts a fresh game (score = 0, time remaining = full, etc).

### Texture and sound Assets

Raylib's `LoadTexture()` must be matched (later in the program) by an `UnloadTexture()`, and `LoadSound()` by an `UnloadSound()`. If you forget to unload, or do so at the wrong time, you can cause memory leaks and crashes. Our custom `TextureAsset` and `SoundAsset` take care of this. They load the asset in their constructor and unload in their destructor, so you never call the load and unload functions yourself. This is that [RAII](https://en.cppreference.com/cpp/language/raii) thing I've been telling you about! A better name for it might be [Scope-Bound Resource Management or SBRM](https://stackoverflow.com/questions/2321511/what-is-meant-by-resource-acquisition-is-initialization-raii). We can pronounce it ess-broom! ;)

```cpp
TextureAsset logo{ "logo.png" };
// Notice how we can just use it in places where raylib expects a Texture2D!!!
DrawTexture(logo, 100, 100, WHITE);

SoundAsset jump{ "jump.wav" };
PlaySound(jump);
```

In the example game the zombie textures live in an `Assets` object in the `GameContext`, but the sounds live in the `GameplayScreen`. This is because the images are needed by multiple screens and are fetched using `GetContext().assets` and the sounds are only used during game play.

Textures and sounds can only be loaded after the raylib window and audio device have been initialized, and they must be unloaded before we close the window and the audio device.

This is why `main()` runs in this order:

```cpp
InitWindow(...);       // 1. Texture2Ds need the window to be initialized.
InitAudioDevice();     // 2. Sounds need the audio to be initialized.
SearchAndSetResourceDir("resources"); // Set the asset directory.
{
    GameContext context;   // 3. Loads every asset in Assets
    // da main loop        // 4. Loaded assets can be used in the main loop. Other assets can be loaded here too.
}                          // 5. The context is destroyed here, unloading the assets (ess-broom!)
CloseAudioDevice();    // 6. Only now is it safe to close audio
CloseWindow();         // 7. And now it's safe to close the window
```

The extra `{ }` block makes sure the context and its assets are destroyed before the audio device and the window are closed. This works because C++ has block-level scoping.

#### Adding Assets Used By Multiple Screens

To add a sound that several screens use, put the file in `resources/` and add a member to `Assets`:

```cpp
struct Assets {
    // Other assets go here.

    SoundAsset scoreSound{ "score.wav" };
};
```

This sound can now be played by any `Screen` using `PlaySound(GetContext().assets.scoreSound)`.

#### Missing Asset files

If a texture can't be loaded, you get a 32×32 magenta square loaded in its place.

If a sound can't be loaded, playing it does nothing.

In both cases a warning message is printed in the console window of a Debug build with the name of the file that failed to load.

### The Resources Folder

Put your images and sounds in `resources/`. At startup, `SearchAndSetResourceDir("resources")` finds that folder and makes it the working directory. This means your program will work whether you run from Visual Studio or double-click the exe in `bin/`. It also means that you load files by name alone: `"zombie_0.png"`, not `"resources/zombie_0.png"`.

## Making a Game From This Template

1. Rename the folder to your game's name before you first run the `build-VisualStudio2026.bat`. The solution and the exe are named after the folder. Don't call it `raylib` 'cause that'll clash with the raylib library itself. If you rename it later, the `.bat` makes a new `.slnx` and `.vcxproj` with the new name, and you can delete the ones with the old name.
2. Open the project using the `.slnx` that was generated by`build-VisualStudio2026.bat`.
3. Change the window title and size in `main.cpp`, the logo text passed to `LogoScreen`, and the title drawn in `MainMenuScreen.cpp`. You can also remove the `LogoScreen` and use the `MainMenuScreen` or a custom one as your initial screen.
4. Replace `GameManager` with your game's rules and state. For example the board logic and state from Frogs and Toads. Rename it if you want to and update `GameContext` to match the new name.
5. Replace the art by putting your asset files in `resources/` and then change the members of `Assets`.
6. Rewrite `GameplayScreen` and `GameOverScreen` too. The other screens (logo, main menu, pause) are ready for you to use or change. The options screen is fake right now.

### Adding New Source Code

Create new files by hitting Ctrl-n, picking "Visual C++" on the left and then the type you want. Then save the file from "File" => "Save As...".

Save the new files to the `src/` folder (example: `src/Screens/GremlinScreen.cpp`) and then run `build-VisualStudio2026.bat`.

Be careful with Visual Studio's "Add" => "New Item". By default it saves the file in the Solution root and the next time the project is regenerated that file will get removed from the build.

Run the `.bat` file after renaming or deleting files too.

## Credits and Licences

- The zombie comes from [Platformer Characters](https://kenney.nl/assets/platformer-characters) and the footsteps from [RPG Audio](https://kenney.nl/assets/rpg-audio) and are [CC0 licenced](https://creativecommons.org/publicdomain/zero/1.0/). Thanks Kenney!
- This project started out from the [raylib-quickstart](https://github.com/raylib-extras/raylib-quickstart) by Jeffery Myers, which is also [CC0 1.0](https://creativecommons.org/publicdomain/zero/1.0/).
- `src/resource_dir.h` is from raylib-extras by Jeffery Myers, under the [MIT licence](https://opensource.org/license/mit).
- [raylib](https://github.com/raysan5/raylib) by Ramon Santamaria and contributors, under the [zlib licence](https://www.zlib.net/zlib_license.html).
- And as always, my code is [released free and unencumbered into the public domain](https://unlicense.org/). Information wants to be freeeeeeeeee.

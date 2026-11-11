#pragma once

#include <raylib.h>

// RAII wrapper for a raylib Texture2D.
//
// A TextureAsset loads its texture when it is created and unloads it when it
// is destroyed, so you never call LoadTexture() or UnloadTexture() yourself.
//
// If the file can't be loaded, a warning is written to the log and a small
// magenta square with a black border is used instead. A missing texture is
// then easy to spot on screen, and the game keeps running.
//
// Only create a TextureAsset after InitWindow(), and make sure it is destroyed
// before CloseWindow(). Textures live on the graphics card, which raylib only
// sets up once the window exists. See Assets.hpp for where to put them.
//
// Example as a member of a screen:
//   class TitleScreen : public FullScreen {
//       TextureAsset logo{ "logo.png" };
//   public:
//       TitleScreen(GameContext& context) : FullScreen{ context } {}
//       void Update() override {}
//       void Draw() const override {
//           DrawTexture(logo, 100, 100, WHITE);
//       }
//   };
//
// A TextureAsset can't be copied. Each one owns its texture, and two copies
// would both try to unload the same texture.
class TextureAsset {
public:
    explicit TextureAsset(const char* path);
    ~TextureAsset();

    TextureAsset(const TextureAsset&) = delete;
    TextureAsset& operator=(const TextureAsset&) = delete;

    // Lets a TextureAsset be passed straight to raylib functions that expect a
    // Texture2D, like DrawTexture(myAsset, x, y, WHITE). The trade-off is that
    // the conversion happens quietly, so it's less obvious what is being passed.
    operator const Texture2D&() const { return texture; }

    int Width() const { return texture.width; }
    int Height() const { return texture.height; }

private:
    Texture2D texture{};
};

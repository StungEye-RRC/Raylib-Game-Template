#include "TextureAsset.hpp"

TextureAsset::TextureAsset(const char* path)
    : texture{ LoadTexture(path) } {
    if (!IsTextureValid(texture)) {
        TraceLog(LOG_WARNING, "TextureAsset: Could not load \"%s\", using a magenta placeholder", path);

        // Build a 32x32 magenta square with a black border.
        Image placeholder{ GenImageColor(32, 32, MAGENTA) };
        ImageDrawRectangleLines(&placeholder, 0, 0, 32, 32, BLACK);
        texture = LoadTextureFromImage(placeholder);
        UnloadImage(placeholder);  // The texture has its own copy on the graphics card.
    }
}

TextureAsset::~TextureAsset() {
    UnloadTexture(texture);
}

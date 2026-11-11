#pragma once

#include "GameManager.hpp"
#include "Assets.hpp"

// GameContext holds the game-wide systems that screens share.
//
// What belongs in the context? Things that outlive a single screen or are
// shared between screens. GameManager belongs here because GameOverScreen
// reads the final score after GameplayScreen has been destroyed. Assets
// belongs here because the zombie textures are drawn by more than one screen.
// Anything that only one screen uses should be a member of that screen instead.
//
// The context owns its members by value. main() creates it after
// InitWindow() and InitAudioDevice(), and its members are destroyed along
// with it, before CloseAudioDevice() and CloseWindow().
//
// Every screen is given the context when it is created, and can reach it
// with GetContext():
//   auto& context = GetContext();
//   context.game.AddScore(10);
//
// Or more concisely:
//   GetContext().game.AddScore(10);
//   DrawTexture(GetContext().assets.zombieFrames[0], x, y, WHITE);
struct GameContext {
    GameManager game;
    Assets assets;
};

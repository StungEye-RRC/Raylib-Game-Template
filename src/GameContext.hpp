#pragma once

#include "GameManager.hpp"
#include "Assets.hpp"

// GameContext holds the game-wide systems that screens share.
//
// This context is for things that outlive a single screen or are shared
// between multiple screens. GameManager is here because the GameOverScreen
// reads its final score after GameplayScreen has been destroyed. Assets
// belongs here because the zombie textures are drawn by more than one screen.
// Anything that only one screen uses should be a member of that screen instead.

struct GameContext {
    GameManager game;
    Assets assets;
};

// Every screen can reach the GameContext with with GetContext():
//   auto& context = GetContext(); // Returns a reference
//   context.game.AddScore(10);
//
// Or without a separate variable:
//   GetContext().game.AddScore(10);
//   DrawTexture(GetContext().assets.zombieFrames[0], x, y, WHITE);
//
// Or as a const reference:
// const auto& context = GetContext(); 

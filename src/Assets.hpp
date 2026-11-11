#pragma once

#include "TextureAsset.hpp"
#include "SoundAsset.hpp"
#include <array>

// Assets holds the textures and sounds that are used across the game.
//
// Where should an asset live?
//   - Used by more than one screen: add it here. Assets is part of the
//     GameContext, so every screen can reach it with GetContext().assets.
//   - Used by only one screen: make it a TextureAsset or SoundAsset member of
//     that screen. It loads when the screen is created and unloads when the
//     screen is destroyed.
//   - Never make an asset a global or static variable. It would be created
//     before InitWindow()/InitAudioDevice() and would fail to load.
//
// Every member loads itself when the GameContext is created in main(), and
// unloads itself when the GameContext is destroyed. That's why Assets doesn't
// need a constructor or destructor of its own.
//
// To add an asset, put its file in the resources/ folder and add a member:
//   TextureAsset background{ "background.png" };
//
// Then use it from any screen:
//   DrawTexture(GetContext().assets.background, 0, 0, WHITE);
struct Assets {
    // Zombie animation frames: 0 is idle, 1 to 3 are the running animation.
    std::array<TextureAsset, 4> zombieFrames{
        TextureAsset{ "zombie_0.png" },
        TextureAsset{ "zombie_1.png" },
        TextureAsset{ "zombie_2.png" },
        TextureAsset{ "zombie_3.png" },
    };

    // Sounds work the same way. For example, once there is a score.wav in
    // resources/, uncomment this line and call
    // PlaySound(GetContext().assets.scoreSound) when the player scores:
    // SoundAsset scoreSound{ "score.wav" };
    //
    // The footstep sounds aren't here because only GameplayScreen plays them.
    // They are members of that screen instead.
};

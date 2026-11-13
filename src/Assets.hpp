#pragma once

#include "TextureAsset.hpp"
#include "SoundAsset.hpp" // Currently unused but here so you can load Sounds.
#include <array>

// Assets holds the textures and sounds that are used across the game.
//
// Where should you load and store your assets?
//   - Add here if the asset is used by more than one screen. An instance of 
//     this Assets class is part of the GameContext, so every screen can reach 
//     it by calling: GetContext().assets
//   - Assets that are only used in a single screen should be TextureAssets 
//     or SoundAssets that are stored as members of that screen. 
//   - Never make an asset a global or static variable. It would be created
//     before InitWindow() / InitAudioDevice() and would fail to load.
//
// Every member of Assets is loaded when theGameContext is created in main(),
// and is unloaded when the GameContext is destroyed. That's why Assets doesn't
// need a constructor or destructor.
//
// To add an asset, put the file in the resources/ folder and add a member:
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

    // Sounds work like this too. For example, if there were a score.wav in
    // resources/, you could uncomment this line:
    // SoundAsset scoreSound{ "score.wav" };
    //
    // And then you could call PlaySound(GetContext().assets.scoreSound) from
    // any screen.
    //
    // The footstep sounds aren't here because only GameplayScreen plays them.
    // They are members of that screen instead.
};

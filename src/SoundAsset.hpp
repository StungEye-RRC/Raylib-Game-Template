#pragma once

#include <raylib.h>

// RAII wrapper for a raylib Sound.
// 
// A SoundAsset loads its sound when it is created and unloads it when it is
// destroyed, so you never call LoadSound() or UnloadSound() yourself.
//
// If the file can't be loaded. a warning is written to the log.
//
// Only create a SoundAsset after InitAudioDevice(), and make sure it is
// destroyed before CloseAudioDevice(). See Assets.hpp for where to put them.
//
// Example:
//   SoundAsset jump{ "jump.wav" };
//   PlaySound(jump);
//
// A SoundAsset can't be copied. Each one owns its sound, and two copies would
// both try to unload the same sound.
class SoundAsset {
public:
    explicit SoundAsset(const char* path);
    ~SoundAsset();

    SoundAsset(const SoundAsset&) = delete;
    SoundAsset& operator=(const SoundAsset&) = delete;

    // Lets a SoundAsset be passed straight to raylib functions that expect a
    // Sound, like PlaySound(myAsset). 
    operator const Sound&() const { return sound; }

private:
    Sound sound{};
};

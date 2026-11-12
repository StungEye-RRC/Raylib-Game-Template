#include "SoundAsset.hpp"

SoundAsset::SoundAsset(const char* path)
    : sound{ LoadSound(path) } {
    if (!IsSoundValid(sound)) {
        // PlaySound() and UnloadSound() do nothing with a sound that failed
        // to load, so a warning is all that's needed.
        TraceLog(LOG_WARNING, "SoundAsset: Could not load \"%s\"", path);
    }
}

SoundAsset::~SoundAsset() {
    UnloadSound(sound);
}

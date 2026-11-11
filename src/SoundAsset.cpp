#include "SoundAsset.hpp"

SoundAsset::SoundAsset(const char* path)
    : sound{ LoadSound(path) } {
    if (!IsSoundValid(sound)) {
        TraceLog(LOG_WARNING, "SoundAsset: Could not load \"%s\", using a silent placeholder", path);

        // Build 0.1 seconds of silence: 4410 frames at 44100 Hz, 16-bit, mono.
        // Each 16-bit sample takes 2 bytes. MemAlloc() fills the samples with
        // zeros, and zero is silence.
        Wave silence{};
        silence.frameCount = 4410;
        silence.sampleRate = 44100;
        silence.sampleSize = 16;
        silence.channels = 1;
        silence.data = MemAlloc(silence.frameCount * silence.channels * 2);

        sound = LoadSoundFromWave(silence);
        UnloadWave(silence);  // The sound has its own copy of the samples.
    }
}

SoundAsset::~SoundAsset() {
    UnloadSound(sound);
}

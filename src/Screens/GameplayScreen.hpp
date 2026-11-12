#pragma once

#include "../Screen.hpp"
#include "../SoundAsset.hpp"
#include <array>

class GameplayScreen : public FullScreen {
private:
    int zombieIndex{ 0 };  // Which of the zombie frames in Assets to draw
    static constexpr float ZOMBIE_STEP_X{ 11.0f };

    // Only this screen plays footsteps, so the sounds are members of this
    // screen instead of being in Assets. They load when a game starts and
    // unload when this screen is destroyed.
    std::array<SoundAsset, 3> footsteps{
        SoundAsset{ "footstep00.ogg" },
        SoundAsset{ "footstep01.ogg" },
        SoundAsset{ "footstep02.ogg" },
    };

    // The shortest time between two footsteps, in seconds. Each footstep is
    // about a quarter of a second long, so without this, mashing SPACE plays
    // overlapping footsteps.
    static constexpr float FOOTSTEP_COOLDOWN{ 0.50f };
    float timeSinceFootstep{ FOOTSTEP_COOLDOWN };

public:
    GameplayScreen(GameContext& context);

    void Update() override;
    void Draw() const override;
};

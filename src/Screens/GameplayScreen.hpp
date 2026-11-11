#pragma once

#include "../Screen.hpp"

class GameplayScreen : public FullScreen {
private:
	int zombieIndex{ 0 };  // Which of the zombie frames in Assets to draw
    static constexpr float ZOMBIE_STEP_X{ 11.0f };
	
public:
    GameplayScreen(GameContext& context);

    void Update() override;
    void Draw() const override;
};

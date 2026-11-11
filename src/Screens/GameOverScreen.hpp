#pragma once

#include "../Screen.hpp"

class GameOverScreen : public FullScreen {
public:
    GameOverScreen(GameContext& context);

    void Update() override;
    void Draw() const override;
};

#pragma once

#include "../Screen.hpp"

class PauseOverlay : public Overlay {
public:
    PauseOverlay(GameContext& context);

    void Update() override;
    void Draw() const override;
};

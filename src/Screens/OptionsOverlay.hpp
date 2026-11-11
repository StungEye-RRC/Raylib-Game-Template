#pragma once

#include "../Screen.hpp"

class OptionsOverlay : public Overlay {
public:
    OptionsOverlay(GameContext& context);

    void Update() override;
    void Draw() const override;
};

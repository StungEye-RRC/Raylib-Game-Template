#pragma once

#include "../Screen.hpp"
#include <string>

class LogoScreen : public FullScreen {
private:
    // Members are initialized in the order they are declared here, not in the
    // order they are listed in the constructor. Keep the two orders the same.
    std::string displayMsg;
    float displayTime;
    float elapsedTime;

public:
    LogoScreen(GameContext& context, const std::string& displayMsg, float duration = 2.0f);

    void Update() override;
    void Draw() const override;
};

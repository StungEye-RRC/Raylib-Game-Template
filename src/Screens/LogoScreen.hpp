#pragma once

#include "../Screen.hpp"
#include <string>

class LogoScreen : public FullScreen {
private:
    float displayTime;
    float elapsedTime;
	std::string displayMsg;

public:
    LogoScreen(GameContext& context, const std::string& displayMsg, float duration = 2.0f);

    void Update() override;
    void Draw() const override;
};

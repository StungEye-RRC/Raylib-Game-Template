#pragma once

#include "../Screen.hpp"
#include <array>
#include <string>

class MainMenuScreen : public FullScreen {
private:
    std::size_t selectedOption;   // 0 = Play, 1 = Options, 2 = Exit
    std::array<std::string, 3> options{ "Play", "Options", "Exit" };

public:
    MainMenuScreen(GameContext& context);

    void Update() override;
    void Draw() const override;
};

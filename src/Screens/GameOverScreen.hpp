#pragma once

#include "../Screen.hpp"
#include "../TextureManager.hpp"

class GameOverScreen : public FullScreen {
private:
    TextureManager::Handle zombieTexture;
public:
    GameOverScreen(GameContext& context);

    void Update() override;
    void Draw() const override;
};

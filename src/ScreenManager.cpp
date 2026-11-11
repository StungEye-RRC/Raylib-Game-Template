#include "ScreenManager.hpp"

ScreenManager::ScreenManager(GameContext& context) : shouldExit{ false }, context{ context } {
}

void ScreenManager::Update() {
    if (screenStack.empty()) return;

    // screenStack.back() is the top screen
    screenStack.back()->Update();

    // The top screen's Update() has returned, so it's now safe to act on its
    // request, even if that means destroying the screen.
    Screen::Request request{ screenStack.back()->TakeRequest() };

    switch (request.type) {
        case Screen::RequestType::None:
            break;

        case Screen::RequestType::Close:
            // Only overlays can ask for this, so there's a screen underneath.
            screenStack.pop_back();
            break;

        case Screen::RequestType::Exit:
            shouldExit = true;
            break;

        case Screen::RequestType::Change:
            if (!request.nextScreen->ShouldStack()) {
                // Replace all screens (full screen behavior)
                screenStack.clear();
            }
            screenStack.push_back(std::move(request.nextScreen));
            break;
    }
}

void ScreenManager::Draw() const {
    // Draw all screens bottom-to-top
    for (const auto& screen : screenStack) {
        screen->Draw();
    }
}

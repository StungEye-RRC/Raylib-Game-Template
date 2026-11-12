#pragma once

#include "Screen.hpp"
#include <memory>
#include <vector>
#include <type_traits>
#include <utility>

struct GameContext;

// Manages the stack of screens:
//   - Only the top screen Update()s
//   - Every screen draws, from the bottom of the stack to the top
//   - FullScreens replace every screen on the stack (clear, then push)
//   - Overlays stack on top of the current screen (push only)
//   - Overlays can close themselves (pop)
//
// main() creates the manager with the GameContext, sets the initial screen,
// then calls Update() and Draw() each frame until ShouldExit() is true.
class ScreenManager {
private:
    std::vector<std::unique_ptr<Screen>> screenStack;
    bool shouldExit{ false };
    GameContext& context;

public:
    ScreenManager(GameContext& context) : context{ context } {}

    // Builds the first screen as T(context, args...), just like
    // Screen::RequestScreenChange<T>(). The first screen must be a FullScreen,
    // so that there is always a screen underneath any overlay.
    template<typename T, typename... Args>
    void SetInitialScreen(Args&&... args) {
        static_assert(std::is_base_of_v<FullScreen, T>, "The initial screen must derive from FullScreen");
        screenStack.push_back(std::make_unique<T>(context, std::forward<Args>(args)...));
    }

    void Update();
    void Draw() const;

    bool ShouldExit() const { return shouldExit; }
};

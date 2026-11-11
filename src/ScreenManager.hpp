#pragma once

#include "Screen.hpp"
#include <memory>
#include <vector>
#include <type_traits>

struct GameContext;

// Manages the screen stack and application flow.
//
// The ScreenManager maintains a stack of screens and handles:
//   - Screen transitions (push/pop/replace)
//   - Update/Draw delegation to active screens
//   - Application exit requests
//   - Handing the game-wide context to the initial screen
//
// Screens are managed as a stack:
//   - Only the top screen updates
//   - Every screen draws, from the bottom of the stack to the top
//   - FullScreens replace every screen on the stack (clear, then push)
//   - Overlays stack on top of the current screen (push only)
//   - Overlays can close themselves (pop)
//
// The manager is initialized with a GameContext reference and an initial screen.
// It updates and draws screens each frame until an exit is requested.
class ScreenManager {
private:
    std::vector<std::unique_ptr<Screen>> screenStack;
    bool shouldExit;
    GameContext& context;

public:
    ScreenManager(GameContext& context);
    ~ScreenManager() = default;

    // Template method - hides unique_ptr, automatically injects the context
    template<typename T, typename... Args>
    void SetInitialScreen(Args&&... args) {
        static_assert(std::is_base_of_v<Screen, T>, "T must derive from Screen");
        screenStack.push_back(std::make_unique<T>(context, std::forward<Args>(args)...));
    }

    void Update();
    void Draw() const;

    bool ShouldExit() const { return shouldExit; }
};

#pragma once

#include <memory>
#include <type_traits>

struct GameContext;

// Base class for all screens in the game.
//
// Screens represent different states of the application (menus, gameplay, overlays).
// The ScreenManager maintains a stack of screens and delegates Update/Draw to them.
//
// Every screen is built with a reference to the GameContext, which holds the
// game-wide systems. Screens don't know about the ScreenManager at all.
//
// Derived classes must implement:
//   - Update() - Handle input and game logic for this screen
//   - Draw() const - Render this screen
//   - ShouldStack() const - Return true if this screen should stack on top of others
//
// Protected methods for screen transitions:
//   - RequestScreenChange<T>() - Request a transition to a new screen
//   - RequestExit() - Request that the application exit
//   - RequestClose() - Close this screen (overlays only)
//   - GetContext() - Access game-wide systems (game, textures)
//
// Example of a derived screen can be seen below in FullScreen and Overlay classes.
class Screen {
public:
    // What a screen has asked the ScreenManager to do once its Update() returns.
    enum class RequestType { None, Change, Close, Exit };

    struct Request {
        RequestType type{ RequestType::None };
        std::unique_ptr<Screen> nextScreen;  // Only used by RequestType::Change
    };

private:
    GameContext& context;
    Request request;  // The pending request. A new request replaces the old one.

    void SetRequest(RequestType type, std::unique_ptr<Screen> nextScreen = nullptr) {
        request.type = type;
        request.nextScreen = std::move(nextScreen);
    }

    // Overlay is a friend so that it can use SetRequest() for RequestClose().
    // No other screen can close itself, so a FullScreen can't leave the
    // screen stack empty.
    friend class Overlay;

protected:
    // Access to the game context for derived classes
    GameContext& GetContext() { return context; }
    const GameContext& GetContext() const { return context; }

    // Requests are deferred. The ScreenManager applies a request after this
    // screen's Update() returns, because a screen must never be destroyed in
    // the middle of its own Update(). If a screen makes more than one request
    // in the same Update(), the last one wins.
    //
    // Make requests from Update(), not from a constructor. A request made in a
    // constructor is only seen after that screen's first Update() returns, and
    // that Update() can replace it.

    // Template method - automatically injects the context and hides unique_ptr from derived classes
	// typename T                   = type of the screen to create
    // typename... Args             = parameter pack (accepts 0 or more types)
    // Args&&...                    = forwarding reference. See: https://jaredmil.medium.com/c-move-semantics-pt-4-forwarding-references-std-forward-e32e95c72a7e
	// std::forward<Args>(args)...  = unpack and forward each argument
    template<typename T, typename... Args>
    void RequestScreenChange(Args&&... args) {
        static_assert(std::is_base_of_v<Screen, T>, "T must derive from Screen");
        SetRequest(RequestType::Change, std::make_unique<T>(context, std::forward<Args>(args)...));
    }

    void RequestExit() {
        SetRequest(RequestType::Exit);
    }

public:
    Screen(GameContext& context) : context{ context } {}
    virtual ~Screen() = default;

    virtual void Update() = 0;
    virtual void Draw() const = 0;
    virtual bool ShouldStack() const = 0;

    // Called by the ScreenManager after Update(). Hands over the pending
    // request and resets it to None.
    Request TakeRequest() {
        Request taken{ std::move(request) };
        request = Request{};
        return taken;
    }
};

// Full-screen screens replace the entire display.
//
// Use FullScreen for major application states like:
//   - Logo/splash screens
//   - Main menu
//   - Gameplay
//   - Game over screen
//
// FullScreen automatically implements ShouldStack() to return false,
// meaning transitioning to a FullScreen will replace the current screen
// rather than stacking on top of it.
class FullScreen : public Screen {
public:
    FullScreen(GameContext& context) : Screen{ context } {}
    bool ShouldStack() const final { return false; }
};

// Overlay screens stack on top of the current screen.
//
// Use Overlay for temporary screens that don't replace the underlying screen:
//   - Pause menu
//   - Options/settings menu
//   - Dialog boxes
//   - In-game menus
//
// Overlay automatically implements ShouldStack() to return true,
// meaning the previous screen remains in the stack and can be resumed.
//
// Overlays can call RequestClose() to pop themselves off the stack
// and return to the previous screen.
class Overlay : public Screen {
protected:
    void RequestClose() {
        SetRequest(RequestType::Close);
    }

public:
    Overlay(GameContext& context) : Screen{ context } {}
    bool ShouldStack() const final { return true; }
};

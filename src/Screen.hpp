#pragma once

#include <memory>
#include <type_traits>
#include <utility>

struct GameContext;

// Base class for all screens in the game.
//
// Screens represent different states of the application (menus, gameplay, overlays).
// The ScreenManager maintains a stack of screens and delegates Update/Draw to them.
//
// Every screen is built with a reference to the GameContext, which holds the
// game-wide systems. Screens don't know about the ScreenManager at all.
//
// Don't derive from Screen directly. Derive from FullScreen or Overlay (below),
// which already implement ShouldStack(). Your screen must implement:
//   - Update() - Handle input and game logic for this screen
//   - Draw() const - Render this screen
//
// Protected methods your screen can call:
//   - RequestScreenChange<T>() - Request a transition to a new screen
//   - RequestExit() - Request that the application exit
//   - RequestClose() - Close this screen (overlays only)
//   - GetContext() - Access game-wide systems (game, assets)
//
// See the files in Screens/ for complete examples.
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

    // Builds the new screen as T(context, args...), so derived screens never
    // pass the context or deal with unique_ptr themselves. For example:
    //   RequestScreenChange<LogoScreen>("Hello", 3.0f);
    //
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

// A FullScreen replaces every screen on the stack. Use it for the major
// states of the game: logo, main menu, gameplay, game over.
class FullScreen : public Screen {
public:
    FullScreen(GameContext& context) : Screen{ context } {}
    bool ShouldStack() const final { return false; }
};

// An Overlay is pushed on top of the current screen, which stays underneath
// and carries on once the overlay closes. Use it for temporary screens such
// as a pause menu, an options menu or a dialog box.
//
// Only an overlay can call RequestClose() to pop itself off the stack.
class Overlay : public Screen {
protected:
    void RequestClose() {
        SetRequest(RequestType::Close);
    }

public:
    Overlay(GameContext& context) : Screen{ context } {}
    bool ShouldStack() const final { return true; }
};

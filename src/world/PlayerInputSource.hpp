#pragma once

#include "util/Signal.hpp"
#include "world/PlayerInput.hpp"

namespace game {

struct InputSource;

struct PlayerInputSource {
    PlayerInputSource(InputSource*);

    Signal<const PlayerInput&> onPlayerInput;

private:
    PlayerInput playerInput;

    void onKeyPressed(const KeyInput&);
    void onKeyReleased(const KeyReleased&);
    void onMouseButtonPressed(const MouseButtonInput&);
    void onMouseButtonReleased(const MouseButtonInput&);
    void onMouseMoved(const MouseMoveInput&);
};

} // namespace game

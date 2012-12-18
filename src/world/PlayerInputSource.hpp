#pragma once

#include <SFML/Window.hpp>

#include "util/Signal.hpp"
#include "world/PlayerInput.hpp"
#include "input/InputSource.hpp"

namespace game {

struct InputSource;

struct PlayerInputSource {
    PlayerInputSource(sf::Window*, InputSource*);

    Signal<const PlayerInput&> onPlayerInput;

private:
    sf::Window* window;

    PlayerInput playerInput;

    void onKeyPressed(const KeyInput&);
    void onKeyReleased(const KeyInput&);
    void onMouseButtonPressed(const MouseButtonInput&);
    void onMouseButtonReleased(const MouseButtonInput&);
    void onMouseMoved(const MouseMoveInput&);
};

} // namespace game

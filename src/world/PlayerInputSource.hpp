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

    void onKeyPressed(KeyInput const&);
    void onKeyReleased(KeyInput const&);
    void onMouseButtonPressed(MouseButtonInput const&);
    void onMouseButtonReleased(MouseButtonInput const&);
    void onMouseMoved(MouseMoveInput const&);
};

} // namespace game

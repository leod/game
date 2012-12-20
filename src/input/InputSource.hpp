#pragma once

#include <queue>

#include <SFML/Window/Event.hpp>

#include "util/Signal.hpp"

namespace game {

// For now we can just use sf's input structs,
// as they seem to be well defined
using MouseButton = sf::Mouse;
using Key = sf::Keyboard;

using KeyInput = sf::Event::KeyEvent;
using MouseButtonInput = sf::Event::MouseButtonEvent;
using MouseMoveInput = sf::Event::MouseMoveEvent;
using MouseWheelInput = sf::Event::MouseWheelEvent;

struct InputSource {
    virtual ~InputSource();

    // Dispatches signals according to newly arrived input
    virtual void dispatch() = 0;

    Signal<KeyInput const&> onKeyPressed;
    Signal<KeyInput const&> onKeyReleased;
    Signal<MouseButtonInput const&> onMouseButtonPressed;
    Signal<MouseButtonInput const&> onMouseButtonReleased;
    Signal<MouseMoveInput const&> onMouseMoved;
    Signal<MouseWheelInput const&> onMouseWheelMoved;
};

} // namespace game

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

    Signal<const KeyInput&> onKeyPressed;
    Signal<const KeyInput&> onKeyReleased;
    Signal<const MouseButtonInput&> onMouseButtonPressed;
    Signal<const MouseButtonInput&> onMouseButtonReleased;
    Signal<const MouseMoveInput&> onMouseMoved;
    Signal<const MouseWheelInput&> onMouseWheelMoved;
};

} // namespace game

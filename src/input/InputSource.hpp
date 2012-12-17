#pragma once

#include <SFML/Event.hpp>

#include "util/Signal.hpp"

namespace game {

// For now we can just use sfml's input structs,
// as they seem to be well defined
using MouseButton = sfml::Mouse;
using Key = sfml::Keyboard;

using KeyInput = sfml::KeyEvent;
using MouseButtonInput = sfml::MouseButtonEvent;
using MouseMoveInput = sfml::MouseMoveEvent;
using MouseWheelInput = sfml::MouseWheelInput;

struct InputSource {
    virtual ~InputSource();

    // Dispatches signals according to newly arrived input
    virtual void update() = 0;

    Signal<const KeyInput&> onKeyPressed;
    Signal<const KeyInput>& onKeyReleased;
    Signal<const MouseButtonInput&> onMouseButtonPressed;
    Signal<const MouseButtonInput&> onMouseButtonReleased;
    Signal<const MouseMoveInput&> onMouseMoved;
    Signal<const MouseWheelInput&> onMouseWheelMoved;
};

} // namespace game

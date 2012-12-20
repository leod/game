#include "world/PlayerInputSource.hpp"

#include <iostream>
#include <functional>

#include "math/Math.hpp"
#include "input/InputSource.hpp"

namespace game {

PlayerInputSource::PlayerInputSource(sf::Window* window,
                                     InputSource* source)
    : window(window) {
    source->onKeyPressed.connect(std::bind(
            &PlayerInputSource::onKeyPressed,
            this, std::placeholders::_1));
    source->onKeyReleased.connect(std::bind(
            &PlayerInputSource::onKeyReleased,
            this, std::placeholders::_1));
    source->onMouseButtonPressed.connect(std::bind(
            &PlayerInputSource::onMouseButtonPressed,
            this, std::placeholders::_1));
    source->onMouseButtonReleased.connect(std::bind(
            &PlayerInputSource::onMouseButtonReleased,
            this, std::placeholders::_1));
    source->onMouseMoved.connect(std::bind(
            &PlayerInputSource::onMouseMoved,
            this, std::placeholders::_1));
}

void PlayerInputSource::onKeyPressed(KeyInput const& input) {
    switch (input.code) {
    case Key::W:
        playerInput.walkBackward = false;
        playerInput.walkForward = true;
        
        onPlayerInput(playerInput);
        break;

    case Key::A:
        playerInput.strafeRight = false;
        playerInput.strafeLeft = true;

        onPlayerInput(playerInput);
        break;

    case Key::S:
        playerInput.walkForward = false;
        playerInput.walkBackward = true;

        onPlayerInput(playerInput);
        break;

    case Key::D:
        playerInput.strafeLeft = false;
        playerInput.strafeRight = true;

        onPlayerInput(playerInput);
        break;

    default:
        break;
    }
}

void PlayerInputSource::onKeyReleased(KeyInput const& input) {
    switch (input.code) {
    case Key::W:
        playerInput.walkForward = false;
        
        onPlayerInput(playerInput);
        break;

    case Key::A:
        playerInput.strafeLeft = false;

        onPlayerInput(playerInput);
        break;

    case Key::S:
        playerInput.walkBackward = false;

        onPlayerInput(playerInput);
        break;

    case Key::D:
        playerInput.strafeRight = false;

        onPlayerInput(playerInput);
        break;

    default:
        break;
    }
}

void PlayerInputSource::onMouseButtonPressed(MouseButtonInput const& input) {
}

void PlayerInputSource::onMouseButtonReleased(MouseButtonInput const& input) {
}

void PlayerInputSource::onMouseMoved(MouseMoveInput const& input) {
    vec2 mouse = vec2(input.x, input.y);
    vec2 center = vec2(window->getSize().x / 2, window->getSize().y / 2);
    playerInput.orientation = -glm::normalize(mouse - center);
    onPlayerInput(playerInput);
}

} // namespace game

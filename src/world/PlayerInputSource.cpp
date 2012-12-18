#include "world/PlayerInputSource.hpp"

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

void PlayerInputSource::onKeyPressed(const KeyInput& input) {
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

void PlayerInputSource::onKeyReleased(const KeyInput& input) {
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

void PlayerInputSource::onMouseButtonPressed(const MouseButtonInput& input) {
}

void PlayerInputSource::onMouseButtonReleased(const MouseButtonInput& input) {
}

void PlayerInputSource::onMouseMoved(const MouseMoveInput& input) {
    playerInput.orientation =
            glm::normalize(vec2((float)input.x / window->getSize().x,
                                (float)input.y / window->getSize().y));
    onPlayerInput(playerInput);
}

} // namespace game

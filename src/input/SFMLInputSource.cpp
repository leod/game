#include "input/SFMLInputSource.hpp"

namespace game {

void SFMLInputSource::dispatch() {
    while (!events.empty()) {
        dispatchOne(events.front());
        events.pop();
    }
}

void SFMLInputSource::onWindowEvent(sf::Event const& event) {
    events.push(event);
}

void SFMLInputSource::dispatchOne(sf::Event const& event) {
    switch (event.type) {
    case sf::Event::KeyPressed:
        onKeyPressed(event.key);
        break;

    case sf::Event::KeyReleased:
        onKeyReleased(event.key);
        break;

    case sf::Event::MouseButtonPressed:
        onMouseButtonPressed(event.mouseButton);
        break;

    case sf::Event::MouseButtonReleased:
        onMouseButtonReleased(event.mouseButton);
        break;

    case sf::Event::MouseMoved:
        onMouseMoved(event.mouseMove);
        break;

    case sf::Event::MouseWheelMoved:
        onMouseWheelMoved(event.mouseWheel);
        break;

    default:
        break;
    }
}

} // namespace game

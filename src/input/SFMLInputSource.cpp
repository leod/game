#include "input/SFMLInputSource.hpp"

namespace game {

void SFMLInputSource::update() {
    while (!events.empty()) {
        dispatchOne(events.front());
        events.pop();
    }
}

void SFMLInputSource::onNewEvent(const sf::Event& event) {
    events.push_back(event);
}

void SFMLInputSource::dispatchOne(const sf::Event& event) {
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

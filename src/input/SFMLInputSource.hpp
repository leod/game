#pragma once

#include <SFML/Event.hpp>

#include "input/InputSource.hpp"

namespace game {

struct SFMLInputSource : public InputSource {
    // Implements InputSource::update
    void update();

    // This function must be called whenever there is a new event on the window
    void onNewEvent(const sf::Event&);

private:
    // For now, we keep events in a queue
    // and dispatch them in the update function
    std::queue<sf::Event> events;

    void dispatchOne(const sf::Event&);
};

} // namespace game

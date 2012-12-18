#pragma once

#include <queue>

#include <SFML/Window/Event.hpp>

#include "input/InputSource.hpp"

namespace game {

struct SFMLInputSource : public InputSource {
    // Implements InputSource::dispatch
    void dispatch();

    // This function must be called whenever there is a new event on the window
    void onWindowEvent(const sf::Event&);

private:
    // For now, we keep events in a queue
    // and dispatch them in the update function
    std::queue<sf::Event> events;

    void dispatchOne(const sf::Event&);
};

} // namespace game

#include "input/ClockTimeSource.hpp"

namespace game {

Time ClockTimeSource::nextDeltaImpl() {
    Time delta = clock.getElapsedTime();
    clock.restart();

    return delta;
}

} // namespace game

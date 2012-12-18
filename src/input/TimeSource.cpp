#include "input/TimeSource.hpp"

namespace game {

TimeSource::~TimeSource() {
}

Time TimeSource::nextDelta() {
    Time delta = nextDeltaImpl();
    onNextDelta(delta);

    return delta;
}

} // namespace game

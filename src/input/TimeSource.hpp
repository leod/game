#pragma once

#include "core/Time.hpp"
#include "util/Signal.hpp"

namespace game {

struct TimeSource {
    virtual ~TimeSource();

    // Returns the delta time of the next frame
    Time nextDelta();

    Signal<Time> onNextDelta;

protected:
    virtual Time nextDeltaImpl() = 0;
};

} // namespace game

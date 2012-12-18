#pragma once

#include "input/TimeSource.hpp"

namespace game {

struct ClockTimeSource : TimeSource {
protected:
    // Implements TimeSource::nextDeltaImpl
    Time nextDeltaImpl();

private:
    Clock clock;
};

}

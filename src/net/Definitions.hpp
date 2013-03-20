#pragma once

#include <cstdint>

namespace game {

#define USE_PREDICTION
#define SIMULATE_LAG

#ifdef SIMULATE_LAG
extern int const lagSimulationMs;
#endif

// Valid ClientIds are != 0
typedef uint16_t ClientId;

enum {
    MAX_CLIENT_ID = 255
};

// Valid Ticks are != 0
typedef uint32_t Tick;

enum NetChannel {
    CHANNEL_MESSAGES = 0,
    CHANNEL_STATES = 1,
    CHANNEL_TIME = 2
};

enum {
    TICK_FREQUENCY = 20
};

typedef uint16_t NetEntityId;
typedef uint16_t NetEntityTypeId;

} // namespace game

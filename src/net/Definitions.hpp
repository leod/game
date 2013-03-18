#pragma once

#include <cstdint>

namespace game {

#define USE_PREDICTION
#define SIMULATE_LAG 200

// Valid ClientIds are != 0
typedef uint16_t ClientId;

enum {
    MAX_CLIENT_ID = 255
};


// Valid Ticks are != 0
typedef uint32_t Tick;

enum {
    CHANNEL_MESSAGES = 0,
    CHANNEL_STATES = 1
};

enum {
    TICK_FREQUENCY = 20
};

typedef uint16_t NetEntityId;
typedef uint16_t NetEntityTypeId;

} // namespace game

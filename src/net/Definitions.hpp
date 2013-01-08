#pragma once

#include <cstdint>

namespace game {

// Valid ClientIds are 1...255
typedef uint8_t ClientId;

enum {
    MAX_CLIENT_ID = 255
};


// Valid Ticks are 1...2^32-1
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

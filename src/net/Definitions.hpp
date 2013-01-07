#pragma once

#include <cstdint>

namespace game {

typedef uint8_t ClientId;
typedef uint32_t Tick;

enum {
    MAX_CLIENT_ID = 255
};

enum {
    CHANNEL_MESSAGES = 0,
    CHANNEL_STATES = 1
};

enum {
    TICK_FREQUENCY = 5
};

} // namespace game

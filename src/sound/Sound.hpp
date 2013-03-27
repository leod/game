#pragma once

namespace game {

enum class Sound {
    FOOTSTEP = 0,
    SHOOT,
    HIT_PLAYER,

    MAX
};

extern char const* soundFilenames[static_cast<int>(Sound::MAX)];

} // namespace game

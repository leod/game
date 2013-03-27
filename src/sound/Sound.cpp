#include "sound/Sound.hpp"

namespace game {

char const* soundFilenames[static_cast<int>(Sound::MAX)] =
{
    "footstep.wav",
    "shoot.wav",
    "hit_player.wav"
};

} // namespace game

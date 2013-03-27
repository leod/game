#pragma once

#include <list>
#include <SFML/Audio.hpp>

#include "math/Math.hpp"
#include "sound/Sound.hpp"

namespace game {

struct SoundPlayer {
    SoundPlayer();

    void play(Sound, vec3 const&);
    void prune();

    static void setListenerPosition(vec3 const&);
    static void setGlobalVolume(float volume); // volume is in [0, 100]

private:
    void load();

    sf::SoundBuffer buffers[static_cast<int>(Sound::MAX)];

    std::list<sf::Sound> soundsPlaying;
}; 

} // namespace game

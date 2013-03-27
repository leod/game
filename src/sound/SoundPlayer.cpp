#include "sound/SoundPlayer.hpp"

#include "core/Error.hpp"
#include "core/Log.hpp"

namespace game {

SoundPlayer::SoundPlayer() {
    load();
}

void SoundPlayer::play(Sound sound, vec3 const& position) {
    int index = static_cast<int>(sound);
    ASSERT(index >= 0 && index < static_cast<int>(Sound::MAX));

    TRACE(sound) << "Playing sound \"" << soundFilenames[index] << "\"";

    sf::SoundBuffer& buffer = buffers[index];
    soundsPlaying.emplace_front(buffer);

    auto soundSource = soundsPlaying.front();
    soundSource.play();
    //soundSource.setPosition(position.x, position.y, position.z);
}

void SoundPlayer::prune() {
    auto it = soundsPlaying.begin();
    while (it != soundsPlaying.end()) {
        sf::Sound const& soundSource = *it;

        if (soundSource.getStatus() == sf::Sound::Stopped)
            it = soundsPlaying.erase(it); 
        else
            ++it;
    }
}

void SoundPlayer::setListenerPosition(vec3 const& position) {
    sf::Listener::setPosition(position.x, position.y, position.z);
}

void SoundPlayer::setGlobalVolume(float volume) {
    sf::Listener::setGlobalVolume(volume);
}

void SoundPlayer::load() {
    std::string soundDir = "sounds/";

    for (size_t i = 0; i < static_cast<int>(Sound::MAX); ++i) {
        if (buffers[i].loadFromFile(soundDir + soundFilenames[i]))
            INFO(sound) << "Loading sound \"" << soundDir + soundFilenames[i]
                        << "\"";
        else
            WARN(sound) << "Failed to load sound \""
                        << soundDir + soundFilenames[i] << "\"";
    }
}

} // namespace game

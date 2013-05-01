#pragma once

#include <deque>

#include "core/Component.hpp"
#include "world/PlayerInput.hpp"

namespace game {

struct PhysicsComponent;

// When did my struct names start to be so long?
// I feel dirty :(
struct PlayerInputComponent : public Component<PlayerInputComponent> {
    PlayerInputComponent(PhysicsComponent*);

    void onPlayerInput(PlayerInput const&);

    void onCorrection(vec3 const& position);

private:
    PhysicsComponent* physics;

    struct LogEntry {
        PlayerInput input;
        vec3 position; 
    };

    std::deque<LogEntry> log;

    void replay(vec3 const& start);
};

} // namespace game

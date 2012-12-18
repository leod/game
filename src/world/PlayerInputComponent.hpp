#pragma once

#include "core/Component.hpp"

namespace game {

struct PlayerInputSource;
struct PhysicsComponent;
struct PlayerInput;

struct PlayerInputComponent : public ComponentBase<PlayerInputComponent> {
    PlayerInputComponent(PlayerInputSource*, PhysicsComponent*);

private:
    PhysicsComponent* physics;

    void onPlayerInput(const PlayerInput&);
};

}

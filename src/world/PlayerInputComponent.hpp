#pragma once

#include "core/Component.hpp"
#include "world/PlayerInput.hpp"
#include "world/TickComponent.hpp"

namespace game {

struct PhysicsComponent;

struct PlayerInputComponent : public Component<PlayerInputComponent> {
    PlayerInputComponent(PhysicsComponent*);

    void onPlayerInput(PlayerInput const&);

private:
    PhysicsComponent* physics;
};

}


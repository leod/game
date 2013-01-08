#pragma once

#include "core/Component.hpp"
#include "world/PlayerInput.hpp"
#include "world/TickComponent.hpp"

namespace game {

struct PhysicsComponent;

struct PlayerInputComponent : public TickComponent {
    PlayerInputComponent(PhysicsComponent*);

    void tick();
    void onPlayerInput(PlayerInput const&);

private:
    PhysicsComponent* physics;

    PlayerInput playerInput;
};

}

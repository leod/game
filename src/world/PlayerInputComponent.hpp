#pragma once

#include "core/Component.hpp"
#include "world/PlayerInput.hpp"
#include "world/TickComponent.hpp"

namespace game {

struct PlayerInputSource;
struct PhysicsComponent;

struct PlayerInputComponent : public TickComponent {
    PlayerInputComponent(PlayerInputSource*, PhysicsComponent*);

    void tick();

private:
    PhysicsComponent* physics;

    PlayerInput playerInput;

    void onPlayerInput(PlayerInput const&);
};

}

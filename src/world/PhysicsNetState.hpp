#pragma once

#include "physics/PhysicsState.hpp"

namespace game {

struct PhysicsComponent;
struct NetStateType;

struct PhysicsNetState : NetStateBase<PhysicsState> {
    PhysicsNetState(PhysicsComponent*);

    NetStateType const& type();
    PhysicsState load();
    void store(PhysicsState const&);

private:
    PhysicsComponent* physics;
};

} // namespace game

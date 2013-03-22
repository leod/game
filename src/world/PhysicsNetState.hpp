#pragma once

#include "physics/PhysicsState.hpp"
#include "net/NetState.hpp"

namespace game {

extern NetStateType const physicsNetStateType;

struct PhysicsComponent;

struct PhysicsNetState : public NetStateBase<PhysicsState> {
    PhysicsNetState(PhysicsComponent*);

    // Implement NetStateBase<PhysicsState>
    NetStateType const& type() const;
    PhysicsState load() const;
    void store(PhysicsState const&);

private:
    PhysicsComponent* physics;
};

} // namespace game

#include "world/PhysicsNetState.hpp"

#include "util/Print.hpp"
#include "core/Entity.hpp"

#include "core/Error.hpp"
#include "util/BitStream.hpp"
#include "physics/PhysicsComponent.hpp"

namespace game {

void writeState(BitStreamWriter& stream, PhysicsState const* state) {
    write(stream, state->position);
    write(stream, state->orientation);
}

void readState(BitStreamReader& stream, PhysicsState* state) {
    read(stream, state->position);
    read(stream, state->orientation);
}

void interpolateState(PhysicsState const* a, PhysicsState const* b, float t,
                      PhysicsState* state) {
    state->position = a->position * (1 - t) + b->position * t;
    state->orientation = a->orientation * (1 - t) + b->orientation * t;
}

static NetStateType const stateType =
        makeNetStateType(writeState, readState, interpolateState);

PhysicsNetState::PhysicsNetState(PhysicsComponent* physics)
    : physics(physics) {
}

NetStateType const& PhysicsNetState::type() const {
    return stateType;
}

PhysicsState PhysicsNetState::load() const {
    return physics->getState();
}

void PhysicsNetState::store(PhysicsState const& state) {
    physics->setState(state);
}

} // namespace game

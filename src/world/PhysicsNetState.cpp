#include "world/PhysicsNetState.hpp"

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

void interpolateState(PhysicsState const*, PhysicsState const*, float,
                      PhysicsState*) {
    ASSERT(false);
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

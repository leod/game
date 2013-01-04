#include "world/PhysicsNetState.hpp"

#include "util/BitStream.hpp"
#include "physics/PhysicsComponent.hpp"
#include "net/NetState.hpp"

namespace game {

static NetStateType stateType;

PhysicsNetState::PhysicsNetState(PhysicsComponent* physics)
    : physics(physics) {
}

NetStateType const& type() {
    return stateType;
}

PhysicsState PhysicsNetState::load() {
    return physics->getState();
}

void store(PhysicsState const& state) {
    physics->setState(state);
}

void writeState(BitStreamWriter& stream, PhysicsState const* state) {
    write(stream, state->position);
    write(stream, state->orientation);
}

void readState(BitStreamReader& stream, PhysicsState* state) {
    read(stream, state->position);
    read(stream, state->orientation);
}

NetStateType stateType = makeNetStateType(writeState, readState, nullptr);

} // namespace game

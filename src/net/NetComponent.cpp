#include "net/NetComponent.hpp"

namespace game {

NetComponent::NetComponent(NetEntityTypeId netTypeId, NetEntityId netId,
                           ClientId owner, std::vector<NetState*>&& states)
    : netTypeId(netTypeId), netId(netId), owner(owner), states(states) {
}

NetEntityTypeId NetComponent::getNetTypeId() const {
    return netTypeId;
}

NetEntityId NetComponent::getNetId() const {
    return netId;
}

ClientId NetComponent::getOwner() const {
    return owner;
}

std::vector<NetState*> const& NetComponent::getStates() const {
    return states;
}

} // namespace game

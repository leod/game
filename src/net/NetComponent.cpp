#include "net/NetComponent.hpp"

namespace game {

NetComponent::NetComponent(NetEntityTypeId netTypeId, NetEntityId netId, std::vector<NetState*>&& states)
    : netTypeId(netTypeId), netId(netId), states(states) {
}

NetEntityId NetComponent::getNetId() const {
    return netId;
}

NetEntityTypeId NetComponent::getNetTypeId() const {
    return netTypeId;
}

std::vector<NetState*> const& NetComponent::getStates() const {
    return states;
}

} // namespace game

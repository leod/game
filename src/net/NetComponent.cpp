#include "net/NetComponent.hpp"

namespace game {

NetComponent::NetComponent(NetEntityId netId, std::vector<NetState*> states)
    : netId(netId), states(states) {

}

NetEntityId NetComponent::getNetId() const {
    return netId;
}

std::vector<NetState*> const& NetComponent::getStates() const {
    return states;
}

} // namespace game

#pragma once

#include <vector>

#include "core/Component.hpp"

namespace game {

typedef std::vector<NetState> NetStateList;

struct NetComponent : public ComponentFamily<NetComponent> {
    NetComponent(NetEntityId id)
        : netStates(netStates) {
    }

    void serialize(BitStream&) const;
    void unserialize(BitStream&);

private:
    NetStateList netStateList;
};

}

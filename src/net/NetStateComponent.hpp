#pragma once

#include <vector>

#include "core/Component.hpp"

namespace game {

typedef std::vector<NetState> NetStateList;

struct NetStateComponent : public ComponentFamily<NetStateComponent> {
    NetStateComponent(NetStateList netStates)
        : netStates(netStates) {
    }

    void serialize(BitStream&);
    void unserialize(BitStream&);

private:
    NetStateList netStateList;
};

}

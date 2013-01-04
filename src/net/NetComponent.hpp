#pragma once

#include <cstdint>
#include <vector>

#include "core/Component.hpp"

namespace game {

struct NetState;

typedef uint16_t NetEntityId;

struct NetComponent : public ComponentFamily<NetComponent> {
    NetComponent(NetEntityId, std::vector<NetState*>);

    NetEntityId getNetId() const;
    std::vector<NetState*> const& getStates() const;

private:
    NetEntityId netId;
    std::vector<NetState*> states; // TODO: Mem cleanup
};

} // namespace game

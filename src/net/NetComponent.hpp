#pragma once

#include <cstdint>
#include <vector>

#include "core/Component.hpp"

namespace game {

struct NetState;

typedef uint16_t NetEntityId;
typedef uint16_t NetEntityTypeId;

struct NetComponent : public ComponentFamily<NetComponent> {
    NetComponent(NetEntityTypeId, NetEntityId, std::vector<NetState*>&&);

    NetEntityId getNetId() const;
    NetEntityTypeId getNetTypeId() const;

    std::vector<NetState*> const& getStates() const;

private:
    NetEntityTypeId netTypeId;
    NetEntityId netId;

    std::vector<NetState*> states; // TODO: Mem cleanup
};

} // namespace game

#pragma once

#include <cstdint>
#include <vector>

#include "core/Component.hpp"
#include "net/Definitions.hpp"

namespace game {

struct NetState;

struct NetComponent : public ComponentFamily<NetComponent> {
    NetComponent(NetEntityTypeId, NetEntityId, ClientId,
                 std::vector<NetState*>&&);

    NetEntityTypeId getNetTypeId() const;
    NetEntityId getNetId() const;
    ClientId getOwner() const;

    std::vector<NetState*> const& getStates() const;

private:
    NetEntityTypeId netTypeId;
    NetEntityId netId;
    ClientId owner;

    std::vector<NetState*> states; // TODO: Mem cleanup
};

} // namespace gam

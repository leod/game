#pragma once

#include <vector>
#include <functional>

#include "core/Component.hpp"
#include "net/Definitions.hpp"

namespace game {

struct NetStateType;

struct NetEntityType {
    NetEntityTypeId typeId;
    std::vector<NetStateType const*> states;
    std::function<ComponentList(NetEntityId, ClientId)> make;
};

} // namespace game

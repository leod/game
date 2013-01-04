#pragma once

#include "core/System.hpp"
#include "net/NetComponent.hpp"

namespace game {

struct BitStreamWriter;

struct NetSystem : public SystemBase<NetComponent> {
    void writeRawStates(BitStreamWriter&) const;
};

} // namespace game

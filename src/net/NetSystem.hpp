#pragma once

#include "core/System.hpp"
#include "net/NetComponent.hpp"

namespace game {

struct BitStreamWriter;

struct NetSystem : public SystemBase<NetComponent> {
    void writeRawStates(BitStreamWriter&) const;

    // Implement SystemBase<NetComponent>
    void onRegister(NetComponent*);
    void onUnregister(NetComponent*);

private:
    std::map<NetEntityId, NetComponent*> components;
};

} // namespace game

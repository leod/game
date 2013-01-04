#pragma once

#include "core/System.hpp"
#include "net/NetComponent.hpp"

namespace game {

struct BitStreamWriter;
struct BitStreamReader;
struct NetStateStore;

struct NetSystem : public SystemBase<NetComponent> {
    // Implement SystemBase<NetComponent>
    void onRegister(NetComponent*);
    void onUnregister(NetComponent*);

    NetComponent* get(NetEntityId);
    NetComponent const* get(NetEntityId) const;

    void writeRawStates(BitStreamWriter&) const;
    void readRawStates(BitStreamReader&, NetStateStore&) const;

private:
    std::map<NetEntityId, NetComponent*> components;
};

} // namespace game

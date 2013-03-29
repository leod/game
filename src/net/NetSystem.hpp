#pragma once

#include <functional>

#include "core/System.hpp"
#include "core/EntityManager.hpp"
#include "math/Math.hpp"
#include "net/NetComponent.hpp"
#include "net/EventTypes.hpp"
#include "net/NetEntityType.hpp"

namespace game {

struct BitStreamWriter;
struct BitStreamReader;
struct NetStateStore;

struct NetSystem : public System<NetComponent> {
    // Implement System<NetComponent>
    void onRegister(NetComponent*) override;
    void onUnregister(NetComponent*) override;

    NetComponent* get(NetEntityId);
    NetComponent const* get(NetEntityId) const;

    bool exists(NetEntityId) const;
    void remove(NetEntityId);

    void writeStates(BitStreamWriter&, ClientId ignore = 0) const;
    void readStates(BitStreamReader&, NetStateStore&) const;

    void applyStates(NetStateStore const&);
    void interpolateStates(NetStateStore const&, NetStateStore const&, float);

    void registerType(NetEntityType const&);
    NetEntityType const& getType(NetEntityTypeId) const;

    Entity* createEntity(NetEntityTypeId, NetEntityId, ClientId);

private:
    // We keep a map of net entities separately from EntityManager,
    // so that we can index by NetEntityId.
    std::map<NetEntityId, NetComponent*> components;

    std::map<NetEntityTypeId, NetEntityType> entityTypes;
};

} // namespace game

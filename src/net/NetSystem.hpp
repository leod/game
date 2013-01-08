#pragma once

#include <functional>

#include "core/System.hpp"
#include "core/EntityRegistry.hpp"
#include "math/Math.hpp"
#include "net/NetComponent.hpp"

namespace game {

struct BitStreamWriter;
struct BitStreamReader;
struct NetStateStore;

typedef std::function<ComponentList(NetEntityId, ClientId, vec3)>
    NetEntityMaker;

struct NetSystem : public SystemBase<NetComponent> {
    // Implement SystemBase<NetComponent>
    void onRegister(NetComponent*);
    void onUnregister(NetComponent*);

    NetComponent* get(NetEntityId);
    NetComponent const* get(NetEntityId) const;

    void writeRawStates(BitStreamWriter&, ClientId ignore = 0) const;
    void readRawStates(BitStreamReader&, NetStateStore&) const;

    void applyStates(NetStateStore const&);
    void interpolateStates(NetStateStore const&, NetStateStore const&, float);

    // TODO: Ids Will be automated.
    template<typename Maker>
    void registerType(NetEntityTypeId typeId, Maker maker) {
        entityTypes[typeId] = maker;
    }

    Entity* createEntity(NetEntityTypeId, NetEntityId, ClientId, vec3);

private:
    std::map<NetEntityId, NetComponent*> components;

    std::map<NetEntityTypeId, NetEntityMaker> entityTypes;
};

} // namespace game

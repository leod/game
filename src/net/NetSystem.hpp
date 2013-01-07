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

typedef std::function<ComponentList(NetEntityId, vec3)> NetEntityMaker;

struct NetSystem : public SystemBase<NetComponent> {
    // Implement SystemBase<NetComponent>
    void onRegister(NetComponent*);
    void onUnregister(NetComponent*);

    NetComponent* get(NetEntityId);
    NetComponent const* get(NetEntityId) const;

    void writeRawStates(BitStreamWriter&) const;
    void readRawStates(BitStreamReader&, NetStateStore&) const;

    void interpolateStates(NetStateStore const&, NetStateStore const&, float);

    // For testing
    void applyStates(NetStateStore const&);

    // TODO: Ids Will be automated.
    void registerType(NetEntityTypeId, NetEntityMaker);

    Entity* createEntity(NetEntityTypeId, NetEntityId, vec3);

private:
    std::map<NetEntityId, NetComponent*> components;

    std::map<NetEntityTypeId, NetEntityMaker> entityTypes;
};

} // namespace game

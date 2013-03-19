#pragma once

#include <functional>

#include "core/System.hpp"
#include "core/EntityManager.hpp"
#include "math/Math.hpp"
#include "net/NetComponent.hpp"
#include "net/EventTypes.hpp"

namespace game {

struct BitStreamWriter;
struct BitStreamReader;
struct NetStateStore;

typedef std::function<ComponentList(NetEntityId, ClientId)>
    NetEntityMaker;

struct NetSystem : public SystemBase<NetComponent> {
    // Implement SystemBase<NetComponent>
    void onRegister(NetComponent*);
    void onUnregister(NetComponent*);

    NetComponent* get(NetEntityId);
    NetComponent const* get(NetEntityId) const;

    bool exists(NetEntityId) const;
    void remove(NetEntityId);

    void storeStateInArray(NetComponent const*, std::vector<uint8_t>& out)
        const;
    void loadStateFromArray(NetComponent*, std::vector<uint8_t> const& in)
        const;

    void writeRawStates(BitStreamWriter&, ClientId ignore = 0) const;
    void readRawStates(BitStreamReader&, NetStateStore&) const;

    void applyStates(NetStateStore const&);
    void interpolateStates(NetStateStore const&, NetStateStore const&, float);

    // TODO: Ids will be automated.
    template<typename Maker>
    void registerType(NetEntityTypeId typeId, Maker maker) {
        entityTypes[typeId] = maker;
    }

    Entity* createEntity(NetEntityTypeId, NetEntityId, ClientId,
                         InitialState const&);

private:
    // We keep a map of net entities separately from EntityManager,
    // so that we can index by NetEntityId.
    std::map<NetEntityId, NetComponent*> components;

    std::map<NetEntityTypeId, NetEntityMaker> entityTypes;
};

} // namespace game

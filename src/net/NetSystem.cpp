#include "net/NetSystem.hpp"

#include <vector>

#include "core/Error.hpp"
#include "core/Entity.hpp"
#include "util/BitStream.hpp"
#include "net/NetState.hpp"
#include "net/NetStateStore.hpp"

namespace game {

void NetSystem::onRegister(NetComponent* component) {
    ASSERT_MSG(components.find(component->getNetId()) == components.end(),
               "NetEntityId " << component->getNetId() <<
               " is being used more than once");

    components[component->getNetId()] = component;
}

void NetSystem::onUnregister(NetComponent* component) {
    size_t numErased = components.erase(component->getNetId());
    ASSERT(numErased == 1);
}

NetComponent* NetSystem::get(NetEntityId id) {
    auto componentIt = components.find(id);

    ASSERT_MSG(componentIt != components.end(),
               "Net entity " << id << " not found");
    ASSERT(componentIt->second->getNetId() == id);

    return componentIt->second;
}

NetComponent const* NetSystem::get(NetEntityId id) const {
    auto componentIt = components.find(id);

    ASSERT_MSG(componentIt != components.end(),
               "Net entity " << id << " not found");
    ASSERT(componentIt->second->getNetId() == id);

    return componentIt->second;
}

void NetSystem::writeRawStates(BitStreamWriter& stream,
                               ClientId ignore) const {
    iterate([&] (NetComponent const* component) {
        if (ignore != 0 && ignore == component->getOwner())
            return;

        write(stream, component->getNetId());

        std::vector<uint8_t> buffer;

        for (auto state : component->getStates()) {
            if (buffer.size() < state->type().size)
                buffer.resize(state->type().size);

            state->load(&buffer[0]);
            state->type().write(stream, &buffer[0]);
        }
    });
}

void NetSystem::readRawStates(BitStreamReader& stream,
                              NetStateStore& store) const {
    while (!stream.eof()) {
        NetEntityId netId;
        read(stream, netId);

        NetComponent const* component = get(netId);

        size_t requiredSize = 0;
        for (auto state : component->getStates())
            requiredSize += state->type().size;

        uint8_t* buffer = store.allocate(netId, requiredSize);

        for (auto state : component->getStates()) {
            state->type().read(stream, buffer);
            buffer += state->type().size;
        }
    }
}

void NetSystem::interpolateStates(NetStateStore const& a,
                                  NetStateStore const& b,
                                  float t) {
    // We shall ignore this issue for now
    /*ASSERT_MSG(a.size() == b.size() && a.dataSize() == b.dataSize(),
               "Implement me: " << a.size() << ", " << b.size() << ", "
                                << a.dataSize() << ", " << b.dataSize());*/

    // Buffer for the interpolation result
    std::vector<uint8_t> buffer;

    for (size_t i = 0, j = 0; i < a.size(); ++i, ++j) {
        auto entryA = a[i],
             entryB = b[j];

        ASSERT(entryA.id == entryB.id);

        auto offsetA = entryA.offset,
             offsetB = entryB.offset;

        auto component = get(entryA.id);

        for (auto state : component->getStates()) {
            if (buffer.size() < state->type().size)
                buffer.resize(state->type().size);

            auto dataA = a.data(offsetA),
                 dataB = b.data(offsetB);

            state->type().interpolate(dataA, dataB, t, &buffer[0]);
            state->store(&buffer[0]);

            offsetA += state->type().size;
            offsetB += state->type().size;
        }
    }
}

void NetSystem::applyStates(NetStateStore const& store) {
    for (size_t i = 0; i < store.size(); ++i) {
        auto entry = store[i];
        auto offset = entry.offset;
        
        NetComponent const* component = get(entry.id);
        
        for (auto state : component->getStates()) {
            state->store(store.data(offset));
            offset += state->type().size;
        }
    }
}

Entity* NetSystem::createEntity(NetEntityTypeId typeId, NetEntityId id,
        ClientId owner, vec3 pos) {
    auto entity = getRegistry()->add(entityTypes[typeId](id, owner, pos));

#ifndef NDEBUG
    auto netComponent = entity->component<NetComponent>();
    ASSERT_MSG(netComponent, "Entity " << id << " has no NetComponent.");
    ASSERT(netComponent->getNetTypeId() == typeId);
    ASSERT(netComponent->getNetId() == id);
    ASSERT(netComponent->getOwner() == owner);
#endif

    return entity;
}

} // namespace game

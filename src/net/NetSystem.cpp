#include "net/NetSystem.hpp"

#include <vector>
#include <iostream>

#include "core/Error.hpp"
#include "core/Entity.hpp"
#include "util/BitStream.hpp"
#include "net/NetState.hpp"
#include "net/NetStateStore.hpp"

namespace game {

void NetSystem::onRegister(NetComponent* component) {
    TRACE(net) << "Creating net entity #" << component->getNetId()
               << ": " << component->getEntity();

    ASSERT_MSG(components.find(component->getNetId()) == components.end(),
               "NetEntityId " << component->getNetId() <<
               " is being used more than once");

    components[component->getNetId()] = component;
}

void NetSystem::onUnregister(NetComponent* component) {
    TRACE(net) << "Removing net entity #" << component->getNetId();

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

bool NetSystem::exists(NetEntityId id) const {
    return components.find(id) != components.end();
}

void NetSystem::remove(NetEntityId id) {
    auto componentIt = components.find(id); 

    ASSERT_MSG(componentIt != components.end(),
               "Net entity " << id << " not found");
    components.erase(componentIt);
}

void NetSystem::writeRawStates(BitStreamWriter& stream,
                               ClientId ignore) const {
    iterate([&] (NetComponent const* component) {
        if (ignore != 0 && ignore == component->getOwner() &&
            component->getNetTypeId() == 1 /* TODO: This is for debugging */)
            return;

        //INFO(net) << "Writing state for " << component->getNetId();

        ASSERT(component->getNetId() != 0);
        write(stream, component->getNetId());

        std::vector<uint8_t> buffer;

        write(stream, component->getNetTypeId());

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

        ASSERT(netId != 0);

        NetEntityTypeId typeId;
        read(stream, typeId);

        NetEntityType const& entityType = getType(typeId);

        size_t requiredSize = 0;
        for (auto state : entityType.states)
            requiredSize += state->size;

        uint8_t* buffer = store.allocate(netId, requiredSize);

        for (auto state : entityType.states) {
            state->read(stream, buffer);
            buffer += state->size;
        }
    }
}

void NetSystem::interpolateStates(NetStateStore const& a,
                                  NetStateStore const& b,
                                  float t) {
    // Buffer for the interpolation result.
    // Kept outside of the for loop so the memory may be reused.
    std::vector<uint8_t> buffer;

    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        auto entryA = a[i],
             entryB = b[j];

        ASSERT(entryA.id != 0);
        ASSERT(entryB.id != 0);

        // Note that
        //     a[k].id < a[k+1].id

        // Skip ahead if a and b are not at the same net object
        if (entryA.id < entryB.id) {
            i++;
            continue;    
        } else if (entryA.id > entryB.id) {
            j++;
            continue;
        } else {
            i++;
            j++;
        }

        ASSERT(entryA.id == entryB.id);
        ASSERT(entryA.size == entryB.size);

        if (!exists(entryA.id)) {
            // TODO: Does this still happen?
            WARN(net) << "Don't have net entity #" << entryA.id
                      << " in tick #" << a.tick() << " (interpolating)";
            continue;
        }

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

        if (!exists(entry.id)) {
            INFO(net) << "Don't have net entity #" << entry.id
                      << " in tick #" << store.tick();
            continue; // Entity was removed
        }

        //INFO(net) << "Applying " << entry.id;
        
        NetComponent const* component = get(entry.id);
        
        for (auto state : component->getStates()) {
            state->store(store.data(offset));
            offset += state->type().size;
        }
    }
}

void NetSystem::registerType(NetEntityType const& type) {
    entityTypes[type.typeId] = std::move(type);
}

NetEntityType const& NetSystem::getType(NetEntityTypeId typeId) const {
    auto entityType = entityTypes.find(typeId);
    ASSERT(entityType != entityTypes.end());

    return entityType->second;
}

Entity* NetSystem::createEntity(NetEntityTypeId typeId, NetEntityId id,
        ClientId owner) {
    auto entityType = getType(typeId);
    auto entity = getManager()->create(entityType.make(id, owner));

    auto netComponent = entity->component<NetComponent>();

#ifndef NDEBUG
    ASSERT_MSG(netComponent, "Entity " << id << " has no NetComponent.");
    ASSERT(netComponent->getNetTypeId() == typeId);
    ASSERT(netComponent->getNetId() == id);
    ASSERT(netComponent->getOwner() == owner);
#endif

    return entity;
}

} // namespace game

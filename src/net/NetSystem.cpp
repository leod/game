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
    TRACE(net) << "Creating net entity #" << component->getNetId();

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

void NetSystem::storeStateInArray(NetComponent const* component,
                                  std::vector<uint8_t>& out) const {
    {
        size_t size = 0;
        for (auto state : component->getStates())
            size += state->type().size;
        out.resize(size);
    }

    size_t index = 0;
    for (auto state : component->getStates()) {
        state->load(&out[index]);
        index += state->type().size;
    }
}

void NetSystem::loadStateFromArray(NetComponent* component,
                                   std::vector<uint8_t> const& in) const {
    size_t index = 0;
    for (auto state : component->getStates()) {
        ASSERT(index < in.size());

        state->store(&in[index]);
        index += state->type().size;
    }

    ASSERT(index == in.size());
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

        if (!exists(netId)) {
            INFO(net) << "Don't have net entity #" << netId << " (reading)";
            continue;
        }

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
    // Buffer for the interpolation result.
    // Kept outside of the for loop so the memory may be reused.
    std::vector<uint8_t> buffer;

    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        auto entryA = a[i],
             entryB = b[j];

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
            // As far as I can tell this can happen if a CreateEntityMessage
            // arrives after the first states it appears in.
            // This happens because we send the states unreliably and the
            // messages reliably.
            INFO(net) << "Don't have net entity #" << entryA.id
                      << " in tick #" << a.tick() << " (interpolating)";
            // TODO
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
            std::cout << "NetSystem: Don't have net entity #" << entry.id
                      << " in tick #" << store.tick() << std::endl;
            continue; // Entity was removed
        }
        
        NetComponent const* component = get(entry.id);
        
        for (auto state : component->getStates()) {
            state->store(store.data(offset));
            offset += state->type().size;
        }
    }
}

Entity* NetSystem::createEntity(NetEntityTypeId typeId, NetEntityId id,
        ClientId owner, InitialState const& state) {
    auto maker = entityTypes.find(typeId);
    ASSERT(maker != entityTypes.end());

    auto entity = getManager()->create(maker->second(id, owner));

    auto netComponent = entity->component<NetComponent>();

#ifndef NDEBUG
    ASSERT_MSG(netComponent, "Entity " << id << " has no NetComponent.");
    ASSERT(netComponent->getNetTypeId() == typeId);
    ASSERT(netComponent->getNetId() == id);
    ASSERT(netComponent->getOwner() == owner);
#endif

    loadStateFromArray(netComponent, state);

    return entity;
}

} // namespace game

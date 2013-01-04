#include "net/NetSystem.hpp"

#include <vector>

#include "core/Error.hpp"
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

    return componentIt->second;
}

NetComponent const* NetSystem::get(NetEntityId id) const {
    auto componentIt = components.find(id);

    ASSERT_MSG(componentIt != components.end(),
               "Net entity " << id << " not found");

    return componentIt->second;
}

void NetSystem::writeRawStates(BitStreamWriter& stream) const {
    iterate([&] (NetComponent const* component) {
        write(stream, component->getNetId());

        // Meh. My current interface for NetState requires me to have
        // a temporary buffer for load.
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

} // namespace game

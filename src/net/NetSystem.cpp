#include "net/NetSystem.hpp"

#include <vector>

#include "core/Error.hpp"
#include "util/BitStream.hpp"
#include "net/NetState.hpp"

namespace game {

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

} // namespace game

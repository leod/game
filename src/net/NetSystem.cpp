#include "net/NetSystem.hpp"

#include "util/BitStream.hpp"

namespace game {

void NetSystem::writeRawStates(BitStreamWriter& stream) const {
    iterate([&] (NetComponent const* component) {
        write(stream, component->getNetId());

        for (auto state : component->getStates()) {
            void* buffer = alloca(state->type().size);

            state->load(buffer);
            state->type().writer(stream, buffer);
        }
    });
}

} // namespace game

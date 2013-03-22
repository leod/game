#include "net/NetEntityType.hpp"

#include "util/BitStream.hpp"
#include "core/EntityManager.hpp"
#include "net/NetComponent.hpp"
#include "net/NetStateStore.hpp"

namespace game {
    
void NetEntityType::readStates(BitStreamReader& stream,
                               NetEntityId netId,
                               NetStateStore& store) {
    size_t requiredSize = 0;
    for (auto state : states)  
        requiredSize += state->size;

    uint8_t* buffer = store.allocate(netId, requiredSize);

    for (auto state : states) {

    }
}

} // namespace game

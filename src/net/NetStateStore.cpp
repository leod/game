#include "net/NetStateStore.hpp"

namespace game {

uint8_t* NetStateStore::allocate(NetEntityId id, size_t size) {
    size_t oldSize = buffer.size();
    buffer.resize(oldSize + size); 

    Entry newEntry = {
        id,
        size,
        &buffer[oldSize]
    };
    entries.push_back(newEntry);

    return newEntry.data;
}

size_t NetStateStore::size() const {
    return entries.size();
}

uint8_t const* NetStateStore::operator[](size_t index) const {
    return entries.at(index).data;
}

} // namespace game

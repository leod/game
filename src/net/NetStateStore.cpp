#include "net/NetStateStore.hpp"

namespace game {

uint8_t* NetStateStore::allocate(NetEntityId id, size_t size) {
    size_t oldSize = buffer.size();
    buffer.resize(oldSize + size); 

    Entry newEntry = {
        id,
        size,
        oldSize
    };
    entries.push_back(newEntry);

    return &buffer[oldSize];
}

size_t NetStateStore::size() const {
    return entries.size();
}

NetStateStore::Entry const& NetStateStore::operator[](size_t index) const {
    return entries.at(index);
}

uint8_t const* NetStateStore::data(size_t offset) const {
    return &buffer[offset];
}

} // namespace game

#pragma once

#include <cstdint>

#include "net/NetComponent.hpp"

namespace game {

struct NetStateStore {
    struct Entry {
        NetEntityId id;
        size_t size;

        size_t offset; // Offset in buffer
    };

    // Adds a new entry. The returned address should not be stored and only be
    // used to initialize the entry data.
    uint8_t* allocate(NetEntityId, size_t);

    // Returns the number of entries
    size_t size() const;

    // Returns the nth entry
    Entry const& operator[](size_t) const;

    // Returns a pointer to the data located at the given offset in the buffer.
    // Do not store the pointer.
    uint8_t const* data(size_t) const;

private:
    std::vector<uint8_t> buffer;
    std::vector<Entry> entries;
};

// TODO: At some point we'll need to cache some memory here probably.

} // namespace game

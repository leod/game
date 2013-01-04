#pragma once

#include <cstdint>

#include "net/NetComponent.hpp"

namespace game {

struct NetStateStore {
    struct Entry {
        NetEntityId id;
        size_t size;

        uint8_t const* data; // Points into buffer
    };

    // Adds a new entry
    uint8_t* allocate(NetEntityId, size_t);

    // Returns the number of entries
    size_t size() const;

    // Returns the nth entry
    Entry const& operator[](size_t) const;

private:
    std::vector<uint8_t> buffer;
    std::vector<Entry> entries;
};

// TODO: At some point we'll need to cache some memory here probably.

} // namespace game

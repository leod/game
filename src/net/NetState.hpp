#pragma once

namespace game {

struct NetState {
    virtual void serialize(BitStream&) = 0;
    virtual void unserialize(BitStream&) = 0;
};

} // namespace game

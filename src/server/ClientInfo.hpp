#pragma once

#include <string>

#include <enet/enet.h>

#include "net/Definitions.hpp"

namespace game {

struct ClientInfo {
    ClientId id;

    ENetPeer* peer;

    std::string name;
};

} // namespace game

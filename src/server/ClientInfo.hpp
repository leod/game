#pragma once

#include <string>

#include <enet/enet.h>

#include "net/Definitions.hpp"

namespace game {

struct PlayerInputComponent;

struct ClientInfo {
    ClientId id;
    ENetPeer* peer;
    std::string name;
    Entity* entity;
    bool connected;

    ClientInfo()
        : id(0), peer(nullptr), name(""), entity(nullptr), connected(true) {
    }
};

} // namespace game

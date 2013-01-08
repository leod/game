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
    PlayerInputComponent* inputComponent;
};

} // namespace game

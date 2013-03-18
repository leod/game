#pragma once

#include "math/Math.hpp"
#include "net/Definitions.hpp"
#include "net/Message.hpp"

namespace game {

struct MessageHub;

typedef std::vector<uint8_t> InitialState;
DEFINE_MESSAGE(CreateEntityMessage,
               NetEntityTypeId, NetEntityId, ClientId, InitialState);

DEFINE_MESSAGE(RemoveEntityMessage, NetEntityId);
DEFINE_MESSAGE(LoggedInMessage, ClientId);
DEFINE_EMPTY_MESSAGE(DisconnectMessage);
DEFINE_MESSAGE(ClientConnectedMessage, ClientId, std::string);
DEFINE_MESSAGE(ClientDisconnectedMessage, ClientId);

void initializeMessageTypes();
    
} // namespace game  

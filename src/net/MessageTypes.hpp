#pragma once

#include "math/Math.hpp"
#include "net/Definitions.hpp"
#include "net/Message.hpp"

namespace game {

struct MessageHub;

DEFINE_MESSAGE(CreateEntityMessage,
               NetEntityTypeId, NetEntityId, ClientId, vec3);
DEFINE_MESSAGE(RemoveEntityMessage, NetEntityId);
DEFINE_MESSAGE(LoggedInMessage, ClientId);
DEFINE_EMPTY_MESSAGE(DisconnectMessage);
DEFINE_MESSAGE(ClientConnectedMessage, ClientId, std::string);
DEFINE_MESSAGE(ClientDisconnectedMessage, ClientId);

void initializeMessageTypes();
    
} // namespace game  

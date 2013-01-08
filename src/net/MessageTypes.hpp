#pragma once

#include "math/Math.hpp"
#include "net/Definitions.hpp"
#include "net/Message.hpp"

namespace game {

struct MessageHub;

struct CreateEntityMessage
    : Message<CreateEntityMessage,
              NetEntityTypeId,
              NetEntityId,
              ClientId,
              vec3>
{};

struct RemoveEntityMessage
    : Message<RemoveEntityMessage,
              NetEntityId>
{};

struct LoggedInMessage
    : Message<LoggedInMessage,
              ClientId>
{};

struct DisconnectMessage
    : Message<DisconnectMessage>
{};

struct ClientConnectedMessage
    : Message<ClientConnectedMessage,
              ClientId,
              std::string>
{};

struct ClientDisconnectedMessage
    : Message<ClientDisconnectedMessage,
              ClientId>
{};

MessageHub* makeMessageHub();
    
} // namespace game  

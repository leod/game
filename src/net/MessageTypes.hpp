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

struct LoginMessage
    : Message<LoginMessage,
              ClientId> {
};

MessageHub* makeMessageHub();
    
} // namespace game  

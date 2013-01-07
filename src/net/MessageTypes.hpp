#pragma once

#include "math/Math.hpp"
#include "net/NetComponent.hpp"
#include "net/Message.hpp"

namespace game {

struct MessageHub;

struct CreateEntity
    : Message<CreateEntity,
              NetEntityTypeId,
              NetEntityId,
              vec3>
{};

MessageHub* makeMessageHub();
    
} // namespace game  

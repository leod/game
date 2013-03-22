#pragma once

#include "net/Event.hpp"

namespace game {

DEFINE_ORDER(CreateEntityOrder, NetEntityTypeId, NetEntityId, ClientId);
DEFINE_ORDER(RemoveEntityOrder, NetEntityId);
DEFINE_ORDER(LoggedInOrder, ClientId);
DEFINE_ORDER(ClientConnectedOrder, ClientId, std::string);
DEFINE_ORDER(ClientDisconnectedOrder, ClientId);

DEFINE_WISH(Ping);
DEFINE_ORDER(Pong);

DEFINE_WISH(DisconnectWish);

} // namespace game

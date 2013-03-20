#pragma once

#include "net/Event.hpp"

namespace game {

typedef std::vector<uint8_t> InitialState;
DEFINE_ORDER(CreateEntityOrder,
             NetEntityTypeId, NetEntityId, ClientId, InitialState);
DEFINE_ORDER(RemoveEntityOrder, NetEntityId);
DEFINE_ORDER(LoggedInOrder, ClientId);
DEFINE_ORDER(ClientConnectedOrder, ClientId, std::string);
DEFINE_ORDER(ClientDisconnectedOrder, ClientId);

DEFINE_WISH(Ping);
DEFINE_ORDER(Pong);

DEFINE_WISH(DisconnectWish);

} // namespace game

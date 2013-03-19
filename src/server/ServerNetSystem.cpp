#include "server/ServerNetSystem.hpp"

#include "core/Log.hpp"
#include "net/EventTypes.hpp"
#include "physics/PhysicsComponent.hpp"

#include "server/Clients.hpp"

namespace game {

static CreateEntityOrder
makeCreateEntityOrder(NetSystem const* netSystem,
                      NetComponent const* component) {
    std::vector<uint8_t> initialState;
    netSystem->storeStateInArray(component, initialState);

    return CreateEntityOrder::make(component->getNetTypeId(),
                                   component->getNetId(),
                                   component->getOwner(),
                                   initialState);
}

ServerNetSystem::ServerNetSystem(EventHub&, Clients& clients)
    : clients(clients), netEntityCounter(0) {
}

void ServerNetSystem::onRegister(NetComponent* component) {
    NetSystem::onRegister(component);

    clients.broadcast(makeCreateEntityOrder(this, component));
}

void ServerNetSystem::onUnregister(NetComponent* component) {
    NetSystem::onUnregister(component);

    clients.broadcast<RemoveEntityOrder>(component->getNetId());
}

NetEntityId ServerNetSystem::makeNetEntityId() {
    return ++netEntityCounter;
}

void
ServerNetSystem::sendCreateEntityOrders(ClientInfo* const client) const {
    INFO(server) << "Replicating entities to client " << *client;

    iterate([&] (NetComponent const* component) {
        if (component->getOwner() == client->id)
            return;

        sendEvent(client->peer, makeCreateEntityOrder(this, component));
    });
}

} // namespace game

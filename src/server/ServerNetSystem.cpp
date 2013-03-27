#include "server/ServerNetSystem.hpp"

#include "core/Log.hpp"
#include "net/EventTypes.hpp"
#include "world/PhysicsComponent.hpp"

#include "server/Clients.hpp"

namespace game {

ServerNetSystem::ServerNetSystem(EventHub&, Clients& clients)
    : clients(clients), netEntityCounter(0) {
}

void ServerNetSystem::onRegister(NetComponent* component) {
    NetSystem::onRegister(component);

    clients.queueEvent<CreateEntityOrder>(
        component->getNetTypeId(),
        component->getNetId(), component->getOwner());
 }

void ServerNetSystem::onUnregister(NetComponent* component) {
    NetSystem::onUnregister(component);

    clients.queueEvent<RemoveEntityOrder>(component->getNetId());
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

        client->queueEvent<CreateEntityOrder>(clients.tick(),
            component->getNetTypeId(),
            component->getNetId(),
            component->getOwner());
    });
}

} // namespace game

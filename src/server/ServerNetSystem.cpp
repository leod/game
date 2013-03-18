#include "server/ServerNetSystem.hpp"

#include "core/Log.hpp"
#include "net/MessageTypes.hpp"
#include "physics/PhysicsComponent.hpp"

#include "server/Clients.hpp"

namespace game {

static CreateEntityMessage
makeCreateEntityMessage(NetSystem const* netSystem,
                        NetComponent const* component) {
    std::vector<uint8_t> initialState;
    netSystem->storeStateInArray(component, initialState);

    return CreateEntityMessage::make(component->getNetTypeId(),
                                     component->getNetId(),
                                     component->getOwner(),
                                     initialState);
}

ServerNetSystem::ServerNetSystem(Clients& clients)
    : clients(clients), netEntityCounter(0) {
}

void ServerNetSystem::onRegister(NetComponent* component) {
    NetSystem::onRegister(component);

    clients.broadcast(makeCreateEntityMessage(this, component));
}

void ServerNetSystem::onUnregister(NetComponent* component) {
    NetSystem::onUnregister(component);

    clients.broadcast<RemoveEntityMessage>(component->getNetId());
}

NetEntityId ServerNetSystem::makeNetEntityId() {
    return ++netEntityCounter;
}

void
ServerNetSystem::sendCreateEntityMessages(ClientInfo* const client) const {
    INFO(server) << "Replicating entities to client " << *client;

    iterate([&] (NetComponent const* component) {
        clients.getMessageHub().send(client->peer,
                                     makeCreateEntityMessage(this, component));
    });
}

} // namespace game

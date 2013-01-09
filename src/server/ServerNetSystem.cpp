#include "server/ServerNetSystem.hpp"

#include "net/MessageTypes.hpp"
#include "physics/PhysicsComponent.hpp"

#include "server/Clients.hpp"

namespace game {

static CreateEntityMessage
makeCreateEntityMessage(NetComponent const* component) {
    // TODO: Hack. I think I'll just send the complete state with the create
    //       message.
    vec3 position(0, 0, 0);
    if (auto physics = component->getEntity()->component<PhysicsComponent>())
        position = physics->getPosition(); 

    return CreateEntityMessage::make(component->getNetTypeId(),
                                     component->getNetId(),
                                     component->getOwner(),
                                     position);
}

ServerNetSystem::ServerNetSystem(Clients& clients)
    : clients(clients) {
}

void ServerNetSystem::onRegister(NetComponent* component) {
    NetSystem::onRegister(component);

    clients.broadcast(makeCreateEntityMessage(component));
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
    iterate([&] (NetComponent const* component) {
        clients.getMessageHub().send(client->peer,
                                     makeCreateEntityMessage(component));
    });
}


} // namespace game

#include "core/Component.hpp"

#include "core/Error.hpp"
#include "core/Entity.hpp"

namespace game {

Component::Component()
    : owner(nullptr) {
}

Component::~Component() {

}

Entity* Component::getOwner() {
    return owner;
}

Entity const* Component::getOwner() const {
    return owner;
}

EntityRegistry* Component::getRegistry() {
    return owner->registry;
}

EntityRegistry const* Component::getRegistry() const {
    return owner->registry;
}

void Component::setOwner(Entity* newOwner) {
    ASSERT_MSG(owner == nullptr, "Owner has already been set.");
    ASSERT_MSG(newOwner != nullptr, "Owner must be non-null.");

    owner = newOwner;
}

FamilyId Component::globalFamilyCounter;

} // namespace game

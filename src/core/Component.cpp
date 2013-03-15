#include "core/Component.hpp"

#include "core/Error.hpp"
#include "core/Entity.hpp"

namespace game {

Component::Component()
    : owner(nullptr) {
}

Component::~Component() {

}

Entity* Component::getEntity() {
    return owner;
}

Entity const* Component::getEntity() const {
    return owner;
}

EntityRegistry* Component::getEntities() {
    return owner->entities;
}

EntityRegistry const* Component::getEntities() const {
    return owner->entities;
}

void Component::print(std::ostream& os) const {
    os << typeid(*this).name();
}

void Component::setOwner(Entity* newOwner) {
    ASSERT_MSG(owner == nullptr, "Owner has already been set.");
    ASSERT_MSG(newOwner != nullptr, "Owner must be non-null.");

    owner = newOwner;
}

FamilyId Component::globalFamilyCounter;

std::ostream& operator<<(std::ostream& os, Component const* component) {
    component->print(os);
    return os;
}

} // namespace game

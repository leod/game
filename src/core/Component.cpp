#include "core/Component.hpp"

#include "core/Error.hpp"
#include "core/Entity.hpp"

namespace game {

ComponentBase::ComponentBase()
    : owner(nullptr) {
}

ComponentBase::~ComponentBase() {

}

Entity* ComponentBase::getEntity() {
    return owner;
}

Entity const* ComponentBase::getEntity() const {
    return owner;
}

EntityManager* ComponentBase::getManager() {
    return owner->entities;
}

EntityManager const* ComponentBase::getManager() const {
    return owner->entities;
}

void ComponentBase::print(std::ostream& os) const {
    os << demangle(typeid(*this).name());
}

void ComponentBase::setOwner(Entity* newOwner) {
    ASSERT_MSG(owner == nullptr, "Owner has already been set.");
    ASSERT_MSG(newOwner != nullptr, "Owner must be non-null.");

    owner = newOwner;
}

FamilyId ComponentBase::globalFamilyCounter;

std::ostream& operator<<(std::ostream& os, ComponentBase const* component) {
    ASSERT(component);
    component->print(os);
    return os;
}

} // namespace game

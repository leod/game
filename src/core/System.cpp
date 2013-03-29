#include "core/System.hpp"

namespace game {

SystemBase::SystemBase(FamilyId familyId)
    : familyId(familyId), entities(nullptr) {
}

SystemBase::~SystemBase() {
}

void SystemBase::onRegister(ComponentBase*) {
}

void SystemBase::onUnregister(ComponentBase*) {
}

FamilyId SystemBase::getFamilyId() const {
    return familyId;
}

EntityManager* SystemBase::getManager() {
    return entities;
}

EntityManager const* SystemBase::getManager() const {
    return entities;
}

} // namespace game

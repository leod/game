#include "core/System.hpp"

namespace game {

System::System(FamilyId familyId)
    : familyId(familyId), entities(nullptr) {
}

System::~System() {
}

void System::onRegister(Component*) {
}

void System::onUnregister(Component*) {
}

FamilyId System::getFamilyId() const {
    return familyId;
}

EntityManager* System::getManager() {
    return entities;
}

EntityManager const* System::getManager() const {
    return entities;
}

}

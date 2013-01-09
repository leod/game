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

EntityRegistry* System::getEntities() {
    return entities;
}

EntityRegistry const* System::getEntities() const {
    return entities;
}

}

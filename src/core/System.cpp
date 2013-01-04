#include "core/System.hpp"

namespace game {

System::System(FamilyId familyId)
    : familyId(familyId), registry(nullptr) {
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

EntityRegistry* System::getRegistry() {
    return registry;
}

EntityRegistry const* System::getRegistry() const {
    return registry;
}

}

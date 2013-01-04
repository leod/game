#include "core/System.hpp"

namespace game {

System::System(FamilyId familyId)
    : familyId(familyId), registry(nullptr) {
}

System::~System() {
}

FamilyId System::getFamilyId() const {
    return familyId;
}

void System::onRegister(Component*) {
}

void System::onUnregister(Component*) {
}

}

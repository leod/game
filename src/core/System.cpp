#include "core/System.hpp"

namespace game {

System::System(FamilyId familyId)
    : familyId(familyId) {
}

System::~System() {
}

FamilyId const System::getFamilyId() const {
    return familyId;
}

void System::onRegister(Component*) {
}

void System::onUnregister(Component*) {
}

}

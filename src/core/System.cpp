#include "core/System.hpp"

namespace game {

System::System(Family family)
    : family(family) {
}

System::~System() {
}

const Family System::getFamily() const {
    return family;
}

void System::onRegister(Component*) {
}

void System::onUnregister(Component*) {
}

}

#include "core/Component.hpp"

#include "core/Error.hpp"

namespace game {

Component::~Component() {

}

void Component::setOwner(Entity* newOwner) {
    ASSERT_MSG(owner == nullptr, "Owner has already been set.");
    ASSERT_MSG(newOwner != nullptr, "Owner must be non-null.");

    owner = newOwner;
}

} // namespace game

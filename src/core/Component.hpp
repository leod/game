#pragma once

#include <iostream>

#include <vector>
#include <map>

#include "core/Error.hpp"

namespace game {

typedef int FamilyId;

struct Entity;
struct EntityRegistry;

struct Component {
    Component();
    virtual ~Component();

    virtual FamilyId const getFamilyId() const = 0;

private:
    Entity* owner;

    void setOwner(Entity* newOwner);
    friend class EntityRegistry;

    static FamilyId globalFamilyCounter;
    template<typename T> friend class ComponentFamily;
};

// ComponentFamily should be used as a baseclass for components
// that define a family of components, as it automatically
// generates a globally unique ID for the family.
template<typename T>
struct ComponentFamily : public Component {
    static FamilyId staticGetFamilyId() {
        ASSERT_MSG(familyId.familyId != 0,
                   "AutoFamilyId not initialized properly.");

        return familyId.familyId;
    }

    virtual FamilyId const getFamilyId() const {
        return staticGetFamilyId();
    }

private:
    struct AutoFamilyId {
        FamilyId familyId;

        AutoFamilyId()
            : familyId(++Component::globalFamilyCounter) {
            std::cout << familyId << std::endl;
        }
    };

    static AutoFamilyId familyId;
};

template<typename T>
typename ComponentFamily<T>::AutoFamilyId ComponentFamily<T>::familyId;

template<typename T>
using ComponentBase = ComponentFamily<T>;


} // namespace game

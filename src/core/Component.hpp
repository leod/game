#pragma once

#include <vector>
#include <map>

#include "core/Error.hpp"

namespace game {

typedef int FamilyId;

struct Entity;
struct EntityRegistry;

struct Component {
    friend class EntityRegistry;
    template<typename T> friend class ComponentFamily;

    Component();
    virtual ~Component();

    virtual FamilyId getFamilyId() const = 0;

    Entity* getEntity();
    Entity const* getEntity() const;
    EntityRegistry* getEntities();
    EntityRegistry const* getEntities() const;

private:
    Entity* owner;

    void setOwner(Entity* newOwner);

    static FamilyId globalFamilyCounter;
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

    virtual FamilyId getFamilyId() const {
        return staticGetFamilyId();
    }

private:
    struct AutoFamilyId {
        FamilyId familyId;

        AutoFamilyId()
            : familyId(++Component::globalFamilyCounter) {
        }
    };

    static AutoFamilyId familyId;
};

template<typename T>
typename ComponentFamily<T>::AutoFamilyId ComponentFamily<T>::familyId;

template<typename T>
using ComponentBase = ComponentFamily<T>;

} // namespace game

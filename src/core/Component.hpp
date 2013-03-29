#pragma once

#include <ostream>
#include <vector>
#include <map>

#include "core/Error.hpp"

namespace game {

typedef int FamilyId;

struct Entity;
struct EntityManager;

struct ComponentBase {
    ComponentBase();
    virtual ~ComponentBase();

    virtual FamilyId getFamilyId() const = 0;

    Entity* getEntity();
    Entity const* getEntity() const;
    EntityManager* getManager();
    EntityManager const* getManager() const;

protected:
    virtual void print(std::ostream&) const;

private:
    friend struct EntityManager;
    template<typename T> friend struct Component;

    Entity* owner;
    void setOwner(Entity* newOwner); // Called by EntityManager at registration

    static FamilyId globalFamilyCounter;

    friend std::ostream& operator<<(std::ostream&, ComponentBase const*);
};

template<typename T>
struct Component : public ComponentBase {
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
typename Component<T>::AutoFamilyId Component<T>::familyId;

template<typename T>
using ComponentFamily = Component<T>;

} // namespace game

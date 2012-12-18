#pragma once

#include <vector>
#include <map>

namespace game {

typedef int FamilyId;

struct Entity;
struct EntityRegistry;

struct Component {
    virtual ~Component();

    virtual const FamilyId getFamilyId() const = 0;

private:
    Entity* owner;

    void setOwner(Entity* newOwner);
    friend class EntityRegistry;
};

// ComponentFamily should be used as a baseclass for components
// that define a family of components, as it automatically
// generates a globally unique ID for the family.
template <typename T>
struct ComponentFamily : public Component {
    static FamilyId staticGetFamilyId() {
        return familyId;
    }

    virtual const FamilyId getFamilyId() {
        return familyId;
    }

private:
    static FamilyId globalFamilyCounter;

    struct StaticCtor {
        StaticCtor() {
            familyId = globalFamilyCounter++;
        }
    };

    static StaticCtor staticCtor;
	
	static FamilyId familyId;
};

template <typename T>
FamilyId ComponentFamily<T>::familyId;


} // namespace game

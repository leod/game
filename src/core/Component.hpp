#pragma once

#include <vector>
#include <map>

namespace game {

typedef int Family;

struct Entity;
struct EntityRegistry;

struct Component {
    virtual ~Component();

    virtual const Family getFamily() const = 0;

private:
    Entity* owner;

    void setOwner(Entity *newOwner);
    friend class EntityRegistry;
};

// ComponentFamily should be used as a baseclass for components
// that define a family of components, as it automatically
// generates a globally unique ID for the family.
template <typename T>
struct ComponentFamily : public Component {
    static Family staticGetFamily() {
        return family;
    }

    virtual const Family getFamily() {
        return family;
    }

private:
    static Family globalFamilyCounter;

	struct StaticCtor {
		StaticCtor() {
			family = globalFamilyCounter++;
		}
	};

    static StaticCtor staticCtor;
	
	static Family family;
};

typedef std::map<Family, Component*> ComponentMap;
typedef std::vector<Component*> ComponentList;

} // namespace game

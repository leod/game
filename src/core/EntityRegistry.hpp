#pragma once

#include <boost/iterator/transform_iterator.hpp>

#include "core/Entity.hpp"
#include "core/System.hpp"
#include "core/Error.hpp"

namespace game {

template<typename T> using ComponentListT = std::vector<T*>;
typedef ComponentListT<Component> ComponentList;
typedef std::map<EntityId, Entity*> EntityMap;
typedef std::vector<System*> SystemList;
typedef std::map<FamilyId, System*> SystemMap;
typedef std::map<FamilyId, ComponentList> FamilyMap;

// Use boost::transform_iterator to create a custom component 
// iterator that hides the ugly downcasting we have to do because
// we lose type-information by only storing Component*s.
// This way, we can eat our cake and then have it.
namespace detail {
    template<typename T>
    struct CastComponent {
        T* operator()(Component* component) const {
            ASSERT(component != nullptr);

            T* result;
#ifndef NDEBUG
            result = dynamic_cast<T*>(component);
            ASSERT_MSG(result != nullptr, "Component has invalid family.");
#else
            // AFAIK, static_cast produces undefined behavior if
            // T isn't a subclass of Component.
            result = static_cast<T*>(component);
#endif
            return result;
        }
    };
}

template<typename T>
using ComponentItT =
    boost::transform_iterator<detail::CastComponent<T>,
                              typename ComponentList::iterator,
                              T*>;
typedef ComponentItT<Component*> ComponentIt;

struct EntityRegistry {
    EntityRegistry(SystemList);

    Entity* get(EntityId);
    Entity const* get(EntityId) const;

    // Creates a new entity from the given list of components and registers it.
    Entity* add(ComponentList);

    // Returns the system that is registered as being responsible for the
    // given type of component (= family).
    // For example, system(RenderSystem::staticGetFamily()) returns the
    // SystemBase<RenderComponent> instance that was passed to the constructor
    // of the EntityRegistry (or nullptr, if none was given).
    //
    // The template functions can be used to hide some of the ugly clutter.
    System* system(FamilyId);
    System const* system(FamilyId) const;

    template<typename T>
    T* system() {
        return dynamic_cast<T*>(system(T::staticGetFamilyId()));
    }

    template<typename T>
    T const* system() const {
        return dynamic_cast<T*>(system(T::staticGetFamilyId()));
    }

    // These methods return iterators that go over all components
    // that belong to a given family.
    // For example, familyBegin<RenderComponent>() would return all
    // components of type RenderComponent or derived.
    ComponentIt familyBegin(FamilyId);
    ComponentIt familyEnd(FamilyId);

    ComponentIt familyBegin(FamilyId) const;
    ComponentIt familyEnd(FamilyId) const;

    template<typename T>
    ComponentItT<T> familyBegin() {
        return ComponentItT<T>(families[T::staticGetFamilyId()].begin(),
                               detail::CastComponent<T>());
    }

    template<typename T>
    ComponentItT<T> familyEnd() {
        return ComponentItT<T>(families[T::staticGetFamilyId()].end(),
                               detail::CastComponent<T>());
    }

    template<typename T, typename Target>
    void withFamily(void (Target::*f)(ComponentItT<T>, ComponentItT<T>),
                    Target& target) {
        (target.*f)(familyBegin<T>(), familyEnd<T>());
    }

private:
    // FamilyId -> System*
    SystemMap systems;

    // FamilyId -> ComponentList
    FamilyMap families;

    // EntityId -> Entity*
    EntityMap entities;

    // Used to create unique EntityIds
    EntityId biggestId;
};

} // namespace game

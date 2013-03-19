#pragma once

#include <boost/iterator/transform_iterator.hpp>

#include "core/Error.hpp"
#include "core/Entity.hpp"

namespace game {

struct System;

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

} // namespace detail

template<typename T>
using ComponentItT =
    boost::transform_iterator<detail::CastComponent<T>,
                              typename ComponentList::iterator,
                              T*>;
template<typename T>
using ConstComponentItT =
    boost::transform_iterator<detail::CastComponent<T>,
                              typename ComponentList::const_iterator,
                              T const*>;
typedef ComponentItT<Component*> ComponentIt;

struct EntityManager {
    EntityManager(std::vector<System*> const&);

    Entity* get(EntityId);
    Entity const* get(EntityId) const;

    // Creates and registers an entity from the given list of components
    Entity* create(ComponentList&&);

    // Unregisters and deletes an entity
    void remove(Entity*);

    // Returns the system that is registered as being responsible for the
    // given type of component (= family).
    // For example, system(RenderSystem::staticGetFamilyId()) returns the
    // SystemBase<RenderComponent> instance that was passed to the constructor
    // of the EntityManager (or nullptr, if none was given).
    //
    // The template functions can be used to hide some of the ugly clutter.
    System* system(FamilyId);
    System const* system(FamilyId) const;

    template<typename T>
    T* system() {
        auto s = dynamic_cast<T*>(system(T::staticGetFamilyId()));
        ASSERT(s);
        return s;
    }

    template<typename T>
    T const* system() const {
        auto s = dynamic_cast<T const*>(system(T::staticGetFamilyId()));
        ASSERT(s);
        return s;
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
    
    template<typename T>
    ConstComponentItT<T> familyBegin() const {
        auto familyIt = families.find(T::staticGetFamilyId());
        ASSERT(familyIt != families.end());
        auto& family = familyIt->second;

        return ConstComponentItT<T>(family.begin(),
                                    detail::CastComponent<T>());
    }

    template<typename T>
    ConstComponentItT<T> familyEnd() const {
        auto familyIt = families.find(T::staticGetFamilyId());
        ASSERT(familyIt != families.end());
        auto& family = familyIt->second;

        return ConstComponentItT<T>(family.end(),
                                    detail::CastComponent<T>());
    }

private:
    std::map<FamilyId, System*> systems;
    std::map<FamilyId, ComponentList> families;
    std::map<EntityId, Entity*> entities;

    // Used to create unique EntityIds
    EntityId biggestId;
};

} // namespace game

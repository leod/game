#pragma once

#include <vector>
#include <map>
#include <functional>

#include "core/Error.hpp"
#include "core/Component.hpp"
#include "core/EntityManager.hpp"

namespace game {

struct SystemBase {
    friend struct EntityManager;

    SystemBase(FamilyId);
    virtual ~SystemBase();

    virtual void onRegister(ComponentBase*);
    virtual void onUnregister(ComponentBase*);

    FamilyId getFamilyId() const;
    EntityManager* getManager();
    EntityManager const* getManager() const;

private:
    FamilyId const familyId;

    EntityManager* entities;
};

template<typename T> struct System : public SystemBase {
    System()
        : SystemBase(T::staticGetFamilyId()) {
    }

    void onRegister(ComponentBase* c) override {
        T* t = dynamic_cast<T*>(c);
        ASSERT(t != nullptr);
        onRegister(t);
    }

    void onUnregister(ComponentBase* c) override {
        T* t = dynamic_cast<T*>(c);
        ASSERT(t != nullptr);
        onUnregister(t);
    }

    virtual void onRegister(T*) {
    }

    virtual void onUnregister(T*) {
    }

    static FamilyId staticGetFamilyId() {
        return T::staticGetFamilyId();
    }

    void iterate(std::function<void(T*)> const& f) {
        EntityManager* manager = getManager();
        auto it = manager->familyBegin<T>();
        auto end = manager->familyEnd<T>();

        for (; it != end; ++it) f(*it);
    }

    void iterate(std::function<void(T const*)> const& f) const {
        EntityManager const* manager = getManager();
        auto it = manager->familyBegin<T>();
        auto end = manager->familyEnd<T>();

        for (; it != end; ++it) f(*it);
    }
};

} // namespace game

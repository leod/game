#pragma once

#include <list>
#include <functional>

using namespace std::placeholders;

#define MEMBER_FN_0(a, fn) std::bind(&decltype(a)::fn, &a)
#define MEMBER_FN_3(a, fn) std::bind(&decltype(a)::fn, &a, _1, _2, _3)

namespace game {

// I might replace this by boost's signals

template<typename... Values>
struct Signal {
    template<typename F>
    void connect(F fn) {
        fns.push_back(fn);
    }

    template<typename C>
    void connect(C* object, void(C::*f)(Values...)) {
        auto wrapper = [=] (Values... values) {
            (object->*f)(values...);
        };

        connect(wrapper);
    }

    void operator()(Values... values) const {
        for (auto const& fn : fns)
            fn(values...); 
    }

private:
    std::list<std::function<void(Values...)>> fns;
};

} // namespace game

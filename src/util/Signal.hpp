#pragma once

#include <vector>
#include <functional>

#define MEMBER_FN_0(a, fn) std::bind(&decltype(a)::fn, a)

namespace game {

// I might replace this by boost's signals

template<typename... Values>
struct Signal {
    void connect(std::function<void(Values...)> fn) {
        fns.push_back(fn);
    }

    void operator()(Values... values) {
        for (auto fn : fns)
            fn(values...); 

    }

private:
    std::vector<std::function<void(Values...)>> fns;
};

} // namespace game

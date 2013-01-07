#pragma once

#include <vector>
#include <functional>

using namespace std::placeholders;

#define MEMBER_FN_0(a, fn) std::bind(&decltype(a)::fn, &a)
#define MEMBER_FN_3(a, fn) std::bind(&decltype(a)::fn, &a, _1, _2, _3)

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

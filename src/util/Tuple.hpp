#pragma once

#include <tuple>
#include <functional>

namespace game {
namespace tuple {

// Helper definitions for unpacking a tuple into a function call
// <http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer>
template<int...>
struct Seq {
};

template<int N, int... S>
struct Gens : Gens<N - 1, N - 1, S...> {
};

template<int... S>
struct Gens<0, S...> {
    typedef Seq<S...> Type;
};

// Unpack a tuple into the arguments of a function call
template<typename... Types>
struct Unpack {
    Unpack(std::tuple<Types...> const& m)
        : m(m) {
    }

    template<int... S, typename F, typename... Params>
    void callFn(Seq<S...>, F f, Params const&... params) const {
        f(params..., std::get<S>(m)...);
    }

    template<typename F, typename... Params>
    void call(F f, Params const&... params) const {
        callFn(typename Gens<sizeof...(Types)>::Type(), f, params...);
    }

private:
    std::tuple<Types...> const& m;
};

// Iterate through the members of a tuple via a templated functor
template<std::size_t N, typename F, typename... Types>
typename std::enable_if<(N == 0)>::type iterate(std::tuple<Types...>&, F&) {
}

template<std::size_t N, typename F, typename... Types>
typename std::enable_if<(N >= 1)>::type iterate(std::tuple<Types...>& m,
                                                F& fn) {
    fn(std::get<N - 1>(m));
    iterate<N - 1>(m, fn);
}

template<std::size_t N, typename F, typename... Types>
typename std::enable_if<(N == 0)>::type iterate(std::tuple<Types...> const&,
                                                F&) {
}

template<std::size_t N, typename F, typename... Types>
typename std::enable_if<(N >= 1)>::type iterate(std::tuple<Types...> const& m,
                                                F& fn) {
    fn(std::get<N - 1>(m));
    iterate<N - 1>(m, fn);
}

} // namespace tuple
} // namespace game

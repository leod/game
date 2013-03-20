#pragma once

#include <iostream>
#include <tuple>
#include <functional>
#include <cstdint>
#include <vector>

#include "math/Math.hpp"
#include "util/Tuple.hpp"

namespace game {

template<typename... Args>
std::ostream& operator<<(std::ostream& os, vec2 const& v) {
    return os << "vec2(" << v.x << ", "
                         << v.y << ")";
}

template<typename... Args>
std::ostream& operator<<(std::ostream& os, vec3 const& v) {
    return os << "vec3(" << v.x << ", "
                         << v.y << ", "
                         << v.z << ")";
}

template<std::size_t N, typename... Types>
typename std::enable_if<(N == 0)>::type
printTuple(std::ostream&, std::tuple<Types...> const&) {
}

template<std::size_t N, typename... Types>
typename std::enable_if<(N == 1)>::type
printTuple(std::ostream& os, std::tuple<Types...> const& m) {
    os << std::get<0>(m);
}

template<std::size_t N, typename... Types>
typename std::enable_if<(N >= 2)>::type
printTuple(std::ostream& os, std::tuple<Types...> const& m) {
    printTuple<N - 1>(os, m);
    os << ", " << std::get<N - 1>(m);
}

template<typename... Args>
std::ostream& operator<<(std::ostream& os, std::tuple<Args...> const& m) {
    os << "(";
    printTuple<(sizeof...(Args))>(os, m);
    return os << ")";
}

inline
std::ostream& operator<<(std::ostream& os, std::vector<uint8_t> const&) {
    return os << "<data>";
}

std::string demangle(std::string const&, bool qualified = false);

} // namespace game

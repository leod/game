#pragma once

#include <iostream>
#include <tuple>

#include "math/Math.hpp"

namespace game {

template<typename Ch, typename Traits, typename... Args>
std::ostream& operator<<(std::basic_ostream<Ch, Traits>& os,
        vec3 const& v) {
    return os << "vec3(" << v.x << ", "
                         << v.y << ", "
                         << v.z << ")";
}

// Adopted from <http://stackoverflow.com/a/6245777>.

template<std::size_t> struct IntCounterT {
}; 

template<typename Ch, typename Tr, typename Tuple, std::size_t I>
void printTuple(std::basic_ostream<Ch, Tr>& os, Tuple const& t,
        IntCounterT<I>){
  printTuple(os, t, IntCounterT<I-1>());
  os << ", " << std::get<I>(t);
}

template<typename Ch, typename Tr, typename Tuple>
void printTuple(std::basic_ostream<Ch, Tr>& os, Tuple const& t,
        IntCounterT<0>){
  os << std::get<0>(t);
}

template<typename Ch, typename Traits, typename... Args>
std::ostream& operator<<(std::basic_ostream<Ch, Traits>& os,
        std::tuple<Args...> const& t)
{
  os << "(";
  printTuple(os, t, IntCounterT<sizeof...(Args) - 1>());
  return os << ")";
}

} // namespace game

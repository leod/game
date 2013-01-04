#pragma once

#include <cstdint>
#include <tuple>
#include <functional>

#include "util/BitStream.hpp"
#include "net/MessageTypeInfo.hpp"

namespace game {

// Messages are defined by deriving a struct T from Message<T, Types...>.
// Types is a type tuple containing the data of the message.
//
// By deriving from Message<T, Types...>, functions for writing and
// reading the message into a bitstream are generated based on Types.
// Based on these functions, an instance of MessageTypeInfo is created,
// which can be used to read and write messages without knowing their
// concrete compile-time types.
//
// WARNING: Contains template poo.
typedef void UntypedMessage;

template<typename T, typename... Types>
struct Message {
    static MessageTypeInfo const typeInfo;

    std::tuple<Types...> m;

    Message() {

    }

    Message(Types const&... data)
        : m(data...) {
    }

    // TODO: Allow non-void functors
    template<typename F, typename... Params>
    void unpack(F f, Params const&... params) const {
        callFn(typename Gens<sizeof...(Types)>::Type(), f, params...);
    }

    UntypedMessage* toUntyped() {
        return static_cast<UntypedMessage*>(this);
    }

    UntypedMessage const* toUntyped() const {
        return static_cast<UntypedMessage*>(this);
    }

    static void write(BitStreamWriter& stream, UntypedMessage const* message) {
        Message const& self = *static_cast<Message const*>(message);

        WriteFunctor f(stream);
        self.iterate<sizeof...(Types)>(f);
    }

    static void read(BitStreamReader& stream, UntypedMessage* message) {
        Message& self = *static_cast<Message*>(message);

        ReadFunctor f(stream);
        self.iterate<sizeof...(Types)>(f);
    }

private:
    // Helper definitions for unpacking the tuple m into a function call
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

    template<int... S, typename F, typename... Params>
    void callFn(Seq<S...>, F f, Params const&... params) const {
        f(params..., std::get<S>(m)...);
    }

    // Iterate through the members of the tuple via template programming
    template<std::size_t N, typename F>
    typename std::enable_if<(N >= 1)>::type iterate(F& fn) {
        fn(std::get<N - 1>(m));
        iterate<N - 1>(fn);
    }

    template<std::size_t N, typename F>
    typename std::enable_if<(N == 0)>::type iterate(F&) {
    }

    template<std::size_t N, typename F>
    typename std::enable_if<(N >= 1)>::type iterate(F& fn) const {
        fn(std::get<N - 1>(m));
        iterate<N - 1>(fn);
    }

    template<std::size_t N, typename F>
    typename std::enable_if<(N == 0)>::type iterate(F&) const {
    }

    // Functors to write and read single members of the tuple
    struct WriteFunctor {
        BitStreamWriter& stream;

        WriteFunctor(BitStreamWriter& stream)
            : stream(stream) {
        }

        template<typename U>
        void operator()(const U& t) {
            ::game::write(stream, t);
        }
    };

    struct ReadFunctor {
        BitStreamReader& stream;

        ReadFunctor(BitStreamReader& stream)
            : stream(stream) {
        }

        template<typename U>
        void operator()(U& t) {
            ::game::read(stream, t);
        }
    };
};

template<typename T, typename... Types>
MessageTypeInfo const Message<T, Types...>::typeInfo = {
    sizeof(T),
    &T::write,
    &T::read
};

} // namespace game

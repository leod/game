#pragma once

#include <cstdint>
#include <functional>
#include <ostream>
#include <iostream>

#include "util/BitStream.hpp"
#include "util/Print.hpp"
#include "util/Tuple.hpp"
#include "net/MessageType.hpp"

#define DEFINE_MESSAGE(_name, ...) \
    struct _name \
        : public Message<_name, __VA_ARGS__> { \
        static char const* name() { return #_name; } \
    }
#define DEFINE_EMPTY_MESSAGE(_name) \
    struct _name \
        : public Message<_name> { \
        static char const* name() { return #_name; } \
    }

namespace game {

template<typename T, typename... Args>
struct Message;

template<typename T, typename... Args>
std::ostream& operator<<(std::ostream&, Message<T, Args...> const&);

// A message type T is defined by deriving a struct T from
// Message<T, Types...>. Types is a type tuple containing the data types
// of the message.
//
// By deriving from Message<T, Types...>, static functions for writing and
// reading the message into a bitstream are generated.
// T should contain a static function name() returning the name of the message
// type. Use DEFINE_MESSAGE to avoid having to write this boilerplate.
//
// Based on these functions, an instance of MessageType is created,
// which can be used to read and write messages without knowing their
// concrete compile-time types.
typedef void UntypedMessage;

template<typename T, typename... Types>
struct Message {
    static MessageType type;

    std::tuple<Types...> m;

    Message() {
    }

    static T make(Types const&... data) {
        T result;
        result.m = std::make_tuple(data...);
        return result;
    }

    // Override this if you want this message to have a reserved id.
    // Returns 0 otherwise.
    static boost::optional<MessageId> reservedId() {
        return boost::none;
    }

    template<typename F, typename... Params>
    void unpack(F f, Params const&... params) const {
        tuple::Unpack<Types...>(m).call(f, params...);
    }

    UntypedMessage* toUntyped() {
        return static_cast<UntypedMessage*>(this);
    }

    UntypedMessage const* toUntyped() const {
        return static_cast<UntypedMessage const*>(this);
    }

    // Implement MessageType
    static void init(UntypedMessage* message) {
        new(message) Message;
    }

    static void write(BitStreamWriter& stream, UntypedMessage const* message) {
        auto self = static_cast<Message const*>(message);

        WriteFunctor f(stream);
        tuple::iterate<sizeof...(Types)>(self->m, f);
    }

    static void read(BitStreamReader& stream, UntypedMessage* message) {
        auto self = static_cast<Message*>(message);

        ReadFunctor f(stream);
        tuple::iterate<sizeof...(Types)>(self->m, f);
    }

    static void print(std::ostream& os, UntypedMessage const* message) {
        auto self = static_cast<Message const*>(message);

        os << *self;
    }

private:
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
MessageType Message<T, Types...>::type = {
    sizeof(T),
    &T::init,
    &T::write,
    &T::read,
    &T::name,
    &T::print
};

template<typename T, typename... Args>
std::ostream& operator<<(std::ostream& o, Message<T, Args...> const& message) {
    return o << T::name() << message.m;
}

} // namespace game

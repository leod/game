#pragma once

#include <map>
#include <ostream>

#include "util/Signal.hpp"
#include "util/BitStream.hpp"
#include "util/Tuple.hpp"
#include "util/Print.hpp"

#define DEFINE_EVENT_WITH_BASE(_base, _name, ...) \
    struct _name \
        : public _base<_name, ##__VA_ARGS__> { \
        static char const* name() { return #_name; } \
    }

#define DEFINE_EVENT(_name, ...) \
    DEFINE_EVENT_WITH_BASE(::game::Event, _name, ##__VA_ARGS__)
                                      
namespace game {

typedef uint8_t EventTypeId; // 0 is reserved

struct EventBase;

struct EventType {
    typedef EventBase* (*Create)();

    // Calls the constructor of the event type on some memory location
    typedef void (*Init)(void*);

    // Returns the name of this event type
    typedef char const* (*Name)();

    size_t const size;

    Create const create;
    Init const init;
    Name const name;

    EventType(size_t, Create, Init, Name);

    EventTypeId id() const;

    // Registers a list of event types globally and generates ids for the
    // event types. As the ids should be the same on all peers, the order
    // of the types in the vector is important.
    // This should be called exactly once at startup.
    static void initialize(std::vector<EventType*> const&);

    static EventType const& getById(EventTypeId);

private:
    EventTypeId id_;

    static bool initialized;

    // Indexed by the id of the event type
    static std::vector<EventType*> types;
};

struct EventBase {
    virtual ~EventBase();
    virtual void write(BitStreamWriter&) const = 0; 
    virtual void read(BitStreamReader&) = 0;
    virtual void print(std::ostream&) const = 0;
    virtual EventType const& getType() const = 0;
};

template<typename E, typename... Types>
struct Event : public EventBase {
    static EventType type;

    std::tuple<Types...> m;

    Event() {
    }

    static E make(Types const&... data) {
        E result;
        result.m = std::make_tuple(data...);
        return result;
    }

    template<typename F, typename... Params>
    void unpack(F f, Params const&... params) const {
        tuple::Unpack<Types...>(m).call(f, params...);
    }

    // Implement EventType
    static EventBase* create() {
        return new E;
    }

    static void init(void* event) {
        new(event) E;
    }

    // Implement EventBase
    void write(BitStreamWriter& stream) const override {
        WriteFunctor f(stream);
        tuple::iterate<sizeof...(Types)>(m, f);
    }

    void read(BitStreamReader& stream) override {
        ReadFunctor f(stream);
        tuple::iterate<sizeof...(Types)>(m, f);
    }

    void print(std::ostream& os) const override {
        os << E::name() << m;
    }

    EventType const& getType() const override {
        return type;
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

template<typename E, typename... Types>
EventType Event<E, Types...>::type = {
    sizeof(E),
    &E::create,
    &E::init,
    &E::name,
};

std::ostream& operator<<(std::ostream&, EventBase const&);

struct EventHub {
    template<typename E, typename... Types>
    void emit(Types&&... args) const {
        E event = E::make(args...); 
        emit(event);
    }

    template<typename E>
    void emit(E const& event) const {
        emitBase(&event);
    }

    void emitBase(EventBase const* event) const {
        auto signalIt = signals.find(event->getType().id());
        if (signalIt == signals.end())
            return;

        auto signal = signalIt->second;
        signal(event);
    }

    template<typename E, typename F>
    void subscribe(F f) {
        auto wrapper = [=] (EventBase const* eventHandle) {
            auto event = static_cast<E const*>(eventHandle);
            event->unpack(f); 
        };

        signals[E::type.id()].connect(wrapper);
    }

    template<typename E, typename Receiver, typename R, typename... A>
    void subscribe(Receiver* receiver, R(Receiver::*f)(A...)) {
        auto wrapper = [=] (A const&... a) {
            (receiver->*f)(a...);
        };

        subscribe<E>(wrapper);
    }

private:
    std::map<EventTypeId, Signal<EventBase const*>> signals;
};

} // namespace game

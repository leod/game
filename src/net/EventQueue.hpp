#pragma once

#include <queue>
#include <tuple>

#include "net/Definitions.hpp"

namespace game {

struct BitStreamReader;
struct BitStreamWriter;
struct EventBase;
struct EventHub;

struct EventQueue {
    template<typename E, typename... Args>
    void add(Tick tick, Args const&... args) {
        E* event = new E;
        event->m = std::make_tuple(args...);

        add(tick, event);
    }

    bool addFromStream(Tick, BitStreamReader&);

    bool empty();
    Tick frontTick(); 

    void popAndEmit(EventHub&);
    void popAndWrite(BitStreamWriter&);

private:
    void add(Tick, EventBase*);

    std::queue<std::pair<Tick, EventBase*>> events;
};

} // namespace game

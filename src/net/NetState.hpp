#pragma once

#include <cstdlib>

namespace game {

struct BitStreamReader;
struct BitStreamWriter;

typedef void UntypedState;

typedef void (*NetStateWriter)(BitStreamWriter&, UntypedState const* in);
typedef void (*NetStateReader)(BitStreamReader&, UntypedState* out);
typedef void (*NetStateInterpolator)(UntypedState const* a,
                                     UntypedState const* b,
                                     float t, UntypedState* out);

struct NetStateType {
    size_t size;

    NetStateWriter writer;
    NetStateReader reader;
    NetStateInterpolator interpolator;
};

template<typename T>
NetStateType makeNetStateType(void (*writer)(BitStreamWriter&, T const*),
                              void (*reader)(BitStreamReader&, T*),
                              void (*interpolator)(T const*, T const*,
                                                   float, T*)) {
    NetStateType type = {
        sizeof(T),
        static_cast<NetStateWriter>(writer),
        static_cast<NetStateReader>(reader),
        static_cast<NetStateInterpolator>(interpolator)
    };

    return type;
}

struct NetState {
    virtual NetStateType const& type() = 0; 
    virtual void load(UntypedState*) = 0;
    virtual void store(UntypedState const*) = 0;
};

template<typename State>
struct NetStateBase : public NetState {
    virtual State load() = 0;
    virtual void store(State const&) = 0;

    void load(UntypedState* out) {
        State state = load();
        memcpy(out, reinterpret_cast<void*>(&state), sizeof(State));
    }

    void store(UntypedState const* state) {
        store(*static_cast<State const*>(state));
    }
};

}

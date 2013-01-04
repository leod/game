#pragma once

#include <cstring>

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

    NetStateWriter write;
    NetStateReader read;
    NetStateInterpolator interpolate;
};

template<typename T>
NetStateType makeNetStateType(void (*write)(BitStreamWriter&, T const*),
                              void (*read)(BitStreamReader&, T*),
                              void (*interpolate)(T const*, T const*,
                                                  float, T*)) {
    NetStateType type = {
        sizeof(T),
        reinterpret_cast<NetStateWriter>(write),
        reinterpret_cast<NetStateReader>(read),
        reinterpret_cast<NetStateInterpolator>(interpolate)
    };

    return type;
}

struct NetState {
    virtual NetStateType const& type() const = 0; 
    virtual void load(UntypedState*) const = 0;
    virtual void store(UntypedState const*) = 0;
};

template<typename State>
struct NetStateBase : public NetState {
    virtual State load() const = 0;
    virtual void store(State const&) = 0;

    // Implements NetState
    void load(UntypedState* out) const {
        State state = load();
        memcpy(out, reinterpret_cast<void*>(&state), sizeof(State));
    }

    // Implements NetState
    void store(UntypedState const* state) {
        store(*static_cast<State const*>(state));
    }
};

}

#pragma once

#include "util/BitStream.hpp"

namespace game {

typedef int8_t MessageId;
typedef void UntypedMessage;

typedef void (*MessageWriter)(UntypedMessage const*, BitStreamWriter&);
typedef void (*MessageReader)(UntypedMessage*, BitStreamReader&);

struct MessageTypeInfo {
    size_t const size;
    MessageWriter const write;
    MessageReader const read; 
};

} // namespace game

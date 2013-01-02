#pragma once

#include "util/BitStream.hpp"

namespace game {

typedef int8_t MessageId;
typedef void UntypedMessage;

typedef void (*MessageWriter)(BitStreamWriter&, UntypedMessage const*);
typedef void (*MessageReader)(BitStreamReader&, UntypedMessage*);

struct MessageTypeInfo {
    size_t const size;
    MessageWriter const write;
    MessageReader const read; 
};

} // namespace game

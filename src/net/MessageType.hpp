#pragma once

#include <iostream>
#include <vector>
#include <boost/optional.hpp>

#include "core/Log.hpp"
#include "util/BitStream.hpp"

namespace game {

typedef int8_t MessageId; // 0 is reserved
typedef void UntypedMessage;

typedef void (*MessageInit)(UntypedMessage*);

typedef void (*MessageWrite)(BitStreamWriter&, UntypedMessage const*);
typedef void (*MessageRead)(BitStreamReader&, UntypedMessage*);

typedef char const* (*MessageName)();
typedef void (*MessagePrint)(std::ostream&, UntypedMessage const*);

struct MessageType {
    size_t const size;

    MessageInit const init;

    MessageWrite const write;
    MessageRead const read; 

    MessageName const name;
    MessagePrint const print;

    MessageType(size_t, MessageInit, MessageWrite, MessageRead, MessageName,
                MessagePrint);

    MessageId id() const;
    std::string toString(UntypedMessage const*) const;

    static void initialize(std::vector<MessageType*> const&);
    static MessageType const& getById(MessageId);

private:
    MessageId id_;

    static bool initialized;

    // Indexed by the id of the message type
    static std::vector<MessageType*> types;
};

} // namespace game

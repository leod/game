#include "net/MessageType.hpp"

#include <algorithm>
#include <sstream>

#include "core/Error.hpp"

namespace game {


MessageType::MessageType(size_t size, MessageInit init, MessageWrite write,
                         MessageRead read, MessageName name, MessagePrint print)
    : size(size), init(init), write(write), read(read), name(name),
      print(print) {
}

MessageId MessageType::id() const {
    ASSERT(initialized);
    return id_;
}

std::string MessageType::toString(UntypedMessage const* message) const {
    std::stringstream ss;
    print(ss, message);
    return ss.str();
}

void MessageType::initialize(std::vector<MessageType*> const& types) {
    MessageType::types = types;

    // Assign ids
    MessageId id = 0;
    for (auto type : types)
        type->id_ = id++;

    // Log
    {
        std::stringstream ss;

        for (auto type : types)
            ss << (int)type->id_ << ":" << type->name() << ", ";
        
        INFO(net) << "Message types: "
                  << ss.str().substr(0, ss.str().size() - 2);
    }

    initialized = true;
}

MessageType const& MessageType::getById(MessageId id) {
    ASSERT(initialized);
    auto type = types[id];
    ASSERT(type);
    ASSERT(type->id_ == id);
    return *type;
}

bool MessageType::initialized = false;
std::vector<MessageType*> MessageType::types;

} // namespace game

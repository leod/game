#include "util/BitStream.hpp"

#include <algorithm>

#include "core/Error.hpp"

namespace game {

BitStreamWriter::BitStreamWriter() {
}

void BitStreamWriter::writeBytes(uint8_t const* data, size_t size) {
    ASSERT(data != nullptr);
    ASSERT(size > 0);

    auto oldSize = buffer.size();
    buffer.resize(oldSize + size);
    std::copy(data, data + size, &buffer[oldSize]);
}

uint8_t const* BitStreamWriter::ptr() const {
    ASSERT(buffer.size() > 0);

    return &buffer[0];
}

BitStreamReader::BitStreamReader(uint8_t const* buffer, size_t bufferLength)
    : buffer(buffer), bufferLength(bufferLength), index(0) {
    ASSERT(buffer != nullptr);
    ASSERT(bufferLength > 0);
}

void BitStreamReader::readBytes(uint8_t* out, size_t size) {
    ASSERT(out != nullptr);
    ASSERT(size > 0);
    ASSERT(index + size <= bufferLength);

    std::copy(buffer + index, buffer + index + size, out);
}

} // namespace game

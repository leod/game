#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_pod.hpp>

namespace game {

// These bit streams operate on a byte level... for now. :P
// Doesn't take care of endianness issues.

struct BitStreamWriter {
    BitStreamWriter();

    void writeBytes(uint8_t const* data, size_t size);

    uint8_t const* ptr() const;

private:
    std::vector<uint8_t> buffer;
};

struct BitStreamReader {
    BitStreamReader(uint8_t const* buffer, size_t bufferLength);

    void readBytes(uint8_t* out, size_t size);

private:
    uint8_t const* buffer;
    size_t const bufferLength;

    size_t index;
};

template<typename T,
         typename boost::enable_if<boost::is_pod<T>, int>::type = 0>
void write(BitStreamWriter& stream, const T& value) {
    stream.writeBytes(reinterpret_cast<uint8_t const*>(&value), sizeof(T));
}

void write(BitStreamWriter& stream, const std::string& str) {
    write(stream, str.size());
    stream.writeBytes(
            reinterpret_cast<const uint8_t*>(str.c_str()),
            str.size());
}

template<typename T>
void write(BitStreamWriter& stream, const std::vector<T>& v) {
    write(stream, v.size());

    for (auto& e : v)
        write(stream, e);
}

template<typename T,
         typename boost::enable_if<boost::is_pod<T>, int>::type = 0>
void read(BitStreamReader& stream, T& value) {
    stream.readBytes(reinterpret_cast<uint8_t*>(&value), sizeof(T));
}

void read(BitStreamReader& stream, std::string& str) {
    size_t size;
    read(stream, size);
    str.resize(size);

    // This might just explode on you.
    stream.readBytes(
            reinterpret_cast<uint8_t*>(const_cast<char*>(str.c_str())),
            size);
}

template<typename T>
void read(BitStreamReader& stream, std::vector<T>& v) {
    size_t size;
    read(stream, size);
    v.resize(size);

    for (auto& e : v)
        read(stream, e);
}

} // namespace game

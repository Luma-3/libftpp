

#include <cstring>
#include <stdexcept>
#include <vector>

#include "data_buffer.hpp"

template <typename T>
DataBuffer& DataBuffer::operator<<(const T& obj)
{
    const uint8_t* data = reinterpret_cast<const uint8_t*>(&obj);
    _buffer.insert(_buffer.end(), data, data + sizeof(T));
    return *this;
}

template <typename T>
DataBuffer& DataBuffer::operator>>(T& obj)
{
    if (sizeof(T) + _cursor > _buffer.size())
    {
        throw std::runtime_error("read buffer out limit");
    }
    std::memcpy(&obj, _buffer.data() + _cursor, sizeof(T));
    _cursor += sizeof(T);
    return *this;
}

template <typename T>
DataBuffer& DataBuffer::operator<<(const std::vector<T>& vec)
{
    size_t length = vec.size();
    *this << length;
    for (size_t i = 0; i < length; ++i)
    {
        (*this) << vec[i];
    }
    return *this;
}

template <typename T>
DataBuffer& DataBuffer::operator>>(std::vector<T>& vec)
{
    size_t length;
    *this >> length;
    for (size_t i = 0; i < length; ++i)
    {
        T data;
        (*this) >> data;
        vec.push_back(data);
    }
    return *this;
}

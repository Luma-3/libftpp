#include "data_buffer.hpp"

#include <cstddef>
#include <string>

DataBuffer::DataBuffer() : _cursor(0) {}

DataBuffer::~DataBuffer() {}

void DataBuffer::clear()
{
    _buffer.clear();
    _cursor = 0;
}

DataBuffer& DataBuffer::operator<<(const std::string& str)
{
    const size_t lenght = str.size();
    *this << lenght;
    _buffer.insert(_buffer.end(), str.begin(), str.end());
    return *this;
}

DataBuffer& DataBuffer::operator>>(std::string& str)
{
    size_t lenght;
    *this >> lenght;
    str.assign(_buffer.begin() + _cursor, _buffer.begin() + _cursor + lenght);
    _cursor += lenght;
    return *this;
}

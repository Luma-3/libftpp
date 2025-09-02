#ifndef _DATA_BUFFER_HPP
#define _DATA_BUFFER_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class DataBuffer
{
private:
    std::vector<uint8_t> _buffer;
    size_t               _cursor;

public:
    DataBuffer();
    ~DataBuffer();

    template <typename T>
    DataBuffer& operator<<(const T& obj);

    template <typename T>
    DataBuffer& operator>>(T& obj);

    DataBuffer& operator<<(const std::string& str);
    DataBuffer& operator>>(std::string& str);

    template <typename T>
    DataBuffer& operator<<(const std::vector<T>& vec);

    template <typename T>
    DataBuffer& operator>>(std::vector<T>& vec);

    void clear();
};

#include "../template/data_buffer.tpp"

#endif // !_DATA_BUFFER_HPP

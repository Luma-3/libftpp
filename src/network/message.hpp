#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP

#include <concepts>
#include <cstddef>
#include <span>
#include <string>
#include <vector>

template <typename T>
concept Serializable = requires(const T& t) {
    { t.serialize() } -> std::convertible_to<std::span<const std::byte>>;
};

class Message
{
public:
    typedef int Type;

    Message(Type type);

    Type getType() const;

    template <typename T>
    Message& operator<<(const T& other);

    template <typename T>
    Message& operator>>(T& other);

    template <typename T>
    Message& operator<<(const std::vector<T>& other);

    template <typename T>
    Message& operator>>(std::vector<T>& other);

    Message& operator>>(const std::string& other);
    Message& operator<<(std::string& other);

private:
    Type _type;
};

#endif // !_MESSAGE_HPP

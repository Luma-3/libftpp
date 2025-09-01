#ifndef _OBJECT_TPP
#define _OBJECT_TPP

#include "pool.hpp"

template <typename TType>
Pool<TType>::Object::Object(Pool* owner, TType* ptr, size_t index)
    : _ptr(ptr), _owner(owner), _index(index){};

template <typename TType>
Pool<TType>::Object::~Object()
{
    if (!_ptr)
        return;
    _ptr->~TType();
    _owner->releaseSlot(_ptr, _index);
    _ptr = nullptr;
}

template <typename TType>
Pool<TType>::Object::Object(Object&& other) noexcept : _ptr(other._ptr), _owner(other._owner)
{
    other._ptr   = nullptr;
    other._owner = nullptr;
}

template <typename TType>
TType* Pool<TType>::Object::operator->()
{
    return _ptr;
}

template <typename TType>
TType& Pool<TType>::Object::operator*()
{
    return *_ptr;
}

template <typename TType>
typename Pool<TType>::Object& Pool<TType>::Object::operator=(Object&& other) noexcept
{
    if (this != &other)
    {
        _ptr         = other._ptr;
        _owner       = other._owner;
        other._ptr   = nullptr;
        other._owner = nullptr;
    }
    return *this;
}

#endif // !_OBJECT_TPP

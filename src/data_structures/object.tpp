#ifndef _OBJECT_TPP
#define _OBJECT_TPP

#include "pool.hpp"

template <typename TType>
Pool<TType>::Object::Object(Pool* owner, TType* ptr, size_t index) : _owner(owner), _index(index){};

template <typename TType>
Pool<TType>::Object::~Object()
{
    void*  ptr = &_owner->_raw[_index];
    TType* obj = reinterpret_cast<TType*>(ptr);
    obj->~TType();
    _owner->releaseSlot(obj, _index);
    _index = 0;
}

template <typename TType>
Pool<TType>::Object::Object(Object&& other) noexcept : _owner(other._owner), _index(other._index)
{
    other._index = 0;
    other._owner = nullptr;
}

template <typename TType>
TType* Pool<TType>::Object::operator->()
{
    void* ptr = &_owner->_raw[_index];
    return reinterpret_cast<TType*>(ptr);
}

template <typename TType>
TType& Pool<TType>::Object::operator*()
{
    void* ptr = &_owner->_raw[_index];
    return *reinterpret_cast<TType*>(ptr);
}

template <typename TType>
typename Pool<TType>::Object& Pool<TType>::Object::operator=(Object&& other) noexcept
{
    if (this != &other)
    {
        _owner       = other._owner;
        _index       = other._index;
        other._owner = nullptr;
        other._index = 0;
    }
    return *this;
}

#endif // !_OBJECT_TPP

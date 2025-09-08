#ifndef POOL_TPP
#define POOL_TPP

#include <cassert>
#include <stdexcept>
#include <utility>

#include "pool.hpp"

template <typename TType>
void Pool<TType>::releaseSlot(size_t index)
{
    if (index >= _capacity)
        return;
    _useSlot[index] = false;
    _freeSlot.push(index);
}

template <typename TType>
Pool<TType>::Pool(const size_t& numberOfObjectStored) : _capacity(numberOfObjectStored)
{
    _raw = std::make_unique<typename std::aligned_storage<sizeof(TType), alignof(TType)>::type[]>(
        numberOfObjectStored);

    _useSlot.resize(numberOfObjectStored, false);
    for (size_t i = 0; i < numberOfObjectStored; ++i)
    {
        _freeSlot.push(numberOfObjectStored - 1 - i);
    }
}

/**
 * @breif Resize the pool to hold more or less objects.
 * @Note: Resizing to a smaller size will destroy objects that do not fit in the new size.
 * @warning: Resizing to a smaller size will invalidate all existing Object instances.
 * Use a Object outside of the pool after a resize will lead to undefined behavior.
 * @param %numberOfObjectStored New size of the pool.
 */

template <typename TType>
void Pool<TType>::resize(const size_t& numberOfObjectStored)
{

    auto newRaw =
        std::make_unique<typename std::aligned_storage<sizeof(TType), alignof(TType)>::type[]>(
            numberOfObjectStored);

    for (size_t i = 0; i < _capacity; ++i)
    {
        if (_useSlot[i] == true && i < numberOfObjectStored)
        {
            void*  oldSlot = &_raw[i];
            TType* oldObj  = reinterpret_cast<TType*>(oldSlot);

            void* newSlot = &newRaw[i];
            new (newSlot) TType(std::move(*oldObj));

            oldObj->~TType();
        }
    }
    _useSlot.resize(numberOfObjectStored, false);
    while (!_freeSlot.empty())
        _freeSlot.pop();
    for (size_t i = 0; i < numberOfObjectStored; ++i)
    {
        int slot = numberOfObjectStored - 1 - i;
        if (_useSlot[slot] == false)
        {
            _freeSlot.push(slot);
        }
    }
    _raw      = std::move(newRaw);
    _capacity = numberOfObjectStored;
}

template <typename TType>
template <typename... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... p_args)
{
    if (_freeSlot.empty())
        throw std::runtime_error("Pool exhausted");
    int slotIndex = _freeSlot.top();

    void*  slot = &_raw[slotIndex];
    TType* obj  = new (slot) TType(std::forward<TArgs>(p_args)...);

    _useSlot[slotIndex] = true;
    _freeSlot.pop();

    return Object(this, obj, slotIndex);
}

#endif // POOL_TPP

#ifndef _POOL_HPP
#define _POOL_HPP

#include <cstddef>
#include <memory>
#include <stack>
#include <type_traits>
#include <vector>

template <typename TType>
class Pool
{
private:
    std::unique_ptr<typename std::aligned_storage<sizeof(TType), alignof(TType)>::type[]> _raw;

    size_t             _capacity = 0;
    std::stack<size_t> _freeSlot;
    std::vector<char>  _useSlot;

    void releaseSlot(size_t index);

public:
    // Start Object
    class Object
    {
    public:
        Object(Pool* owner, TType* ptr, size_t index);
        Object(const Object&) = delete;
        Object(Object&& other) noexcept;
        ~Object();

        TType* operator->();
        TType& operator*();

        Object& operator=(const Object&) = delete;
        Object& operator=(Object&& other) noexcept;

    private:
        Pool*  _owner;
        size_t _index;
    };
    // End Object

    Pool(const size_t& numberOfObjectStored);

    template <typename... TArgs>
    Object acquire(TArgs&&... p_args);

    void resize(const size_t& numberOfObjectStored);
};

#include "object.tpp"
#include "pool.tpp"
#endif // _POOL_HPP

#include <gtest/gtest.h>

#include "pool.hpp"

TEST(PoolTest, BasicAssertion)
{
    Pool<int> pool(5);

    auto obj1 = pool.acquire(10);
    EXPECT_EQ(*obj1, 10);

    *obj1 = 15;
    EXPECT_EQ(*obj1, 15);

    auto obj2 = pool.acquire(20);
    EXPECT_EQ(*obj2, 20);
}

TEST(PoolTest, Exhaustion)
{
    Pool<int> pool(2);

    auto obj1 = pool.acquire(10);
    auto obj2 = pool.acquire(20);

    EXPECT_THROW(pool.acquire(30), std::runtime_error);
}

TEST(PoolTest, ReuseSlot)
{
    Pool<int> pool(1);

    {
        auto obj1 = pool.acquire(10);
        EXPECT_EQ(*obj1, 10);
    } // obj1 goes out of scope and should be released

    auto obj2 = pool.acquire(20);
    EXPECT_EQ(*obj2, 20); // Should succeed since obj1 was released
}

TEST(PoolTest, ResizeLarger)
{
    Pool<int> pool(2);

    auto obj1 = pool.acquire(10);
    auto obj2 = pool.acquire(20);

    EXPECT_THROW(pool.acquire(30), std::runtime_error);

    pool.resize(4); // Resize to larger capacity

    auto obj3 = pool.acquire(30);
    auto obj4 = pool.acquire(40);

    EXPECT_EQ(*obj3, 30);
    EXPECT_EQ(*obj4, 40);
}

TEST(PoolTest, ResizeSmallerThrows)
{
    Pool<int> pool(3);

    auto obj1 = pool.acquire(10);
    auto obj2 = pool.acquire(20);

    EXPECT_THROW(pool.resize(2), std::bad_array_new_length);
}

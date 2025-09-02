#include <gtest/gtest.h>

#include <string>

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

    auto obj3 = pool.acquire(30);
    (*obj3)++;
    EXPECT_EQ(*obj3, 31);
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

TEST(PoolTest, ResizeSmaller)
{
    Pool<int> pool(3);

    auto obj1 = pool.acquire(10);
    {
        auto obj2 = pool.acquire(20);
        auto obj3 = pool.acquire(30);

        pool.resize(2); // Resize to smaller capacity, obj3 should be destroyed

        *obj3 = 35; // This is undefined behavior
        EXPECT_EQ(*obj3, 35);

        EXPECT_THROW(pool.acquire(40), std::runtime_error); // Pool is full
    }

    {
        auto obj4 = pool.acquire(40);
        EXPECT_EQ(*obj4, 40);
    }

    *obj1 = 50;
    EXPECT_EQ(*obj1, 50);

    // Now we can acquire again
    auto obj5 = pool.acquire(50);
    EXPECT_EQ(*obj5, 50);
}

TEST(PoolTest, MoveSemantics)
{
    Pool<int> pool(2);

    auto obj1 = pool.acquire(10);
    EXPECT_EQ(*obj1, 10);

    auto obj2 = std::move(obj1); // Move obj1 to obj2
    EXPECT_EQ(*obj2, 10);
    EXPECT_EQ(obj1.operator->(), nullptr); // obj1 should be null now

    *obj2 = 20;
    EXPECT_EQ(*obj2, 20);
}

TEST(PoolTest, ComplexType)
{
    struct TestStruct
    {
        int         a;
        float       b;
        std::string c;
        TestStruct(int x, float y, std::string z) : a(x), b(y), c(z) {}
    };

    Pool<TestStruct> pool(2);

    auto obj1 = pool.acquire(10, 20.5f, std::string("Hello"));
    EXPECT_EQ(obj1->a, 10);
    EXPECT_EQ(obj1->b, 20.5f);
    EXPECT_EQ(obj1->c, "Hello");

    auto obj2 = pool.acquire(30, 40.5f, "World");
    EXPECT_EQ(obj2->a, 30);
    EXPECT_EQ(obj2->b, 40.5f);
    EXPECT_EQ(obj2->c, "World");
}

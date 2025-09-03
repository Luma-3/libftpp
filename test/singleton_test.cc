#include <gtest/gtest.h>

#include "singleton.hpp"

class TestSingletonClass
{
public:
    TestSingletonClass(int p_value) : _value(p_value) {}
    int value() const
    {
        return _value;
    }

private:
    int _value;
};

TEST(SingletonTest, BasicFunctionality)
{
    Singleton<TestSingletonClass> singleton;

    // Initially, instance should be nullptr
    EXPECT_EQ(singleton.instance(), nullptr);

    // Instantiate the singleton with a value
    singleton.instanciate(42);
    EXPECT_NE(singleton.instance(), nullptr);
    EXPECT_EQ(singleton.instance()->value(), 42);

    // Attempting to instantiate again should throw an error
    EXPECT_THROW(singleton.instanciate(100), std::runtime_error);
}

TEST(SingletonTest, MultipleInstances)
{
    Singleton<TestSingletonClass> singleton1;
    Singleton<TestSingletonClass> singleton2;

    // Instantiate the first singleton
    singleton1.instanciate(10);
    EXPECT_EQ(singleton1.instance()->value(), 10);

    EXPECT_THROW(singleton1.instanciate(20), std::runtime_error);
}

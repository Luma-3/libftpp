#include <gtest/gtest.h>

#include "thread.hpp"
#include "thread_safe_iostream.hpp"

TEST(ThreadTest, BasicFunctionality)
{

    Thread thread("[TestThread] ", []() { ThreadSafeIO << "Thread is running." << std::endl; });

    ::testing::internal::CaptureStdout();
    thread.start();
    thread.stop();
    std::string output = ::testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "[TestThread] Thread is running.\n");

    SUCCEED();
}

TEST(ThreadTest, MultipleThreads)
{
    Thread thread1("[Thread1] ", []() { ThreadSafeIO << "Thread 1 is running." << std::endl; });
    Thread thread2("[Thread2] ", []() { ThreadSafeIO << "Thread 2 is running." << std::endl; });

    ::testing::internal::CaptureStdout();
    thread1.start();
    thread2.start();
    thread1.stop();
    thread2.stop();
    std::string output = ::testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "[Thread1] Thread 1 is running.\n[Thread2] Thread 2 is running.\n");

    SUCCEED();
}

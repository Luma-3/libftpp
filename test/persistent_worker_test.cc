#include <gtest/gtest.h>

#include "persistent_worker.hpp"

TEST(PersistentWorkerTest, AddAndRemoveTask)
{
    PersistentWorker worker(std::chrono::milliseconds(10));

    bool taskExecuted = false;
    worker.addTask("testTask", [&taskExecuted]() { taskExecuted = true; });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_TRUE(taskExecuted);

    worker.removeTask("testTask");
    taskExecuted = false;

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_FALSE(taskExecuted);
}

TEST(PersistentWorkerTest, ExceptionHandling)
{
    PersistentWorker worker(std::chrono::milliseconds(10));

    worker.addTask("exceptionTask", []() { throw std::runtime_error("Test exception"); });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto exceptions = worker.getExceptions();
    EXPECT_FALSE(exceptions.empty());
    try
    {
        if (!exceptions.empty())
            std::rethrow_exception(exceptions[0]);
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_STREQ(e.what(), "Test exception");
    }
    catch (...)
    {
        FAIL() << "Caught unexpected exception type";
    }
}

TEST(PersistentWorkerTest, MultipleTasks)
{
    PersistentWorker worker(std::chrono::milliseconds(10));

    int counter1 = 0;
    int counter2 = 0;

    worker.addTask("task1", [&counter1]() { counter1++; });
    worker.addTask("task2", [&counter2]() { counter2++; });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_GT(counter1, 0);
    EXPECT_GT(counter2, 0);

    worker.removeTask("task1");
    int oldCounter1 = counter1;
    int oldCounter2 = counter2;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(counter1, oldCounter1); // task1 should not increment anymore
    EXPECT_GT(counter2, oldCounter2); // task2 should still increment
}

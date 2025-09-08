
#include <gtest/gtest.h>

#include "worker_pool.hpp"

class TestJob : public WorkerPool::IJob
{
public:
    TestJob(int id, bool shouldThrow = false) : id(id), shouldThrow(shouldThrow) {}
    void execute() override
    {
        if (shouldThrow)
            throw std::runtime_error("Job " + std::to_string(id) + " failed");
        // Simulate work
    }
    int  id;
    bool shouldThrow;
};

TEST(WorkerPoolTest, BasicFunctionality)
{
    WorkerPool            pool(4);
    std::vector<TestJob*> jobs;
    for (int i = 0; i < 10; ++i)
    {
        jobs.push_back(new TestJob(i));
        pool.addJob(jobs.back());
    }

    // Allow some time for jobs to complete
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto exceptions = pool.getExceptions();
    EXPECT_TRUE(exceptions.empty());

    for (auto job : jobs)
        delete job;
}

TEST(WorkerPoolTest, ExceptionHandling)
{
    WorkerPool            pool(4);
    std::vector<TestJob*> jobs;
    for (int i = 0; i < 10; ++i)
    {
        bool shouldThrow = (i % 3 == 0); // Every third job throws an exception
        jobs.push_back(new TestJob(i, shouldThrow));
        pool.addJob(jobs.back());
    }

    // Allow some time for jobs to complete
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto exceptions = pool.getExceptions();
    EXPECT_EQ(exceptions.size(), 4); // Expecting 4 exceptions (0, 3, 6, 9)

    for (auto job : jobs)
        delete job;
}

TEST(WorkerPoolTest, NoJobs)
{
    WorkerPool pool(4);

    // Allow some time to ensure no jobs are processed
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto exceptions = pool.getExceptions();
    EXPECT_TRUE(exceptions.empty());
}

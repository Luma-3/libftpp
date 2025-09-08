#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "thread_safe_queue.hpp"

using namespace std::chrono_literals;

TEST(ThreadSafeQueueTest, BasicOperations)
{
    ThreadSafeQueue<int> queue;

    queue.push_back(1);
    queue.push_back(2);
    queue.push_back(3);

    EXPECT_EQ(queue.pop_back(), 3);
    EXPECT_EQ(queue.pop_back(), 2);
    EXPECT_EQ(queue.pop_back(), 1);
}

TEST(ThreadSafeQueue_ThreadSafety_SimpleProducerConsumer, MultipleProducersSingleConsumer)
{
    ThreadSafeQueue<int> q;

    const int producers_count    = 4;
    const int items_per_producer = 1000;
    const int total_items        = producers_count * items_per_producer;

    // We'll push distinct ranges so we can verify the sum at the end
    std::vector<std::thread> producers;
    producers.reserve(producers_count);

    long long expected_sum = 0;
    for (int p = 0; p < producers_count; ++p)
    {
        int start = p * items_per_producer;
        int end   = start + items_per_producer;
        // sum of arithmetic sequence
        long long sum_range = (long long)(start + end - 1) * items_per_producer / 2;
        expected_sum += sum_range;

        producers.emplace_back(
            [start, end, &q]()
            {
                for (int v = start; v < end; ++v)
                {
                    q.push_back(v);
                }
            });
    }

    std::atomic<int>       popped_count{0};
    std::atomic<long long> popped_sum{0};

    // Consumer thread: keep trying to pop until it collected total_items.
    std::thread consumer(
        [&]()
        {
            while (popped_count.load() < total_items)
            {
                try
                {
                    int v = q.pop_front();
                    popped_sum.fetch_add(v);
                    popped_count.fetch_add(1);
                }
                catch (...)
                {
                    // queue might be temporarily empty -> yield and retry
                    std::this_thread::sleep_for(1ms);
                }
            }
        });

    for (auto& t : producers)
        t.join();
    // Wait for consumer to finish with a timeout guard to avoid infinite hang in test
    const auto max_wait   = 5s;
    const auto start_wait = std::chrono::steady_clock::now();
    while (popped_count.load() < total_items)
    {
        if (std::chrono::steady_clock::now() - start_wait > max_wait)
            break;
        std::this_thread::sleep_for(1ms);
    }

    // If consumer still running, join (it should exit if it popped all)
    if (consumer.joinable())
        consumer.join();

    EXPECT_EQ(popped_count.load(), total_items);
    EXPECT_EQ(popped_sum.load(), expected_sum);
}

TEST(ThreadSafeQueueTest, PushFrontPopFront)
{
    ThreadSafeQueue<int> queue;

    queue.push_front(1);
    queue.push_front(2);
    queue.push_front(3);

    EXPECT_EQ(queue.pop_front(), 3);
    EXPECT_EQ(queue.pop_front(), 2);
    EXPECT_EQ(queue.pop_front(), 1);
}

TEST(ThreadSafeQueueTest, MixedOperations)
{
    ThreadSafeQueue<int> queue;

    queue.push_back(1);
    queue.push_front(2);
    queue.push_back(3);
    queue.push_front(4);

    EXPECT_EQ(queue.pop_front(), 4);
    EXPECT_EQ(queue.pop_back(), 3);
}

TEST(ThreadSafeQueueTest, ConcurrentMixedOperations)
{
    ThreadSafeQueue<int> queue;
    const int            numElements = 1000;

    auto producer = [&queue, numElements]()
    {
        for (int i = 0; i < numElements; ++i)
        {
            if (i % 2 == 0)
            {
                queue.push_back(i);
            }
            else
            {
                queue.push_front(i);
            }
        }
    };

    auto consumer = [&queue, numElements]()
    {
        for (int i = 0; i < numElements; ++i)
        {
            if (i % 2 == 0)
            {
                queue.pop_back();
            }
            else
            {
                queue.pop_front();
            }
        }
    };

    std::thread prodThread(producer);
    std::thread consThread(consumer);

    prodThread.join();
    consThread.join();

    // The queue should be empty after all operations
}

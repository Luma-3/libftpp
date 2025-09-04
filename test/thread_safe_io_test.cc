#include <gtest/gtest.h>

#include <algorithm>
#include <regex>
#include <thread>

#include "thread_safe_iostream.hpp"

TEST(ThreadSafeIOStreamTest, BasicOutput)
{
    testing::internal::CaptureStdout();
    ThreadSafeIO.setPrefix("[Test] ");
    ThreadSafeIO << "Hello, " << "world!" << std::endl;
    ThreadSafeIO.flush();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "[Test] Hello, world!\n");
}

TEST(ThreadSafeIOStreamTest, MultipleThreads)
{
    const int num_threads         = 5;
    const int messages_per_thread = 10;

    testing::internal::CaptureStdout();
    auto thread_func = [](int thread_id)
    {
        for (int i = 0; i < messages_per_thread; ++i)
        {
            ThreadSafeIO << "Thread " << thread_id << " message " << i << std::endl;
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(thread_func, i);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    std::string output = testing::internal::GetCapturedStdout();

    int line_count = std::count(output.begin(), output.end(), '\n');
    EXPECT_EQ(line_count, num_threads * messages_per_thread);
}

TEST(ThreadSafeIOStreamTest, PrefixChange)
{
    testing::internal::CaptureStdout();
    ThreadSafeIO.setPrefix("[First] ");
    ThreadSafeIO << "Message one" << std::endl;
    ThreadSafeIO.setPrefix("[Second] ");
    ThreadSafeIO << "Message two" << std::endl;
    ThreadSafeIO.flush();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "[First] Message one\n[Second] Message two\n");
}

TEST(ThreadSafeIOStreamTest, NoPrefix)
{
    testing::internal::CaptureStdout();
    ThreadSafeIO.setPrefix("");
    ThreadSafeIO << "No prefix message" << std::endl;
    ThreadSafeIO.flush();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "No prefix message\n");
}

TEST(ThreadSafeIOStreamTest, MixedContent)
{
    testing::internal::CaptureStdout();
    ThreadSafeIO.setPrefix("[Mixed] ");
    ThreadSafeIO << "Integer: " << 42 << ", Float: " << 3.14 << ", String: " << "test" << std::endl;
    ThreadSafeIO.flush();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "[Mixed] Integer: 42, Float: 3.14, String: test\n");
}

TEST(ThreadSafeIOStreamTest, LargeOutput1Thread)
{
    const int num_lines = 1000;

    testing::internal::CaptureStdout();
    for (int i = 0; i < num_lines; ++i)
    {
        ThreadSafeIO << "Line " << i << std::endl;
    }
    ThreadSafeIO.flush();
    std::string output = testing::internal::GetCapturedStdout();

    int line_count = std::count(output.begin(), output.end(), '\n');
    EXPECT_EQ(line_count, num_lines);
}

TEST(ThreadSafeIOStreamTest, LargeOutputMultipleThreadsOptFlush)
{
    const int num_threads          = 10;
    const int num_lines_per_thread = 200;

    testing::internal::CaptureStdout();
    auto thread_func = [](int thread_id)
    {
        for (int i = 0; i < num_lines_per_thread; ++i)
        {
            ThreadSafeIO << "Thread " << thread_id << " Line " << i << '\n';
        }

        ThreadSafeIO.flush();
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(thread_func, i);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    std::string output = testing::internal::GetCapturedStdout();

    int line_count = std::count(output.begin(), output.end(), '\n');
    EXPECT_EQ(line_count, num_threads * num_lines_per_thread);
}

TEST(ThreadSafeIOStreamTest, FlushBehavior)
{
    testing::internal::CaptureStdout();
    ThreadSafeIO << "Before flush" << '\n';
    ThreadSafeIO.flush();
    std::string output1 = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output1, "Before flush\n");

    testing::internal::CaptureStdout();
    ThreadSafeIO << "After flush" << '\n';
    ThreadSafeIO.flush();
    std::string output2 = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output2, "After flush\n");
}

TEST(ThreadSafeIOStreamTest, MultithreadTestRegex)
{
    const int         num_threads         = 5;
    const int         messages_per_thread = 10;
    const std::string prefix              = "[ThreadSafe] ";

    // Capture stdout avant de lancer les threads
    testing::internal::CaptureStdout();

    auto thread_func = [&](int thread_id)
    {
        ThreadSafeIO.setPrefix(prefix);
        for (int i = 0; i < messages_per_thread; ++i)
        {
            ThreadSafeIO << "Thread " << thread_id << " message " << i << std::endl;
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(thread_func, i);
    }
    for (auto& t : threads)
    {
        t.join();
    }

    ThreadSafeIO.flush();

    std::string output = testing::internal::GetCapturedStdout();

    // Vérifie le nombre total de lignes
    int line_count = std::count(output.begin(), output.end(), '\n');
    EXPECT_EQ(line_count, num_threads * messages_per_thread);

    // Vérifie que chaque ligne commence par le préfixe et correspond au motif attendu
    std::regex         line_pattern("\\[ThreadSafe\\] Thread [0-9]+ message [0-9]+");
    std::istringstream iss(output);
    std::string        line;
    while (std::getline(iss, line))
    {
        EXPECT_TRUE(std::regex_match(line, line_pattern));
    }
}

#include <gtest/gtest.h>

#include <algorithm>
#include <regex>
#include <thread>

#include "thread_safe_iostream.hpp"

struct CoutCapture
{
    std::ostringstream oss;
    std::streambuf*    old = nullptr;
    CoutCapture()
    {
        old = std::cout.rdbuf(oss.rdbuf());
    }
    ~CoutCapture()
    {
        std::cout.rdbuf(old);
    }
    std::string str() const
    {
        return oss.str();
    }
};

TEST(ThreadSafeIOStreamTest, BasicOutput)
{
    CoutCapture cap;
    ThreadSafeIO.setPrefix("[Test] ");
    ThreadSafeIO << "Hello, " << "world!" << std::endl;
    EXPECT_EQ(cap.str(), "[Test] Hello, world!\n");
}

TEST(ThreadSafeIOStreamTest, MultipleThreads)
{
    const int num_threads         = 5;
    const int messages_per_thread = 10;

    CoutCapture cap;
    auto        thread_func = [](int thread_id)
    {
        for (int i = 0; i < messages_per_thread; ++i)
        {
            ThreadSafeIO << "Thread " << thread_id << " message " << i << '\n';
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(std::thread(thread_func, i));
    }

    for (auto& t : threads)
    {
        t.join();
    }

    std::regex         line_pattern("Thread [0-9]+ message [0-9]+");
    std::istringstream iss(cap.str());
    std::string        line;
    int                line_count = 0;
    while (std::getline(iss, line))
    {
        EXPECT_TRUE(std::regex_match(line, line_pattern));
        ++line_count;
    }
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
    ThreadSafeIO.setPrefix("");
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

// TEST(ThreadSafeIOStreamTest, PromptTest)
// {
//     // Prépare l'entrée simulée
//     std::istringstream simulated_input("42\n3.14\ntest_string\n");
//     ThreadSafeIOStream test_io(std::cout, simulated_input);
//     test_io.setPrefix("[PromptTest] ");
//
//     int         int_value;
//     double      double_value;
//     std::string string_value;
//
//     // Capture stdout avant d'appeler prompt
//     testing::internal::CaptureStdout();
//
//     test_io.prompt("Enter an integer: ", int_value);
//     test_io.prompt("Enter a double: ", double_value);
//     test_io.prompt("Enter a string: ", string_value);
//
//     std::string output = testing::internal::GetCapturedStdout();
//
//     // Vérifie que les valeurs ont été correctement lues
//     EXPECT_EQ(int_value, 42);
//     EXPECT_DOUBLE_EQ(double_value, 3.14);
//     EXPECT_EQ(string_value, "test_string");
//
//     // Vérifie que les prompts ont été affichés avec le préfixe correct
//     std::string expected_output =
//         "[PromptTest] Enter an integer: [PromptTest] Enter a double: [PromptTest] Enter a string:
//         ";
//     EXPECT_EQ(output, expected_output);
// }

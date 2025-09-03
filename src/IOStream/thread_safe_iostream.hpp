#ifndef _THREAD_SAFE_IOSTREAM_HPP
#define _THREAD_SAFE_IOSTREAM_HPP

#include <iostream>
#include <mutex>
#include <streambuf>
#include <string>
#include <thread>

class ThreadSafeStreambuf : public std::streambuf
{
};
#endif // !_THREAD_SAFE_IOSTREAM_HPP

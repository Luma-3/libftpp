#ifndef _THREAD_HPP
#define _THREAD_HPP

#include <functional>
#include <string>
#include <thread>

#include "thread_safe_iostream.hpp"

class Thread
{
private:
    std::string           _name;
    std::thread           _thread;
    std::function<void()> _func;

public:
    Thread(const std::string& name, std::function<void()> func);
    void start();
    void stop();
};

#endif // !_THREAD_HPP

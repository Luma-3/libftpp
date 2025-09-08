#include "thread.hpp"

#include <functional>
#include <string>
#include <thread>

#include "thread_safe_iostream.hpp"

Thread::Thread(const std::string& name, std::function<void()> func)
    : _name(name), _func(std::move(func))
{
}

void Thread::start()
{
    _thread = std::thread(
        [this]
        {
            ThreadSafeIO.setPrefix(_name);
            _func();
        });
}

void Thread::stop()
{
    if (_thread.joinable())
    {
        _thread.join();
    }
}

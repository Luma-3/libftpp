#include "thread_safe_iostream.hpp"

#include <mutex>
#include <ostream>
#include <sstream>

std::mutex ThreadSafeIOStream::_global_io_mutex;

void ThreadSafeIOStream::setPrefix(const std::string& prefix)
{
    _prefix = prefix;
}

ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream& (*manip)(std::ostream&))
{

    if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
    {
        _buffer << '\n';
        flush();
    }
    else
    {
        manip(_buffer);
    }
    return *this;
}

void ThreadSafeIOStream::flush()
{
    auto temp = _buffer.str();
    _buffer.str("");
    _buffer.clear();
    _atEndLine = true;

    if (temp.empty())
    {
        return;
    }

    std::lock_guard<std::mutex> lock(_global_io_mutex);
    std::cout << temp;
    std::cout.flush();
}

thread_local ThreadSafeIOStream ThreadSafeIO;

#include "thread_safe_iostream.hpp"

#include <istream>
#include <mutex>
#include <ostream>
#include <sstream>

std::mutex                      ThreadSafeIOStream::_global_io_mutex;
thread_local std::ostringstream ThreadSafeIOStream::_buffer;
thread_local std::string        ThreadSafeIOStream::_prefix;

ThreadSafeIOStream::ThreadSafeIOStream(std::ostream& out, std::istream& in) : _out(out), _in(in) {};

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
        _buffer << manip;
    }
    return *this;
}

void ThreadSafeIOStream::flush()
{
    auto temp = _buffer.str();
    _buffer.str("");
    _buffer.clear();

    if (temp.empty())
        return;

    std::lock_guard<std::mutex> lock(_global_io_mutex);
    _out << _prefix << temp;
    _out.flush();
}

thread_local ThreadSafeIOStream ThreadSafeIO(std::cout, std::cin);

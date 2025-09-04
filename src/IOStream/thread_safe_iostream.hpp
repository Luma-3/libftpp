#ifndef _THREAD_SAFE_IOSTREAM_HPP
#define _THREAD_SAFE_IOSTREAM_HPP

#include <iostream>
#include <istream>
#include <mutex>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <string>

class ThreadSafeIOStream : public std::streambuf
{
private:
    thread_local static std::string        _prefix;
    thread_local static std::ostringstream _buffer;
    bool                                   _atEndLine = true;

    static std::mutex _global_io_mutex;

    std::ostream& _out;
    std::istream& _in;

public:
    ThreadSafeIOStream(std::ostream& out, std::istream& in);

    template <typename T>
    ThreadSafeIOStream& operator<<(const T& value)
    {
        std::ostringstream ss;
        return *this;
    }

    ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&));

    template <typename T>
    ThreadSafeIOStream& operator>>(T& value)
    {
        std::lock_guard<std::mutex> lock(_global_io_mutex);
        _in >> value;
        return *this;
    }

    void setPrefix(const std::string& prefix);

    template <typename T>
    void prompt(const std::string& question, T& dest)
    {
        {
            std::lock_guard<std::mutex> lock(_global_io_mutex);
            _out << _prefix << question;
            _out.flush();
        }
        std::lock_guard<std::mutex> lock(_global_io_mutex);
        _in >> dest;
    }

    void flush();
};

extern thread_local ThreadSafeIOStream ThreadSafeIO;

#endif // !_THREAD_SAFE_IOSTREAM_HPP

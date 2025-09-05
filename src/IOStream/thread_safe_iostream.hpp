#ifndef _THREAD_SAFE_IOSTREAM_HPP
#define _THREAD_SAFE_IOSTREAM_HPP

#include <cstddef>
#include <iostream>
#include <mutex>
#include <ostream>
#include <sstream>
#include <string>

class ThreadSafeIOStream
{
private:
    std::string        _prefix = "";
    std::ostringstream _buffer;
    bool               _atEndLine = true;

    static std::mutex _global_io_mutex;

public:
    template <typename T>
    ThreadSafeIOStream& operator<<(const T& value)
    {
        std::ostringstream ss;
        ss << value;
        std::string tmp = ss.str();

        size_t start = 0;
        while (start < tmp.size())
        {
            if (_atEndLine)
            {
                _buffer << _prefix;
                _atEndLine = false;
            }

            size_t pos = tmp.find('\n', start);
            if (pos == std::string::npos)
            {
                _buffer << std::string(tmp.begin() + start, tmp.end());
                break;
            }
            std::string line(tmp.begin() + start, tmp.begin() + pos + 1);
            _buffer << line;
            _atEndLine = true;
            start      = pos + 1;
        }

        return *this;
    }

    ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&));

    template <typename T>
    ThreadSafeIOStream& operator>>(T& value)
    {
        std::lock_guard<std::mutex> lock(_global_io_mutex);
        std::cin >> value;
        return *this;
    }

    void setPrefix(const std::string& prefix);

    template <typename T>
    void prompt(const std::string& question, T& dest)
    {
        {
            std::lock_guard<std::mutex> lock(_global_io_mutex);
            std::cout << _prefix << question;
            std::cout.flush();
        }
        std::lock_guard<std::mutex> lock(_global_io_mutex);
        std::cin >> dest;
    }

    void flush();
};

extern thread_local ThreadSafeIOStream ThreadSafeIO;

#endif // !_THREAD_SAFE_IOSTREAM_HPP

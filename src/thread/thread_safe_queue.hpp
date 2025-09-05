#ifndef _THREAD_SAFE_QUEUE_HPP_
#define _THREAD_SAFE_QUEUE_HPP_

#include <mutex>
#include <queue>

template <typename TType>
class ThreadSafeQueue
{
private:
    std::deque<TType> _queue;
    std::mutex        _mutex;

public:
    void push_back(const TType& newElement)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push_back(newElement);
    }
    void push_front(const TType& newElement)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push_front(newElement);
    }

    TType pop_back()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        TType                       elem = _queue.back();
        _queue.pop_back();
        return elem;
    }

    TType pop_front()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        TType                       elem = _queue.front();
        _queue.pop_front();
        return elem;
    }
};

#endif // !_THREAD_SAFE_QUEUE_HPP_

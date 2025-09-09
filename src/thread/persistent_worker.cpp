#include "persistent_worker.hpp"

#include <chrono>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

PersistentWorker::PersistentWorker(std::chrono::milliseconds tick) : _stop(false), _tick(tick)
{
    _worker = std::thread(&PersistentWorker::runtime, this);
}

PersistentWorker::~PersistentWorker()
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _stop = true;
    }
    _cv.notify_all();

    if (_worker.joinable())
    {
        _worker.join();
    }
}

void PersistentWorker::addTask(const std::string& name, const std::function<void()>& func)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _jobs[name] = std::make_shared<std::function<void()>>(func);
    _cv.notify_one();
}

void PersistentWorker::removeTask(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _jobs.erase(name);
}

void PersistentWorker::runtime()
{
    while (true)
    {
        std::vector<std::shared_ptr<std::function<void()>>> copy;
        {
            std::unique_lock<std::mutex> lock(_mtx);
            _cv.wait_for(lock, _tick, [this] { return !_jobs.empty() || _stop; });

            if (_stop)
                return;

            for (auto& pair : _jobs)
            {
                copy.push_back(pair.second);
            }
        }
        for (auto& func : copy)
        {
            try
            {
                (*func)();
            }
            catch (...)
            {
                std::lock_guard<std::mutex> lock(_mtxExceptions);
                _exceptions.push(std::current_exception());
            }
        }
    }
}

std::vector<std::exception_ptr> PersistentWorker::getExceptions()
{
    std::vector<std::exception_ptr> result;
    std::lock_guard<std::mutex>     lock(_mtxExceptions);
    while (!_exceptions.empty())
    {
        result.push_back(_exceptions.front());
        _exceptions.pop();
    }
    return result;
}

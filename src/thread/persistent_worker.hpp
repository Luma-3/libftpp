#ifndef _PERSISTENT_WORKER_HPP
#define _PERSISTENT_WORKER_HPP

#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

class PersistentWorker
{
private:
    std::thread                                                             _worker;
    std::unordered_map<std::string, std::shared_ptr<std::function<void()>>> _jobs;
    std::queue<std::exception_ptr>                                          _exceptions;

    std::mutex              _mtx;
    std::mutex              _mtxExceptions;
    std::condition_variable _cv;
    bool                    _stop;

    std::chrono::milliseconds _tick;

    void runtime();

public:
    PersistentWorker(std::chrono::milliseconds tick = std::chrono::milliseconds(1));
    ~PersistentWorker();

    void addTask(const std::string& name, const std::function<void()>& func);

    /**
     * @brief Remove a task by its name.
     * @param name The name of the task to be removed.
     * @warning If the task is already in copy vector, it will still be executed one last time
     * before being removed.
     */
    void removeTask(const std::string& name);

    std::vector<std::exception_ptr> getExceptions();
};

#endif // !_PERSISTENT_WORKER_HPP

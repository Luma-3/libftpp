#include "worker_pool.hpp"

#include <cstddef>
#include <exception>
#include <mutex>
#include <vector>

WorkerPool::WorkerPool(size_t nb_worker) : _stop(false)
{
    for (size_t i = 0; i < nb_worker; ++i)
    {
        _workers.emplace_back(&WorkerPool::runtime, this);
    }
}

void WorkerPool::runtime()
{
    while (true)
    {
        IJob* job;
        {
            std::unique_lock<std::mutex> lock(_mtx);
            _cv.wait(lock, [this] { return !_jobs.empty() || _stop; });

            if (_stop && _jobs.empty())
                return;

            job = _jobs.front();
            _jobs.pop();
        }

        try
        {
            job->execute();
        }
        catch (...)
        {
            std::lock_guard<std::mutex> lock(_mtxExceptions);
            _jobsExceptions.push(std::current_exception());
        }
    }
}

void WorkerPool::addJob(IJob* job)
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _jobs.push(job);
    }
    _cv.notify_one();
}

std::vector<std::exception_ptr> WorkerPool::getExceptions()
{
    std::lock_guard<std::mutex>     lock(_mtxExceptions);
    std::vector<std::exception_ptr> result;
    while (!_jobsExceptions.empty())
    {
        result.push_back(_jobsExceptions.front());
        _jobsExceptions.pop();
    }
    return result;
}

WorkerPool::~WorkerPool()
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _stop = true;
    }
    _cv.notify_all();

    for (auto& worker : _workers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

WorkerPool::IJob::~IJob() {};

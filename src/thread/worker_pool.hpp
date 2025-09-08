#ifndef _WORKER_POOL_HPP_
#define _WORKER_POOL_HPP_

#include <condition_variable>
#include <cstddef>
#include <exception>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class WorkerPool
{
public:
    class IJob
    {
    public:
        virtual void execute() = 0;
        virtual ~IJob();
    };

    WorkerPool(size_t nb_worker);
    ~WorkerPool();

    void                            addJob(IJob* job);
    std::vector<std::exception_ptr> getExceptions();

private:
    std::queue<IJob*>              _jobs;
    std::queue<std::exception_ptr> _jobsExceptions;
    std::vector<std::thread>       _workers;
    bool                           _stop;
    std::mutex                     _mtx;
    std::mutex                     _mtxExceptions;
    std::condition_variable        _cv;

    void runtime();
};

#endif // !_WORKER_POOL_HPP_

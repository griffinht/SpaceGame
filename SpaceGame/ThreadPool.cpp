#include "ThreadPool.h"

ThreadPool::ThreadPool(int threadsAmt, int queueLimit)
{
    this->queueLimit = queueLimit;
	threads.reserve(threadsAmt);
	for (int i = 0; i < threadsAmt; i++)
	{
		threads.emplace_back(std::bind(&ThreadPool::ThreadEntry, this, i));
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> uniqueLock(mutex);
		shutdown = true;
		condVar.notify_all();
	}
}

void ThreadPool::QueueJob(std::function<void(void)> function)
{
    std::unique_lock<std::mutex> uniqueLock(mutex);
    if (queueLimit > 0)
    {
        while (jobs.size() > queueLimit)
        {
            jobs.pop();
        }
    }
    jobs.emplace(std::move(function));
    condVar.notify_one();
}

void ThreadPool::ThreadEntry(int i)
{
    std::function <void(void)> job;

    while (true)
    {
        {
            std::unique_lock<std::mutex> uniqueLock(mutex);

            while (!shutdown && jobs.empty())
            {
                condVar.wait(uniqueLock);
            }

            if (jobs.empty())
            {
                return;
            }

            job = std::move(jobs.front());
            jobs.pop();
        }

        job();
    }
}

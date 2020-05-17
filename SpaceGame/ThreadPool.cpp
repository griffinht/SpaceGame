#include "ThreadPool.h"

ThreadPool::ThreadPool(int threadsAmt)
{
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

#pragma once

#include <functional>
#include <mutex>
#include <queue>


class ThreadPool
{
public:
	ThreadPool(int threads);
	~ThreadPool();
	void QueueJob(std::function <void(void)> function);
private:
	std::mutex mutex;
	std::condition_variable condVar;
	std::queue <std::function <void(void)>> jobs;
	std::vector <std::thread> threads;
	bool shutdown = false;
protected:
	void ThreadEntry(int i);
};
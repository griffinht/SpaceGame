#pragma once

#include <functional>
#include <mutex>
#include <queue>


class ThreadPool
{
public:
	ThreadPool(int threads, int queueLimit);
	~ThreadPool();
	void QueueJob(std::function <void(void)> function);
private:
	std::mutex mutex;
	std::condition_variable condVar;
	std::queue <std::function <void(void)>> jobs;
	std::vector <std::thread> threads;
	bool shutdown = false;
	int queueLimit;
protected:
	void ThreadEntry(int i);
};
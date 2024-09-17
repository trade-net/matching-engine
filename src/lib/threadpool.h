#ifndef INCLUDED_THREADPOOL
#define INCLUDED_THREADPOOL

#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <memory>

class ThreadPool{
public:
	ThreadPool(size_t numThreads);

	void enqueue(size_t threadInIndex, std::function<void()> task);
	
	~ThreadPool();

private:
	std::vector<std::thread> workers;
	std::vector<std::queue<std::function<void()>>> taskQueues;
	std::vector<std::unique_ptr<std::mutex>> queueMutexes;
	std::vector<std::unique_ptr<std::condition_variable>> conditions;
	bool stop = false;
};

#endif

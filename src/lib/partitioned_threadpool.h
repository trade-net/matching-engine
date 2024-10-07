#ifndef INCLUDED_PARTITIONED_THREADPOOL
#define INCLUDED_PARTITIONED_THREADPOOL

#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <memory>

class PartitionedThreadPool{
public:
	PartitionedThreadPool(size_t numThreads);

	void enqueue(size_t threadInIndex, std::function<void()> task);
	
	~PartitionedThreadPool();

private:
	std::vector<std::thread> workers;
	std::vector<std::queue<std::function<void()>>> taskQueues;
	std::vector<std::unique_ptr<std::mutex>> queueMutexes;
	std::vector<std::unique_ptr<std::condition_variable>> conditions;
	bool stop = false;
};

#endif

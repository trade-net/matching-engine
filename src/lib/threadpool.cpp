#include <threadpool.h>
#include <iostream>

ThreadPool::ThreadPool(size_t numThreads)
{
	taskQueues.resize(numThreads);
	queueMutexes.resize(numThreads);
	conditions.resize(numThreads);
	for(size_t i = 0; i < numThreads; ++i)
	{
		workers.emplace_back([this, i] {
			try{
				while(true)
				{
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(*queueMutexes[i]);

						conditions[i]->wait(lock, [this, i] {
							return !taskQueues[i].empty() || stop;
						});

						if(stop && taskQueues[i].empty())
							return;

						task = std::move(taskQueues[i].front());
						taskQueues[i].pop();
					}

					task();
				}
			} catch (const std::exception &e) {
				std::cerr << "Exception in thread " << i << ": " << e.what() << std::endl;
				throw;
			} catch (...) {
				std::cerr << "Unknown exception in thread " << i << std::endl;
			}
		});

		queueMutexes[i] = std::make_unique<std::mutex>();
		conditions[i] = std::make_unique<std::condition_variable>();

	}
}

void ThreadPool::enqueue(size_t threadIndex, std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(*queueMutexes[threadIndex]);
		taskQueues[threadIndex].emplace(std::move(task));
	}
	conditions[threadIndex]->notify_one();
}

ThreadPool::~ThreadPool()
{
	{
		for(size_t i = 0; i < workers.size(); ++i)
		{
			std::unique_lock<std::mutex> lock(*queueMutexes[i]);
		}
		stop = true;
	}
	for(size_t i = 0; i < workers.size(); ++i){
		conditions[i]->notify_all();
	}

	for(std::thread& worker : workers)
	{
		worker.join();
	}
}

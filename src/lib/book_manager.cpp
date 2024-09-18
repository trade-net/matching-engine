#include <book_manager.h>
#include <memory>

size_t getThreadForSecurity(const std::string& security, size_t poolSize)
{
	std::hash<std::string> hasher;
	return hasher(security) % poolSize;
}

BookManager::BookManager(size_t poolSize)
: threadPool(poolSize)
, threadPoolSize(poolSize)
{}

void BookManager::processOrderRequest(OrderRequest& orderRequest)
{
	std::shared_ptr<Order> order = orderRequest.toOrder();
	std::string security = orderRequest.security;
	
	size_t threadIndex = getThreadForSecurity(security, threadPoolSize);

	threadPool.enqueue(
		threadIndex,
		[this, security, order](){
			OrderBook* book = nullptr;
			{
				std::unique_lock<std::mutex> lock(orderBooksMutex);
				auto it = orderBooks.find(security);
				if(it == orderBooks.end())
				{
					orderBooks.insert(std::pair<std::string, OrderBook>(security, OrderBook()));
					it = orderBooks.find(security);
				}
				book = &it->second;
			}

			OrderStatus status = book->processOrder(order);

			if(!book->isActive())
			{
				std::unique_lock<std::mutex> lock(orderBooksMutex);
				orderBooks.erase(security);
			}
		}
	);
}

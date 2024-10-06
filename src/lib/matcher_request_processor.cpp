#include <matcher_request_processor.h>
#include <memory>

size_t getThreadForSecurity(const std::string& security, size_t poolSize)
{
	std::hash<std::string> hasher;
	return hasher(security) % poolSize;
}

MatcherRequestProcessor::MatcherRequestProcessor(size_t poolSize, size_t RPPoolSize)
: RequestProcessor(RPPoolSize)
, threadPool(poolSize)
, threadPoolSize(poolSize)
{
	registerHandler<OrderRequest, int>("getOrder", [this](const OrderRequest& order){
		return processOrderRequest(order);
	});
}

int MatcherRequestProcessor::processOrderRequest(const OrderRequest& orderRequest)
{
	std::shared_ptr<Order> order = Order::fromOrderRequest(orderRequest);
	const std::string& security = order->security;
	
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

	return 0;
}

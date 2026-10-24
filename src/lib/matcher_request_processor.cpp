#include <matcher_request_processor.h>
#include <memory>

size_t getThreadForSecurity(const std::string& security, size_t poolSize)
{
	std::hash<std::string> hasher;
	return hasher(security) % poolSize;
}

MatcherRequestProcessor::MatcherRequestProcessor(size_t poolSize)
: orderBooks(poolSize)
, threadPool(poolSize)
, threadPoolSize(poolSize)
{
	registerHandler<OrderRequest, int>([this](const OrderRequest& order){
		return processOrderRequest(order);
	});
	registerHandler<OrderUpdateRequest, int>([this](const OrderUpdateRequest& orderUpdate){
		return processOrderUpdateRequest(orderUpdate);
	});
}

int MatcherRequestProcessor::processOrderRequest(const OrderRequest& orderRequest)
{
	Order order = Order::fromOrderRequest(orderRequest);
	const std::string& security = order.security;
	
	size_t threadIndex = getThreadForSecurity(security, threadPoolSize);

	threadPool.enqueue(
		threadIndex,
		[this, threadIndex, security, order](){
			auto& books = orderBooks[threadIndex];
			OrderBook& book = books.try_emplace(security).first->second;

			OrderStatus status = book.processOrder(order);

			if(!book.isActive())
			{
				books.erase(security);
			}
		}
	);

	return 0;
}

int MatcherRequestProcessor::processOrderUpdateRequest(const OrderUpdateRequest& orderUpdateRequest)
{
	return 0;
}

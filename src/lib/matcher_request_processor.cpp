#include <matcher_request_processor.h>
#include <future>
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
			s_ordersProcessed.fetch_add(1, std::memory_order_relaxed);

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

std::map<std::string, BookSnapshot> MatcherRequestProcessor::snapshotAll(size_t depth)
{
	using Result = std::map<std::string, BookSnapshot>;

	std::vector<std::future<Result>> futures;
	for(size_t i = 0; i < threadPoolSize; ++i)
	{
		auto promise = std::make_shared<std::promise<Result>>();
		futures.push_back(promise->get_future());

		threadPool.enqueue(i, [this, i, depth, promise](){
			Result result;
			for(const auto& [security, book] : orderBooks[i])
			{
				result.emplace(security, book.snapshot(depth));
			}
			promise->set_value(std::move(result));
		});
	}

	Result all;
	for(auto& future : futures)
	{
		all.merge(future.get());
	}
	return all;
}

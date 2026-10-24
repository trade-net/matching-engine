#ifndef INCLUDED_BOOK_MANAGER
#define INCLUDED_BOOK_MANAGER

#include <order_book.h>
#include <partitioned_threadpool.h>
#include <order_request.pb.h>
#include <order.h>
#include <limit.h>
#include <request_processor.h>

#include <memory>

class MatcherRequestProcessor: public network::RequestProcessor{
public:
	explicit MatcherRequestProcessor(size_t poolSize);
	int processOrderRequest(const OrderRequest& order);
	int processOrderUpdateRequest(const OrderUpdateRequest& orderUpdateRequest);

private:
	// one map per partition, only ever touched by that partition's thread.
	// Declared before threadPool so the threads are joined before the books go away.
	std::vector<std::unordered_map<std::string, OrderBook>> orderBooks;
	PartitionedThreadPool threadPool;
	size_t threadPoolSize;
};

#endif

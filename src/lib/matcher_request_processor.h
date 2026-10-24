#ifndef INCLUDED_BOOK_MANAGER
#define INCLUDED_BOOK_MANAGER

#include <order_book.h>
#include <partitioned_threadpool.h>
#include <order_request.pb.h>
#include <order.h>
#include <limit.h>
#include <request_processor.h>

#include <atomic>
#include <map>
#include <memory>

class MatcherRequestProcessor: public network::RequestProcessor{
public:
	explicit MatcherRequestProcessor(size_t poolSize);
	int processOrderRequest(const OrderRequest& order);
	int processOrderUpdateRequest(const OrderUpdateRequest& orderUpdateRequest);

	// Number of orders the partition threads have finished matching.
	uint64_t ordersProcessed() const{
		return s_ordersProcessed.load(std::memory_order_relaxed);
	}

	// Snapshots every active book. Each book is read on the partition thread that
	// owns it, so this blocks until every partition has run its snapshot task.
	// Each book is internally consistent, but books are not captured at one instant.
	// Must not be called from a partition thread.
	std::map<std::string, BookSnapshot> snapshotAll(size_t depth);

private:
	// one map per partition, only ever touched by that partition's thread.
	// Declared before threadPool so the threads are joined before the books go away.
	std::vector<std::unordered_map<std::string, OrderBook>> orderBooks;
	std::atomic<uint64_t> s_ordersProcessed{0};
	PartitionedThreadPool threadPool;
	size_t threadPoolSize;
};

#endif

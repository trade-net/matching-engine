#ifndef INCLUDED_BOOK_MANAGER
#define INCLUDED_BOOK_MANAGER

#include <order_book.h>
#include <partitioned_threadpool.h>
#include <order_request.pb.h>
#include <order.h>
#include <limit.h>
#include <request_processor.h>

#include <memory>

class MatcherRequestProcessor: public RequestProcessor{
public:
	MatcherRequestProcessor(size_t poolSize, size_t RPPoolSize);
	int processOrderRequest(const OrderRequest& order);

private:
	std::unordered_map<std::string, OrderBook> orderBooks;
	PartitionedThreadPool threadPool;
	size_t threadPoolSize;

	std::mutex orderBooksMutex;
};

#endif


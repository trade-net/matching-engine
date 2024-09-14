#ifndef INCLUDED_BOOK_MANAGER
#define INCLUDED_BOOK_MANAGER

#include <order_book.h>
#include <order_request.h>
#include <order.h>
#include <limit.h>

#include <memory>

class BookManager{
public:
	BookManager();
	void processOrderRequest(OrderRequest& order);

private:
	std::unordered_map<std::string, OrderBook> orderBooks;
};

#endif


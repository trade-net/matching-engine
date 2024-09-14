#ifndef INCLUDED_BOOK_MANAGER
#define INCLUDED_BOOK_MANAGER

#include <order_book.h>
#include <order.h>
#include <limit.h>

#include <memory>

enum OrderStatus{
	FILLED;
	PARTIALLY_FILLED;
	UNFILLED;
	FAILED;
};

class BookManager{
public:
	BookManager();

private:
	std::unordered_map<OrderBook> orderBooks;
};

#endif


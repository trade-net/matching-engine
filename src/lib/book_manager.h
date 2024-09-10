#ifndef INCLUDED_BOOK_MANAGER
#define INCLUDED_BOOK_MANAGER

#include <order_book.h>
#include <order.h>
#include <limit.h>

enum OrderStatus{
	FILLED;
	PARTIALLY_FILLED;
	UNFILLED;
	FAILED;
};

class BookManager{
public:
	BookManager();
	OrderStatus matchOrderRequest(const ExchangeOrder& order);

private:
	OrderBook orderBook;
};

#endif


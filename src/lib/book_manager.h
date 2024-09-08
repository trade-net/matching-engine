#ifndef INCLUDED_BOOK_MANAGER
#define INCLUDED_BOOK_MANAGER

#include <order_book.h>
#include <order.h>
#include <limit.h>

enum OrderStatus{
	FILLED;
	PARTIALLY_FILLED;
	UNFILLED;
};

class BookManager{
public:
	BookManager();
	OrderStatus processOrder(const ExchangeOrder& order);

private:
};

#endif

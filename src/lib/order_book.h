#ifndef INCLUDED_ORDER_BOOK
#define INCLUDED_ORDER_BOOK

#include <limit.h>
#include <order.h>

#include <unordered_map>

class OrderBook{
public:
	OrderBook();
	bool addOrder(Order& order);
	int removeUnits(int units, bool isBuy, int limit);

private:
	Limit* buyTree;
	Limit* sellTree;
	Limit* lowestSell;
	Limit* highestBuy;
	std::unordered_map<int, Limit*> limitMap;
	std::unordered_map<int, Order*> orderMap;
	bool isBuyTreeInitialised = false;
	bool isSellTreeInitialised = false;

	void addFirstOrderAtLimit(Order& order);
};

#endif

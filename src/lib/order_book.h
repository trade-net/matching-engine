#ifndef INCLUDED_ORDER_BOOK
#define INCLUDED_ORDER_BOOK

#include <limit.h>
#include <order.h>

#include <unordered_map>

class OrderBook{
public:
	OrderBook();
	bool addOrder(Order& order);
	int removeUnits(int units, bool isBuy, int limit=0);

	// for testing
	bool isLimitInMap(int limit){
		auto it = limitMap.find(limit);
		return it != limitMap.end();
	}

	bool isOrderInMap(int id){
		auto it = orderMap.find(id);
		return it != orderMap.end();
	}

private:
	Limit* buyTree = nullptr;
	Limit* sellTree = nullptr;
	Limit* lowestSell = nullptr;
	Limit* highestBuy = nullptr;
	std::unordered_map<int, Limit*> limitMap;
	std::unordered_map<int, Order*> orderMap;
	bool isBuyTreeInitialised = false;
	bool isSellTreeInitialised = false;

	void addFirstOrderAtLimit(Order& order);
};

#endif

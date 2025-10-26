#ifndef INCLUDED_ORDER_BOOK
#define INCLUDED_ORDER_BOOK

#include <limit.h>
#include <order.h>

#include <unordered_map>
#include <memory>

struct OrderStatus{
	OrderStatus(int units)
	: totalUnits(units)
	, unitsUnfilled(units)
	, unitsInBook(0)
	, unitsFilled(0)
	, priceFilled(0)
	{}

	OrderStatus();

	void fill(int units, int price)
	{
		unitsFilled += units;
		priceFilled += units*price;
		unitsUnfilled -= units;
	}

	void fillRemaining(int price)
	{
		unitsFilled += unitsUnfilled;
		priceFilled += unitsUnfilled*price;
		unitsUnfilled = 0;
	}

	int totalUnits;
	int unitsUnfilled;
	int unitsInBook;
	int unitsFilled;
	int priceFilled;
};

class OrderBook{
public:
	OrderBook();
	OrderStatus processOrder(const Order& order);
	void addOrder(const Order& order, std::optional<int> remainingUnits = std::nullopt);
	OrderStatus matchOrder(int units, bool isBuy, int limit);

	bool isActive(){
		return !buyTree.empty() or !sellTree.empty();
	}

	// for testing
	bool isLimitInMap(int limit){
		auto it = limitMap.find(limit);
		return it != limitMap.end();
	}

	bool isOrderInMap(int id){
		if(auto it = orderToLimitMap.find(id); it != orderToLimitMap.end()){
			return it->second->second.find(id);
		}

		return false;
	}

	int getHighestBuy() const{
		return buyTree.rbegin()->first;
	}

	int getLowestSell() const{
		return sellTree.begin()->first;
	}

private:
	std::map<int, Limit> buyTree;
	std::map<int, Limit> sellTree;

	std::unordered_map<int, std::map<int, Limit>::iterator> limitMap;
	std::unordered_map<int, std::map<int, Limit>::iterator> orderToLimitMap;

	// match orderStatus.unfilledOrders with orders in the current Limit
	// returns true if all orders in current is matched, false otherwise
	bool matchWithLimit(OrderStatus& orderStatus, Limit& current);
};

#endif

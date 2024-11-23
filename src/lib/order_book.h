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
	OrderStatus processOrder(std::shared_ptr<Order> order);
	void addOrder(std::shared_ptr<Order> order);
	OrderStatus matchOrder(std::shared_ptr<Order> order);

	bool isActive(){
		return !buyTree.empty() or !sellTree.empty();
	}

	// for testing
	bool isLimitInMap(int limit){
		auto it = limitMap.find(limit);
		return it != limitMap.end();
	}

	bool isOrderInMap(int id){
		auto it = orderMap.find(id);
		return it != orderMap.end();
	}

	int getHighestBuy() const{
		return buyTree.rbegin()->first;
	}

	int getLowestSell() const{
		return sellTree.begin()->first;
	}

private:
	std::map<int, std::shared_ptr<Limit>> buyTree;
	std::map<int, std::shared_ptr<Limit>> sellTree;

	std::unordered_map<int, std::shared_ptr<Limit>> limitMap;
	std::unordered_map<int, std::shared_ptr<Order>> orderMap;
	void addFirstOrderAtLimit(std::shared_ptr<Order> order);

	// match orderStatus.unfilledOrders with orders in the current Limit
	// returns true if all orders in current is matched, false otherwise
	bool matchWithLimit(OrderStatus& orderStatus, std::shared_ptr<Limit> current);
};

#endif

#ifndef INCLUDED_ORDER_BOOK
#define INCLUDED_ORDER_BOOK

#include <limit.h>
#include <order.h>

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <unordered_map>

struct OrderStatus{
	OrderStatus(int units)
	: totalUnits(units)
	, unitsUnfilled(units)
	, unitsInBook(0)
	, unitsFilled(0)
	, priceFilled(0)
	{}

	void fill(int units, int price)
	{
		unitsFilled += units;
		priceFilled += static_cast<int64_t>(units) * price;
		unitsUnfilled -= units;
	}

	void fillRemaining(int price)
	{
		unitsFilled += unitsUnfilled;
		priceFilled += static_cast<int64_t>(unitsUnfilled) * price;
		unitsUnfilled = 0;
	}

	int totalUnits;
	int unitsUnfilled;
	int unitsInBook;
	int unitsFilled;
	int64_t priceFilled;
};

class OrderBook{
public:
	OrderBook();
	OrderStatus processOrder(const Order& order);
	void addOrder(const Order& order);
	OrderStatus matchOrder(int units, bool isBuy, int limit, bool isMarket);
	bool cancelOrder(int id);

	bool isActive(){
		return !buyTree.empty() or !sellTree.empty();
	}

	// for testing
	bool isLimitInMap(int limit){
		return limitMap.find(limit) != limitMap.end();
	}

	bool isOrderInMap(int id){
		auto it = orderToLimitMap.find(id);
		return it != orderToLimitMap.end() and it->second->find(id);
	}

	std::optional<int> getHighestBuy() const{
		if(buyTree.empty()) return std::nullopt;
		return buyTree.begin()->first;
	}

	std::optional<int> getLowestSell() const{
		if(sellTree.empty()) return std::nullopt;
		return sellTree.begin()->first;
	}

private:
	// buyTree sorted highest price first so both sides can be matched
	// and iterated front-to-back the same way
	std::map<int, Limit, std::greater<int>> buyTree;
	std::map<int, Limit> sellTree;

	std::unordered_map<int, Limit*> limitMap;
	std::unordered_map<int, Limit*> orderToLimitMap;

	// match orderStatus.unfilledOrders with orders in the current Limit
	// returns true if all orders in current is matched, false otherwise
	bool matchWithLimit(OrderStatus& orderStatus, Limit& current);
};

#endif

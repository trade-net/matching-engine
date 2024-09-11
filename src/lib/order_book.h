#ifndef INCLUDED_ORDER_BOOK
#define INCLUDED_ORDER_BOOK

#include <limit.h>
#include <order.h>

#include <unordered_map>
#include <memory>

class OrderBook{
public:
	OrderBook();
	void addOrder(std::shared_ptr<Order> order);
	int removeUnits(int units, bool fromBuyTree, int limit=0);

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
		return highestBuy->price();
	}

	int getLowestSell() const{
		return lowestSell->price();
	}

private:
	std::shared_ptr<Limit> buyTree = nullptr;
	std::shared_ptr<Limit> sellTree = nullptr;
	std::shared_ptr<Limit> lowestSell = nullptr;
	std::shared_ptr<Limit> highestBuy = nullptr;
	std::unordered_map<int, std::shared_ptr<Limit>> limitMap;
	std::unordered_map<int, std::shared_ptr<Order>> orderMap;

	void addFirstOrderAtLimit(std::shared_ptr<Order> order);
};

#endif

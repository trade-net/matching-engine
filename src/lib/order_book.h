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
	void matchOrder(std::shared_ptr<Order> order, OrderStatus& orderStatus);
	void removeUnits(int units, bool fromBuyTree, int limit, int& unitsRemaining, int& unitsFilled, int& priceFilled);

	bool isActive(){
		return buyTree or sellTree;
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
		return highestBuy->price();
	}

	int getLowestSell() const{
		return lowestSell->price();
	}

	int getBuyTreeRoot() const{
		return buyTree->price();
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

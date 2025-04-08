#ifndef INCLUDED_LIMIT
#define INCLUDED_LIMIT

#include <order.h>
#include <memory>
#include <vector>
#include <list>

class Limit{
public:
	Limit(std::shared_ptr<Order> order);

	void addOrderToLimit(std::shared_ptr<Order> order);

	int price() const{
		return s_price;
	}

	int size() const{
		return s_orders.size();
	}

	int volume() const{
		return s_volume;
	}

	void decrementHeadOrder(int units);

	void fillUnits(int units);

private:
	int s_price;
	int s_volume; // number of units
	std::list<std::shared_ptr<Order>> s_orders;
	std::unordered_map<int, std::list<std::shared_ptr<Order>>::iterator> orderMap;
};

#endif

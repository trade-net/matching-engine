#ifndef INCLUDED_LIMIT
#define INCLUDED_LIMIT

#include <order.h>
#include <memory>
#include <vector>

class Limit{
public:
	Limit(Order&& order);

	void addOrderToLimit(Order&& order);

	int price() const{
		return s_price;
	}

	int size() const{
		return s_orders.size();
	}

	int volume() const{
		return s_volume;
	}

	std::shared_ptr<Order> deleteHeadOrder();

	std::shared_ptr<Order> headOrder(){
		return s_orders.front();
	}

	void decrementHeadOrder(int units);

private:
	int s_price;
	int s_volume; // number of units
	std::list<Order> s_orders;
	std::unordered_map<int, std::list<Order>::iterator> orderMap;
};

#endif

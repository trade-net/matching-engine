#ifndef INCLUDED_LIMIT
#define INCLUDED_LIMIT

#include <order.h>
#include <memory>
#include <vector>

class Limit{
public:
	Limit(std::shared_ptr<Order> order);

	void addOrderToLimit(std::shared_ptr<Order> order);

	int price() const{
		return s_price;
	}

	int size() const{
		return s_size;
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
	int s_size; // number of Orders
	int s_volume; // number of units
	std::vector<std::shared_ptr<Order>> s_orders;
};

#endif

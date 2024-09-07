#ifndef INCLUDED_LIMIT
#define INCLUDED_LIMIT

#include <order.h>
#include <memory>

class Limit{
public:
	Limit(std::shared_ptr<Order> order, std::shared_ptr<Limit> parent);

	Limit(std::shared_ptr<Order> order);

	std::shared_ptr<Order> addOrderToLimit(std::shared_ptr<Order> order);

	static std::shared_ptr<Limit> createFirstLimitAtPrice(std::shared_ptr<Order> order, std::shared_ptr<Limit>& root);

	std::shared_ptr<Limit> removeLimit(bool isBuy);

	int price() const{
		return s_price;
	}

	int size() const{
		return s_size;
	}

	int volume() const{
		return s_volume;
	}

	void decrementVolume(int amount)
	{
		s_volume -= amount;
	}

	std::shared_ptr<Order> headOrder() const{
		return s_headOrder;
	}

	std::shared_ptr<Order> tailOrder() const{
		return s_tailOrder;
	}

	void setHeadOrder(std::shared_ptr<Order> order)
	{
		s_headOrder = order;
	}

private:
	int s_price;
	int s_size; // number of Orders
	int s_volume; // number of units
	std::shared_ptr<Limit> s_parent;
	std::shared_ptr<Limit> s_leftChild;
	std::shared_ptr<Limit> s_rightChild;
	std::shared_ptr<Order> s_headOrder;
	std::shared_ptr<Order> s_tailOrder;
};

#endif

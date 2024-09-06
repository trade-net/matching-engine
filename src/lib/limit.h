#ifndef INCLUDED_LIMIT
#define INCLUDED_LIMIT

#include <order.h>

class Limit{
public:
	Limit(Order& order, Limit* parent);

	Limit(Order& order);

	Order* addOrderToLimit(Order& order);

	static Limit* createFirstLimitAtPrice(Order& order, Limit*& root);

	Limit* removeLimit(bool isBuy);

	int price() const{
		return s_price;
	}

	int size() const{
		return s_size;
	}

	int volume() const{
		return s_volume;
	}

	Order* headOrder() const{
		return s_headOrder;
	}

	void setHeadOrder(Order* order)
	{
		s_headOrder = order;
	}

private:
	int s_price;
	int s_size; // number of Orders
	int s_volume; // number of units
	Limit* s_parent;
	Limit* s_leftChild;
	Limit* s_rightChild;
	Order* s_headOrder;
	Order* s_tailOrder;

};

#endif

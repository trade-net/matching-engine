#ifndef INCLUDED_ORDER
#define INCLUDED_ORDER

#include <memory>

class Limit; //forward declaration

class Order{
public:
	Order(int id, bool isBuy, int units, int limit, int timestamp)
	: id(id)
	, isBuy(isBuy)
	, units(units)
	, limit(limit)
	, timestamp(timestamp)
	{}

	int id;
	bool isBuy;
	int units;
	int limit;
	int timestamp;
	std::shared_ptr<Order> nextOrder = nullptr;
	std::shared_ptr<Order> prevOrder = nullptr;
	std::shared_ptr<Limit> parentLimit = nullptr;
};

#endif

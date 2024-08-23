#ifndef INCLUDED_ORDER
#define INCLUDED_ORDER

class Limit; //forward declaration

class Order{
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
	Order* nextOrder = nullptr;
	Order* prevOrder = nullptr;
	Limit* parentLimit = nullptr;
};

#endif

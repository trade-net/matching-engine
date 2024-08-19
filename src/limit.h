#ifndef INCLUDED_LIMIT
#define INCLUDED_LIMIT

#include "order.h"

struct Limit{
	int price;
	int size; // number of Orders
	int volume; // number of units
	Limit* parent;
	Limit* leftChild = nullptr;
	Limit* rightChild = nullptr;
	Order* headOrder = nullptr;
	Order* tailOrder = nullptr;
};

#endif

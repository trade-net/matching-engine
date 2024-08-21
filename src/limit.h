#ifndef INCLUDED_LIMIT
#define INCLUDED_LIMIT

#include "order.h"

struct Limit{
	Limit(Order& order, Limit* parent)
	: price(order.limit)
	, size(1)
	, volume(order.units)
	, parent(parent)
	, headOrder(&order)
	, tailOrder(&order)
	{
	}

	Limit(Order& order)
	: price(order.limit)
	, size(1)
	, volume(order.units)
	, parent(nullptr)
	, headOrder(&order)
	, tailOrder(&order)
	{
	}

	Order* addOrderToLimit(Order& order)
	{
		size += 1;
		volume += order.units;
		tailOrder->nextOrder = &order;
		Order* oldTailOrder = tailOrder;
		tailOrder = &order;
		return oldTailOrder;
	}

	static Limit* createFirstLimitAtPrice(Order& order, Limit*& root)
	{
		if(root == nullptr)
		{
			root = new Limit(order);
			return root;
		}

		Limit* current = root;
		Limit* parent = nullptr;
		while(current != nullptr)
		{
			parent = current;
			if(current->price > order.limit)
			{
				current = current->leftChild;
			}
			else
			{
				current = current->rightChild;
			}
		}

		if(parent->price > order.limit)
		{
			parent->leftChild = new Limit(order, parent);
			return parent->leftChild;
		}
		else
		{
			parent->rightChild = new Limit(order, parent);
			return parent->rightChild;
		}

	}

	int price;
	int size; // number of Orders
	int volume; // number of units
	Limit* parent;
	Limit* leftChild = nullptr;
	Limit* rightChild = nullptr;
	Order* headOrder;
	Order* tailOrder;

};

#endif

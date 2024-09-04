#include "limit.h"

Limit::Limit(Order& order, Limit* parent)
: s_price(order.limit)
, s_size(1)
, s_volume(order.units)
, s_parent(parent)
, s_leftChild(nullptr)
, s_rightChild(nullptr)
, s_headOrder(&order)
, s_tailOrder(&order)
{}

Limit::Limit(Order& order)
: s_price(order.limit)
, s_size(1)
, s_volume(order.units)
, s_parent(nullptr)
, s_leftChild(nullptr)
, s_rightChild(nullptr)
, s_headOrder(&order)
, s_tailOrder(&order)
{}

Order* Limit::addOrderToLimit(Order& order)
{
	s_size += 1;
	s_volume += order.units;
	s_tailOrder->nextOrder = &order;
	Order* oldTailOrder = s_tailOrder;
	s_tailOrder = &order;
	return oldTailOrder;
}

Limit* Limit::createFirstLimitAtPrice(Order& order, Limit*& root)
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
		if(current->s_price > order.limit)
		{
			current = current->s_leftChild;
		}
		else
		{
			current = current->s_rightChild;
		}
	}

	if(parent->s_price > order.limit)
	{
		parent->s_leftChild = new Limit(order, parent);
		return parent->s_leftChild;
	}
	else
	{
		parent->s_rightChild = new Limit(order, parent);
		return parent->s_rightChild;
	}

}

Limit* Limit::removeLimit(bool isBuy)
{
	if(isBuy)
	{
		s_parent->s_rightChild = (s_leftChild ? s_leftChild : nullptr);
	}
	else
	{
		s_parent->s_leftChild = (s_rightChild ? s_rightChild : nullptr);
	}
	return s_parent;
}

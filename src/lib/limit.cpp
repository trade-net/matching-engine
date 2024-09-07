#include <limit.h>
#include <iostream>

Limit::Limit(std::shared_ptr<Order> order, std::shared_ptr<Limit> parent)
: s_price(order->limit)
, s_size(1)
, s_volume(order->units)
, s_parent(parent)
, s_leftChild(nullptr)
, s_rightChild(nullptr)
, s_headOrder(order)
, s_tailOrder(order)
{}

Limit::Limit(std::shared_ptr<Order> order)
: s_price(order->limit)
, s_size(1)
, s_volume(order->units)
, s_parent(nullptr)
, s_leftChild(nullptr)
, s_rightChild(nullptr)
, s_headOrder(order)
, s_tailOrder(order)
{}

std::shared_ptr<Order> Limit::addOrderToLimit(std::shared_ptr<Order> order)
{
	s_size += 1;
	s_volume += order->units;
	s_tailOrder->nextOrder = order;
	std::shared_ptr<Order> oldTailOrder = s_tailOrder;
	s_tailOrder = order;
	return oldTailOrder;
}

std::shared_ptr<Limit> Limit::createFirstLimitAtPrice(std::shared_ptr<Order> order, std::shared_ptr<Limit>& root)
{
	if(root == nullptr)
	{
		root = std::make_shared<Limit>(order);
		return root;
	}

	std::shared_ptr<Limit> current = root;
	std::shared_ptr<Limit> parent = nullptr;
	while(current != nullptr)
	{
		parent = current;
		if(current->s_price > order->limit)
		{
			current = current->s_leftChild;
		}
		else
		{
			current = current->s_rightChild;
		}
	}

	if(parent->s_price > order->limit)
	{
		parent->s_leftChild = std::make_shared<Limit>(order, parent);
		return parent->s_leftChild;
	}
	else
	{
		parent->s_rightChild = std::make_shared<Limit>(order, parent);
		return parent->s_rightChild;
	}

}

std::shared_ptr<Limit> Limit::removeLimit(bool isBuy)
{
	if(isBuy)
	{
		if(s_leftChild)
		{
			if(s_parent)
			{
				s_parent->s_rightChild = s_leftChild;
				s_leftChild->s_parent = s_parent;
				return s_leftChild;
			}
			// if the current limit is the root node of the tree
			else
			{
				// make the left child the new root node
				s_leftChild->s_parent = nullptr;
				// traverse through the right childs of the root node
				// to find the next largest limit
				std::shared_ptr<Limit> nextLimit = s_leftChild;
				while(nextLimit->s_rightChild)
				{
					nextLimit = nextLimit->s_rightChild;
				}
				return nextLimit;
			}
		}

		s_parent->s_rightChild = nullptr;
		return s_parent;
	}
	else
	{
		if(s_rightChild)
		{
			if(s_parent)
			{
				s_parent->s_leftChild = s_rightChild;
				s_rightChild->s_parent = s_parent;
				return s_rightChild;
			}
			else
			{
				s_rightChild->s_parent = nullptr;
				std::shared_ptr<Limit> nextLimit = s_rightChild;
				while(nextLimit->s_leftChild)
				{
					nextLimit = nextLimit->s_leftChild;
				}
				return nextLimit;
			}
		}

		s_parent->s_leftChild = nullptr;
		return s_parent;
	}
}

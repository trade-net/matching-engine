#include <limit.h>
#include <iostream>

Limit::Limit(std::shared_ptr<Order> order)
: s_price(order->limit)
, s_size(1)
, s_volume(order->units)
{
	s_orders.emplace_back(order);
}

void Limit::addOrderToLimit(std::shared_ptr<Order> order)
{
	s_size += 1;
	s_volume += order->units;
	s_orders.emplace_back(order);
}

std::shared_ptr<Order> Limit::deleteHeadOrder()
{
	auto head = s_orders.front();
	s_size -= 1;
	s_volume -= head->units;
	s_orders.erase(s_orders.begin());

	return head;
}

void Limit::decrementHeadOrder(int units)
{
	s_volume -= units;
	s_orders.front()->units -= units;
}

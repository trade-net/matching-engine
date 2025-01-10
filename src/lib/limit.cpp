#include <limit.h>
#include <iostream>

Limit::Limit(std::shared_ptr<Order> order)
: s_price(order->limit)
, s_size(1)
, s_volume(order->units)
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

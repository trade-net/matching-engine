#include <limit.h>
#include <iostream>

Limit::Limit(Order&& order)
: s_price(order.limit)
, s_volume(order.units)
{
	s_orders.push_back(std::move(order));
}

void Limit::addOrderToLimit(Order&& order)
{
	s_volume += order->units;
	s_orders.push_back(std::move(order));
}

void Limit::fillUnits(int units)
{
	while(units){
		auto& head = s_orders.front();
		if(units >= head.units){
			units -= head.units;
			s_volume -= head.units;
			orderMap.erase(head.id);
			s_orders.pop_front();
		}
		else{
			head.units -= units;
			units = 0;
		}
	}
}

std::shared_ptr<Order> Limit::deleteHeadOrder()
{
	auto head = s_orders.front();
	s_volume -= head->units;
	s_orders.pop_front();

	return head;
}

void Limit::decrementHeadOrder(int units)
{
	s_volume -= units;
	s_orders.front().units -= units;
}

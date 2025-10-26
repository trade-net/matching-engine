#include <limit.h>
#include <iostream>

Limit::Limit(const Order& order)
: s_price(order.limit)
, s_volume(order.units)
{
	s_orders.emplace_back(order);
    s_orderMap[order.id] = s_orders.begin();
}

void Limit::addOrderToLimit(const Order& order)
{
	s_volume += order.units;
	s_orders.emplace_back(order);
    s_orderMap[order.id] = std::prev(s_orders.end());
}

std::vector<int> Limit::fillUnits(int units)
{
	std::vector<int> filledOrders;
	while(units){
		auto& head = s_orders.front();
		if(units >= head.units){
			std::cout << "Filling order id=" << head.id << " for " << head.units << " units" << std::endl;
			units -= head.units;
			s_volume -= head.units;

			filledOrders.push_back(head.id);
			s_orders.pop_front();
			s_orderMap.erase(head.id);
		}
		else{
			head.units -= units;
			s_volume -= units;
			units = 0;
		}
	}

	return filledOrders;
}

void Limit::decrementHeadOrder(int units)
{
	s_volume -= units;
	s_orders.front().units -= units;
}

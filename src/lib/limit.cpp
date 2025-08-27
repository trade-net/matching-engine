#include <limit.h>
#include <iostream>

Limit::Limit(std::shared_ptr<Order> order)
: s_price(order->limit)
, s_volume(order->units)
{
	s_orders.push_back(order);
	s_orderMap[order->id] = s_orders.begin();
}

void Limit::addOrderToLimit(std::shared_ptr<Order> order)
{
	s_volume += order->units;
	s_orders.push_back(order);
	s_orderMap[order->id] = std::prev(s_orders.end());
}

std::vector<int> Limit::fillUnits(int units)
{
	std::vector<int> filledOrders;
	while(units){
		auto head = s_orders.front();
		if(units >= head->units){
			units -= head->units;
			s_volume -= head->units;

			filledOrders.push_back(head->id);
			s_orders.pop_front();
			s_orderMap.erase(head->id);
		}
		else{
			head->units -= units;
			s_volume -= units;
			units = 0;
		}
	}

	return filledOrders;
}

void Limit::decrementHeadOrder(int units)
{
	s_volume -= units;
	s_orders.front()->units -= units;
}

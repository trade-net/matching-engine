#include <limit.h>
#include <logger.h>

Limit::Limit(const Order& order)
: s_price(order.limit)
, s_volume(order.units)
, s_isBuy(order.isBuy)
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

void Limit::removeOrder(int id)
{
	auto it = s_orderMap.find(id);
	s_volume -= it->second->units;
	s_orders.erase(it->second);
	s_orderMap.erase(it);
}

std::vector<int> Limit::fillUnits(int units)
{
	std::vector<int> filledOrders;
	while(units){
		auto& head = s_orders.front();
		if(units >= head.units){
			LOG_INFO("Filling order id=" << head.id << " for " << head.units << " units");
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

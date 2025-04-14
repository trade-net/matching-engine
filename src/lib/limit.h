#ifndef INCLUDED_LIMIT
#define INCLUDED_LIMIT

#include <order.h>
#include <memory>
#include <vector>
#include <list>

class Limit{
public:
	Limit(std::shared_ptr<Order> order);

	void addOrderToLimit(std::shared_ptr<Order> order);

	int price() const{
		return s_price;
	}

	int size() const{
		return s_orders.size();
	}

	int volume() const{
		return s_volume;
	}

	void decrementHeadOrder(int units);

	std::vector<int> fillUnits(int units);

	std::vector<int> getOrders(){
		std::vector<int> orders(s_orderMap.size());
		int i=0;
		for(const auto& pair: s_orderMap){
			orders[i++] = pair.first;
		}

		return orders;
	}

	bool find(int id){
		return s_orderMap.find(id) != s_orderMap.end();
	}

private:
	int s_price;
	int s_volume; // number of units
	std::list<std::shared_ptr<Order>> s_orders;
	std::unordered_map<int, std::list<std::shared_ptr<Order>>::iterator> s_orderMap;
};

#endif

#ifndef INCLUDED_LIMIT
#define INCLUDED_LIMIT

#include <order.h>
#include <memory>
#include <vector>
#include <list>

class Limit{
public:
	Limit(Order order);

	void addOrderToLimit(Order order);

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
	std::list<Order> s_orders;
	std::unordered_map<int, std::list<Order>::iterator> s_orderMap;
};

#endif

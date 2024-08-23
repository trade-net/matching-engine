#include "order_book.h"
#include <iostream>


OrderBook::OrderBook(){}

void OrderBook::addFirstOrderAtLimit(Order& order)
{
	Limit* limit;
	if(order.isBuy)
	{
		limit = Limit::createFirstLimitAtPrice(order, buyTree);
	}
	else
	{
		limit = Limit::createFirstLimitAtPrice(order, sellTree);
	}

	order.parentLimit = limit;

	limitMap.insert(std::pair<int, Limit*>(order.limit, limit));

	std::cout << "Added first order at limit $" << order.limit << std::endl;
}


bool OrderBook::addOrder(Order& order)
{
	if(auto it = limitMap.find(order.limit); it != limitMap.end())
	{
		Limit* limitPtr = it->second;
		order.prevOrder = limitPtr->addOrderToLimit(order);
		std::cout << "Added order to limit $" << order.limit << " size=" << limitPtr->size() << " volume=" << limitPtr->volume() << std::endl;

	}
	else
	{
		addFirstOrderAtLimit(order);
	}


	orderMap.insert(std::pair<int, Order*>(order.id, &order));

	return true;
}

bool OrderBook::executeOrder(Order& order);

int main(){
	OrderBook aapl;
	Order a(1, true, 3, 100, 1);
	Order b(2, true, 5, 100, 2);
	Order c(3, true, 5, 100, 3);
	Order d(3, true, 8, 95, 4);
	Order e(3, true, 2, 95, 5);
	Order f(3, true, 3, 97, 6);
	Order g(3, true, 7, 105, 7);
	Order h(3, true, 6, 102, 8);
	Order i(3, true, 2, 105, 9);

	aapl.addOrder(a);
	aapl.addOrder(b);
	aapl.addOrder(c);
	aapl.addOrder(d);
	aapl.addOrder(e);
	aapl.addOrder(f);
	aapl.addOrder(g);
	aapl.addOrder(h);
	aapl.addOrder(i);
}

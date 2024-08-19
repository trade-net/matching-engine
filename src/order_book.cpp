#include "order_book.h"
#include <iostream>

OrderBook::OrderBook(){}

bool OrderBook::addOrder(Order& order)
{
	if(order.isBuy)
	{
		if(isBuyTreeInitialised)
		{
			if(auto it = limitMap.find(order.limit); it != limitMap.end())
			{
				Limit* limitPtr = it->second;
				std::cout << "Found limit" << std::endl;
				if(limitPtr->tailOrder == nullptr)
				{
					std::cout << "This is the second order of the limit" << std::endl;
					limitPtr->headOrder->nextOrder = &order;
					order.prevOrder = limitPtr->headOrder;
					limitPtr->tailOrder = &order;
				}
				else
				{
					std::cout << "Two or more orders are already present" << std::endl;
					limitPtr->tailOrder->nextOrder = &order;
					order.prevOrder = limitPtr->tailOrder;
					limitPtr->tailOrder = &order;
				}
			}

			else
			{

			}
		}
		else
		{
			buyTree.price = order.limit;
			buyTree.size = 1;
			buyTree.volume = order.units;
			buyTree.headOrder = &order;
			order.parentLimit = &buyTree;
			limitMap.insert(std::pair<int, Limit*>(buyTree.price, &buyTree));
			isBuyTreeInitialised = true;
		}

		orderMap.insert(std::pair<int, Order*>(order.id, &order));
	}

	return true;
}

int main(){
	OrderBook aapl;
	Order x(1, true, 3, 100, 1);
	Order y(2, true, 5, 100, 2);
	Order z(3, true, 5, 100, 3);

	aapl.addOrder(x);
	aapl.addOrder(y);
	aapl.addOrder(z);
}

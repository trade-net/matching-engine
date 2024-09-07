#include <order_book.h>
#include <iostream>


OrderBook::OrderBook(){}

void OrderBook::addFirstOrderAtLimit(std::shared_ptr<Order> order)
{
	std::shared_ptr<Limit> limit;
	if(order->isBuy)
	{
		limit = Limit::createFirstLimitAtPrice(order, buyTree);
		if(!highestBuy or limit->price() > highestBuy->price())
		{
			highestBuy = limit;
		}
	}
	else
	{
		limit = Limit::createFirstLimitAtPrice(order, sellTree);
		if(!lowestSell or limit->price() < lowestSell->price())
		{
			lowestSell = limit;
		}
	}

	order->parentLimit = limit;

	limitMap.insert(std::pair<int, std::shared_ptr<Limit>>(order->limit, limit));
	
	std::cout << "Added first order at limit $" << order->limit << std::endl;
}


bool OrderBook::addOrder(std::shared_ptr<Order> order)
{
	if(auto it = limitMap.find(order->limit); it != limitMap.end())
	{
		std::shared_ptr<Limit> limitPtr = it->second;
		order->prevOrder = limitPtr->addOrderToLimit(order);
		std::cout << "Added order to limit $" << order->limit << " size=" << limitPtr->size() << " volume=" << limitPtr->volume() << std::endl;

	}
	else
	{
		addFirstOrderAtLimit(order);
	}


	orderMap.insert(std::pair<int, std::shared_ptr<Order>>(order->id, order));

	return true;
}


// isBuy tells us whether we're removing from the buyTree or sellTree
int OrderBook::removeUnits(int units, bool isBuy, int limit)
{
	std::shared_ptr<Limit> current;
	if(isBuy)
	{
		current = highestBuy;
	}
	else
	{
		current = lowestSell;
	}

	// While there are still units to remove
	// and we haven't reached the end of the tree
	// and we haven't hit the removal limit
	while(units > 0 and current and current->price() >= limit)
	{
		// if the volume of the current limit is less than the number of units to delete
		// remove the whole limit from the tree
		std::cout << "A" << std::endl;
		std::cout << current->volume()  << std::endl;
		std::cout << current->price()  << std::endl;
		if(units >= current->volume())
		{
			// iterate through the orders in the limit and delete them from the map
			// clean up their memory
			std::shared_ptr<Order> currentOrder = current->headOrder();
			while(currentOrder)
			{
				orderMap.erase(currentOrder->id);
				currentOrder = currentOrder->nextOrder;
			}

			// remove the limit from the tree and map, clean up memory
			units -= current->volume();
			limitMap.erase(current->price());
			current = current->removeLimit(isBuy);
			if(isBuy)
			{
				highestBuy = current;
			}
			else
			{
				lowestSell = current;
			}
		}

		// if not, remove the remaining units from the current limit
		else
		{
			// starting from the head order
			std::shared_ptr<Order> currentOrder = current->headOrder();
			while(units)
			{
				// if more units to delete than that of the current order
				// can just delete the order and update the doubly linked list
				if(units >= currentOrder->units)
				{
					// decrement the units remaining by the number of shares in current order
					// update current limit's volume accordingly
					units -= currentOrder->units;	
					current->decrementVolume(currentOrder->units);
					orderMap.erase(currentOrder->id);

					// update headOrder and nextOrder's prevOrder
					currentOrder->nextOrder->prevOrder = nullptr;
					currentOrder = currentOrder->nextOrder;
					current->setHeadOrder(currentOrder);
				}
				// if not, subtract the remaining units from the current order
				// update current limit's volume accordingly
				else
				{
					currentOrder->units -= units;
					current->decrementVolume(units);
					units = 0;
				}
			}
		}
	}
	return units;
}


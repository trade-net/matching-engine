#include <order_book.h>
#include <iostream>


OrderBook::OrderBook(){}

void OrderBook::addFirstOrderAtLimit(Order& order)
{
	Limit* limit;
	if(order.isBuy)
	{
		limit = Limit::createFirstLimitAtPrice(order, buyTree);
		if(limit->price() > highestBuy->price())
		{
			highestBuy = limit;
		}
	}
	else
	{
		limit = Limit::createFirstLimitAtPrice(order, sellTree);
		if(limit->price() < lowestSell->price())
		{
			lowestSell = limit;
		}
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


// isBuy tells us whether we're removing from the buyTree or sellTree
int OrderBook::removeUnits(int units, bool isBuy, int limit)
{
	Limit* current;
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
		if(units >= current->volume())
		{
			// iterate through the orders in the limit and delete them from the map
			// clean up their memory
			Order* currentOrder = current->headOrder();
			Order* orderToDelete = nullptr;
			while(currentOrder)
			{
				orderToDelete = currentOrder;
				orderMap.erase(currentOrder->id);
				currentOrder = currentOrder->nextOrder;
				delete orderToDelete;
			}

			// remove the limit from the tree and map, clean up memory
			Limit* limitToDelete = current;
			current = current->removeLimit(isBuy);

			units -= limitToDelete->volume();
			limitMap.erase(limitToDelete->price());
			delete limitToDelete;
		}

		// if not, remove the remaining units from the current limit
		else
		{
			// starting from the head order
			Order* currentOrder = current->headOrder();
			Order* orderToDelete = nullptr;
			while(units)
			{
				// if more units to delete than that of the current order
				// can just delete the order and update the doubly linked list
				if(units >= currentOrder->units)
				{
					// decrement the units remaining
					units -= currentOrder->units;	
					orderToDelete = currentOrder;
					orderMap.erase(currentOrder->id);

					// update headOrder and nextOrder's prevOrder
					currentOrder->nextOrder->prevOrder = nullptr;
					currentOrder = currentOrder->nextOrder;
					current->setHeadOrder(currentOrder);
					delete orderToDelete;
				}
				// if not, subtract the remaining units from the current order
				else
				{
					currentOrder->units -= units;
					units = 0;
				}
			}
		}
	}
	return units;
}



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
	
	std::cout << "Created " << (order->isBuy ? "buy" : "sell") << " limit=" << order->limit << " and added order id=" << order->id << ", units=" << order->units << std::endl;
}


void OrderBook::addOrder(std::shared_ptr<Order> order)
{
	if(auto it = limitMap.find(order->limit); it != limitMap.end())
	{
		std::shared_ptr<Limit> limitPtr = it->second;
		order->prevOrder = limitPtr->addOrderToLimit(order);
		std::cout << "Added order id=" << order->id << ", units=" << order->units << " to " << (order->isBuy ? "buy" : "sell") << " limit=" << order->limit << ". New size=" << limitPtr->size() << " volume=" << limitPtr->volume() << std::endl;

	}
	else
	{
		addFirstOrderAtLimit(order);
	}


	orderMap.insert(std::pair<int, std::shared_ptr<Order>>(order->id, order));
}

void OrderBook::matchOrder(std::shared_ptr<Order> order, OrderStatus& orderStatus)
{
	removeUnits(order->units, !order->isBuy, order->limit, orderStatus.unitsUnfilled, orderStatus.unitsFilled, orderStatus.priceFilled);
	order->units = orderStatus.unitsUnfilled;
}

void OrderBook::removeUnits(int units, bool fromBuyTree, int limit, int& unitsRemaining, int& unitsFilled, int& priceFilled)
{
	unitsRemaining = units;
	priceFilled = 0;

	std::shared_ptr<Limit> current;
	if(fromBuyTree)
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
	while(unitsRemaining > 0 and current and (fromBuyTree ? current->price() >= limit : current->price() <= limit))
	{
		// if the volume of the current limit is less than the number of units to delete
		// remove the whole limit from the tree
		if(unitsRemaining >= current->volume())
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
			unitsRemaining -= current->volume();
			limitMap.erase(current->price());

			priceFilled += current->volume() * current->price();

			std::cout << "Incoming order matched all orders at " << (fromBuyTree ? "buy" : "sell") << " limit=" << current->price()
				<< ", size=" << current->size()
				<< ", volume=" << current->volume()
				<< std::endl;

			if(fromBuyTree)
			{
				current = current->removeLimit(fromBuyTree, buyTree);
				highestBuy = current;
			}
			else
			{
				current = current->removeLimit(fromBuyTree, sellTree);
				lowestSell = current;
			}

		}

		// if not, remove the remaining units from the current limit
		else
		{
			std::cout << "Incoming order partial match with " << (fromBuyTree ? "buy" : "sell") << " limit=" << current->price()
				<< ", size=" << current->size()
				<< ", volume=" << current->volume()
				<< ": units matched = " << unitsRemaining
				<< std::endl;
			// starting from the head order
			std::shared_ptr<Order> currentOrder = current->headOrder();
			while(unitsRemaining)
			{
				// if more units to delete than that of the current order
				// can just delete the order and update the doubly linked list
				if(unitsRemaining >= currentOrder->units)
				{
					// decrement the units remaining by the number of shares in current order
					// update current limit's volume accordingly
					unitsRemaining -= currentOrder->units;
					current->decrementVolume(currentOrder->units);
					current->decrementSize();
					orderMap.erase(currentOrder->id);

					priceFilled += currentOrder->units * currentOrder->limit;

					// update headOrder and nextOrder's prevOrder
					currentOrder->nextOrder->prevOrder = nullptr;
					currentOrder = currentOrder->nextOrder;
					current->setHeadOrder(currentOrder);
				}
				// if not, subtract the remaining units from the current order
				// update current limit's volume accordingly
				else
				{
					currentOrder->units -= unitsRemaining;
					current->decrementVolume(unitsRemaining);

					priceFilled += unitsRemaining * currentOrder->limit;
					unitsRemaining = 0;
				}
			}
		}
	}

	unitsFilled = units - unitsRemaining;
}

OrderStatus OrderBook::processOrder(std::shared_ptr<Order> order)
{
	std::cout << "Processing order id=" << order->id
		<< ", isBuy=" << order->isBuy
		<< ", units=" << order->units
		<< ", limit=" << order->limit
		<< ", timestamp=" << order->timestamp
		<< ", security=" << order->security
		<< std::endl;

	OrderStatus orderStatus(order->units);

	matchOrder(order, orderStatus);

	std::cout << "Order id=" << order->id << ": " << order->units << " units remaining after matching" << std::endl;

	if(order->units and order->limit)
	{
		addOrder(order);
		orderStatus.unitsInBook = order->units;
	}

	return orderStatus;
}

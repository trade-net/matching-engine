#include <order_book.h>
#include <iostream>


OrderBook::OrderBook(){}

void OrderBook::addFirstOrderAtLimit(std::shared_ptr<Order> order)
{
	std::shared_ptr<Limit> limit = std::make_shared<Limit>(order);
	if(order->isBuy)
	{
		buyTree.insert({order->limit, limit});
	}
	else
	{
		sellTree.insert({order->limit, limit});
	}

	limitMap.insert({order->limit, limit});
	
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

OrderStatus OrderBook::matchOrder(std::shared_ptr<Order> order)
{
	OrderStatus orderStatus(order->units);
	if(!order->isBuy)
	{
		if(buyTree.empty())
		{
			std::cout << "buyTree empty, skipping match" << std::endl;
			return orderStatus;
		}
		for(auto it=buyTree.rbegin(); it != buyTree.rend();)
		{
			if(orderStatus.unitsUnfilled == 0 or it->first < order->limit)
			{
				break;
			}

			if(!matchWithLimit(orderStatus, it->second))
			{
				break;
			}

			it = std::make_reverse_iterator(buyTree.erase(std::prev(it.base())));
		}
	}
	else
	{
		if(sellTree.empty())
		{
			std::cout << "sellTree empty, skipping match" << std::endl;
			return orderStatus;
		}
		for(auto it=sellTree.begin(); it != sellTree.end();)
		{
			if(orderStatus.unitsUnfilled == 0 or it->first > order->limit)
			{
				break;
			}

			if(!matchWithLimit(orderStatus, it->second))
			{
				break;
			}

			it = buyTree.erase(it);
		}
	}
	order->units = orderStatus.unitsUnfilled;

	return orderStatus;
}

bool OrderBook::matchWithLimit(OrderStatus& orderStatus, std::shared_ptr<Limit> current)
{
	// if the volume of the current limit is less than the number of units to delete
	// remove the whole limit from the tree
	if(orderStatus.unitsUnfilled >= current->volume())
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
		orderStatus.fill(current->volume(), current->price());
		limitMap.erase(current->price());

		std::cout << "Incoming order matched all orders at limit=" << current->price()
			<< ", size=" << current->size()
			<< ", volume=" << current->volume()
			<< std::endl;

		return true;
	}

	// if not, remove the remaining units from the current limit
	else
	{
		std::cout << "Incoming order partial match with limit=" << current->price()
			<< ", size=" << current->size()
			<< ", volume=" << current->volume()
			<< ": units matched = " << orderStatus.unitsUnfilled
			<< std::endl;
		// starting from the head order
		std::shared_ptr<Order> currentOrder = current->headOrder();
		while(orderStatus.unitsUnfilled)
		{
			// if more units to delete than that of the current order
			// can just delete the order and update the doubly linked list
			if(orderStatus.unitsUnfilled >= currentOrder->units)
			{
				// decrement the units remaining by the number of shares in current order
				// update current limit's volume accordingly
				orderStatus.fill(currentOrder->units, currentOrder->limit);
				current->decrementVolume(currentOrder->units);
				current->decrementSize();
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
				currentOrder->units -= orderStatus.unitsUnfilled;
				current->decrementVolume(orderStatus.unitsUnfilled);

				orderStatus.fillRemaining(currentOrder->limit);
			}
		}
	}

	return false;
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

	OrderStatus orderStatus = matchOrder(order);

	std::cout << "Order id=" << order->id << ": " << order->units << " units remaining after matching" << std::endl;

	if(order->units and order->limit)
	{
		addOrder(order);
		orderStatus.unitsInBook = order->units;
	}

	return orderStatus;
}

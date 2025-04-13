#include <order_book.h>
#include <iostream>


OrderBook::OrderBook(){}


void OrderBook::addOrder(std::shared_ptr<Order> order)
{
	if(auto it = limitMap.find(order->limit); it != limitMap.end()){
		it->second->second.addOrderToLimit(order);
		orderToLimitMap[order->id] = it->second;
	}
	else{
		if(order->isBuy){
			auto [mapIt, _] = buyTree.emplace(order->limit, Limit(order));
			limitMap[order->limit] = mapIt;
			orderToLimitMap[order->id] = mapIt;
		}
		else{
			auto [mapIt, _] = sellTree.emplace(order->limit, Limit(order));
			limitMap[order->limit] = mapIt;
			orderToLimitMap[order->id] = mapIt;
		}
	}	

}

OrderStatus OrderBook::matchOrder(std::shared_ptr<Order> order)
{
	std::cout << "Matching Order " << order->id << std::endl;
	OrderStatus orderStatus(order->units);
	if(!order->isBuy)
	{
		for(auto it=buyTree.rbegin(); it != buyTree.rend();)
		{
			if(
				orderStatus.unitsUnfilled == 0 or 
				it->first < order->limit or
				!matchWithLimit(orderStatus, it->second)
			){
				break;
			}

			// remove the limit from the tree and map, clean up memory
			it = std::make_reverse_iterator(buyTree.erase(std::prev(it.base())));
		}
	}
	else
	{
		for(auto it=sellTree.begin(); it != sellTree.end();)
		{
			if(
				orderStatus.unitsUnfilled == 0 or
				it->first > order->limit or
				!matchWithLimit(orderStatus, it->second)
			){
				break;
			}

			// remove the limit from the tree and map, clean up memory
			it = sellTree.erase(it);
		}
	}
	return orderStatus;
}

bool OrderBook::matchWithLimit(OrderStatus& orderStatus, Limit& current)
{
	// if the volume of the current limit is less than the number of units to delete
	// remove the whole limit from the tree
	if(orderStatus.unitsUnfilled >= current.volume())
	{
		std::cout << "Incoming order matched all orders at limit=" << current.price()
			<< ", size=" << current.size()
			<< ", volume=" << current.volume()
			<< std::endl;

		for(auto& id : current.getOrders())
		{
			orderToLimitMap.erase(id);
		}
		limitMap.erase(current.price());
		orderStatus.fill(current.volume(), current.price());
		return true;
	}

	// if not, remove the remaining units from the current limit
	std::cout << "Incoming order partial match with limit=" << current.price()
		<< ", size=" << current.size()
		<< ", volume=" << current.volume()
		<< ": units matched = " << orderStatus.unitsUnfilled
		<< std::endl;
	
	auto ordersToDelete = current.fillUnits(orderStatus.unitsUnfilled);
	for(auto& id : ordersToDelete)
	{
		orderToLimitMap.erase(id);
	}
	orderStatus.fillRemaining(current.price());

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

	// update remaining units in order
	order->units = orderStatus.unitsUnfilled;

	std::cout << "Order id=" << order->id << ": " << order->units << " units remaining after matching" << std::endl;

	if(order->units and order->limit)
	{
		addOrder(order);
		orderStatus.unitsInBook = order->units;
	}

	return orderStatus;
}

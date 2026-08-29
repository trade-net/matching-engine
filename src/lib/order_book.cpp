#include <order_book.h>
#include <logger.h>


OrderBook::OrderBook(){}


void OrderBook::addOrder(const Order& order)
{
	if(auto it = limitMap.find(order.limit); it != limitMap.end()){
		it->second->second.addOrderToLimit(order);
		orderToLimitMap[order.id] = it->second;
	}
	else{
		if(order.isBuy){
			auto [mapIt, _] = buyTree.emplace(order.limit, Limit(order));
			limitMap[order.limit] = mapIt;
			orderToLimitMap[order.id] = mapIt;
		}
		else{
			auto [mapIt, _] = sellTree.emplace(order.limit, Limit(order));
			limitMap[order.limit] = mapIt;
			orderToLimitMap[order.id] = mapIt;
		}
	}    
}

OrderStatus OrderBook::matchOrder(int units, bool isBuy, int limit, bool isMarket)
{
	OrderStatus orderStatus(units);
	if(!isBuy)
	{
		for(auto it=buyTree.rbegin(); it != buyTree.rend();)
		{
			if(
				orderStatus.unitsUnfilled == 0 or
				(!isMarket and it->first < limit) or
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
				(!isMarket and it->first > limit) or
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
		LOG_INFO("Incoming order matched all orders at limit=" << current.price()
			<< ", size=" << current.size()
			<< ", volume=" << current.volume());

		for(auto& id : current.getOrders())
		{
			orderToLimitMap.erase(id);
		}
		limitMap.erase(current.price());
		orderStatus.fill(current.volume(), current.price());
		return true;
	}

	// if not, remove the remaining units from the current limit
	LOG_INFO("Incoming order partial match with limit=" << current.price()
		<< ", size=" << current.size()
		<< ", volume=" << current.volume()
		<< ": units matched = " << orderStatus.unitsUnfilled);

	auto ordersToDelete = current.fillUnits(orderStatus.unitsUnfilled);
	for(auto& id : ordersToDelete)
	{
		orderToLimitMap.erase(id);
	}
	orderStatus.fillRemaining(current.price());

	return false;
}

OrderStatus OrderBook::processOrder(const Order& order)
{
	LOG_INFO("Processing order id=" << order.id
		<< ", isBuy=" << order.isBuy
		<< ", units=" << order.units
		<< ", limit=" << order.limit
		<< ", isMarket=" << order.isMarket
		<< ", timestamp=" << order.timestamp
		<< ", security=" << order.security);

	OrderStatus orderStatus = matchOrder(order.units, order.isBuy, order.limit, order.isMarket);

	LOG_INFO("Order id=" << order.id << ": " << orderStatus.unitsUnfilled << " units remaining after matching");

	// market orders never rest on the book: unfilled units are dropped
	if(orderStatus.unitsUnfilled && !order.isMarket)
	{
		auto remainingOrder = order.withUnits(orderStatus.unitsUnfilled);
		addOrder(remainingOrder);
		orderStatus.unitsInBook = orderStatus.unitsUnfilled;
	}

	return orderStatus;
}

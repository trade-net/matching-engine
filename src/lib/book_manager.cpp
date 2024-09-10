#include <book_manager.h>
#include <memory>

BookManager::BookManager()
: orderBook()
{}

OrderStatus BookManager::matchOrderRequest(const OrderRequest& orderRequest)
{
	std::shared_ptr<Order> order = orderRequest.toOrder();
	int orderLimit = 0;
	if(orderRequest.type() == OrderRequestType::LIMIT)
	{
		orderLimit = order->limit;
	}

	// remove shares from the sell tree if order isBuy, and buy tree of order isSell
	int remaining = orderBook.removeUnits(order->units, !order->isBuy, orderLimit);

	if(remaining)
	{
		if(orderLimit)
		{
			order->units = remaining;
			orderBook.addOrder(order);
			return (remaining == order->units) ? OrderStatus::UNFILLED : OrderStatus::PARTIALLY_FILLED;
		}
		else
		{
			return (remaining == order->units) ? OrderStatus::FAILED : OrderStatus::PARTIALLY_FILLED;
		}
	}

	return OrderStatus::FILLED;
}


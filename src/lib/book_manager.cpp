#include <book_manager.h>
#include <memory>

BookManager::BookManager()
: orderBook()
{}

OrderStatus BookManager::processOrderRequest(const OrderRequest& orderRequest)
{
	std::shared_ptr<Order> order = orderRequest.toOrder();
	int orderLimit = 0;
	if(orderRequest.type() == OrderRequestType::LIMIT)
	{
		orderLimit = order->limit;
	}

	int remaining = orderBook.removeUnits(order->units, order->isBuy, orderLimit);

	if(remaining)
	{
		if(orderLimit)
		{
			order->units = remaining;
			orderBook.addOrder(order);
		}
		return OrderStatus::PARTIALLY_FILLED;
	}

	return OrderStatus::FILLED;
}


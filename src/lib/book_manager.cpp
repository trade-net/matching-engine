#include <book_manager.h>
#include <memory>

BookManager::BookManager()
{}

void BookManager::processOrderRequest(OrderRequest& orderRequest)
{
	std::shared_ptr<Order> order = orderRequest.toOrder();
	std::string security = orderRequest.security;
	OrderStatus status;

	auto it = orderBooks.find(security);
	if(it == orderBooks.end())
	{
		orderBooks.insert(std::pair<std::string, OrderBook>(security, OrderBook()));
		it = orderBooks.find(security);
	}

	OrderBook& book = it->second;
	status = book.processOrder(order);
	if(!book.isActive())
	{
		orderBooks.erase(security);
	}
}


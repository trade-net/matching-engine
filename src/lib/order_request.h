#ifndef INCLUDED_ORDER_REQUEST
#define INCLUDED_ORDER_REQUEST

struct OrderRequest{
	OrderRequest(){}
	std::shared_ptr<Order> toOrder();
	std::string security = "";
};

#endif

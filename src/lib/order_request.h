#ifndef INCLUDED_ORDER_REQUEST
#define INCLUDED_ORDER_REQUEST

struct OrderRequest{
	OrderRequest(int id, bool isBuy, int units, int limit, int timestamp, const std::string& security)
	: id(id)
	, isBuy(isBuy)
	, units(units)
	, limit(limit)
	, timestamp(timestamp)
	, security(security)
	{}
	std::shared_ptr<Order> toOrder()
	{
		return std::make_unique<Order>(id, isBuy, units, limit, timestamp);
	}

	int id;
	bool isBuy;
	int units;
	int limit;
	int timestamp;
	std::string security;
};

#endif

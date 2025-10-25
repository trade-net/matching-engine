#ifndef INCLUDED_ORDER
#define INCLUDED_ORDER

#include <memory>
#include <order_request.pb.h>

class Limit; //forward declaration

class Order{
public:
	Order(int id, bool isBuy, int units, int limit, int timestamp, const std::string& security)
	: id(id)
	, isBuy(isBuy)
	, units(units)
	, limit(limit)
	, timestamp(timestamp)
	, security(security)
	{}

	// to be deleted eventually, for testing only
	Order(int id, bool isBuy, int units, int limit, int timestamp)
	: id(id)
	, isBuy(isBuy)
	, units(units)
	, limit(limit)
	, timestamp(timestamp)
	, security("")
	{}

	static Order fromOrderRequest(const OrderRequest& orderRequest)
	{
		return Order(
			orderRequest.id(),
			orderRequest.is_buy(),
			orderRequest.units(),
			orderRequest.limit(),
			orderRequest.timestamp(),
			orderRequest.security()
		);
	}

	int id;
	bool isBuy;
	int units;
	int limit;
	int timestamp;
	std::string security;
};

#endif

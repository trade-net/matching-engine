#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <order_book.h>
#include <order.h>
#include <memory>
#include <iostream>

namespace{

class OrderBookTest: public testing::Test{
public:
	OrderBookTest()
	{
		book.addOrder(a);
		book.addOrder(b);
		book.addOrder(c);
		book.addOrder(d);
		book.addOrder(e);
		book.addOrder(f);
		book.addOrder(g);
		book.addOrder(h);
		book.addOrder(i);
		book.addOrder(j);
	}
	
	OrderBook book;
	std::shared_ptr<Order> a = std::make_shared<Order>(1, true, 3, 100, 1);
	std::shared_ptr<Order> b = std::make_shared<Order>(2, true, 5, 100, 2);
	std::shared_ptr<Order> c = std::make_shared<Order>(3, true, 5, 100, 3);
	std::shared_ptr<Order> d = std::make_shared<Order>(4, true, 8, 95, 4);
	std::shared_ptr<Order> e = std::make_shared<Order>(5, true, 23, 95, 5);
	std::shared_ptr<Order> f = std::make_shared<Order>(6, true, 3, 97, 6);
	std::shared_ptr<Order> g = std::make_shared<Order>(7, true, 7, 105, 7);
	std::shared_ptr<Order> h = std::make_shared<Order>(8, true, 6, 102, 8);
	std::shared_ptr<Order> i = std::make_shared<Order>(9, true, 12, 102, 9);
	std::shared_ptr<Order> j = std::make_shared<Order>(10, true, 10, 98, 10);
};

TEST_F(OrderBookTest, testRemoveUnits){
	int remaining = book.removeUnits(7, true);
	ASSERT_EQ(remaining, 0);
	ASSERT_FALSE(book.isOrderInMap(7));
	ASSERT_FALSE(book.isLimitInMap(105));
	ASSERT_EQ(book.getHighestBuy(), 102);

	remaining = book.removeUnits(20, true, 102);
	ASSERT_EQ(remaining, 2);
	ASSERT_EQ(book.getHighestBuy(), 100);
	ASSERT_FALSE(book.isOrderInMap(8));
	ASSERT_FALSE(book.isOrderInMap(9));
	ASSERT_FALSE(book.isLimitInMap(102));

	remaining = book.removeUnits(15, true);
	ASSERT_EQ(remaining, 0);
	ASSERT_EQ(book.getHighestBuy(), 98);
	ASSERT_FALSE(book.isOrderInMap(1));
	ASSERT_FALSE(book.isOrderInMap(2));
	ASSERT_FALSE(book.isOrderInMap(3));
	ASSERT_FALSE(book.isLimitInMap(100));
	ASSERT_TRUE(book.isLimitInMap(98));

	remaining = book.removeUnits(8, true);
	ASSERT_EQ(remaining, 0);
	ASSERT_EQ(book.getHighestBuy(), 97);
	ASSERT_FALSE(book.isOrderInMap(10));
	ASSERT_FALSE(book.isLimitInMap(98));
}

}

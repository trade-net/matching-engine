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
		book.addOrder(k);
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
	std::shared_ptr<Order> k = std::make_shared<Order>(11, true, 13, 96, 11);
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

	remaining = book.removeUnits(20, true, 96);
	ASSERT_EQ(remaining, 4);
	ASSERT_EQ(book.getHighestBuy(), 95);
	ASSERT_FALSE(book.isOrderInMap(6));
	ASSERT_FALSE(book.isOrderInMap(11));
	ASSERT_FALSE(book.isLimitInMap(97));
	ASSERT_FALSE(book.isLimitInMap(96));

	remaining = book.removeUnits(40, true);
	ASSERT_EQ(remaining, 9);
	ASSERT_FALSE(book.isOrderInMap(4));
	ASSERT_FALSE(book.isOrderInMap(5));
	ASSERT_FALSE(book.isLimitInMap(95));
}

TEST_F(OrderBookTest, testRemoveAndAddUnits)
{
	int remaining = book.removeUnits(7, true);
	ASSERT_EQ(remaining, 0);
	ASSERT_FALSE(book.isOrderInMap(7));
	ASSERT_FALSE(book.isLimitInMap(105));
	ASSERT_EQ(book.getHighestBuy(), 102);

	remaining = book.removeUnits(5, true, 102);
	ASSERT_EQ(remaining, 0);
	ASSERT_EQ(book.getHighestBuy(), 102);
	ASSERT_TRUE(book.isOrderInMap(8));
	ASSERT_TRUE(book.isOrderInMap(9));
	ASSERT_TRUE(book.isLimitInMap(102));

	remaining = book.removeUnits(1, true);
	ASSERT_EQ(remaining, 0);
	ASSERT_EQ(book.getHighestBuy(), 102);
	ASSERT_FALSE(book.isOrderInMap(8));
	ASSERT_TRUE(book.isOrderInMap(9));
	ASSERT_TRUE(book.isLimitInMap(102));

	std::shared_ptr<Order> order104 = std::make_shared<Order>(12, true, 8, 104, 12);
	book.addOrder(order104);
	ASSERT_EQ(book.getHighestBuy(), 104);
	ASSERT_TRUE(book.isLimitInMap(104));
	ASSERT_TRUE(book.isOrderInMap(12));

	remaining = book.removeUnits(20, true, 103);
	ASSERT_EQ(remaining, 12);
	ASSERT_EQ(book.getHighestBuy(), 102);
	ASSERT_FALSE(book.isOrderInMap(12));
	ASSERT_FALSE(book.isLimitInMap(104));
	ASSERT_TRUE(book.isOrderInMap(9));
	ASSERT_TRUE(book.isLimitInMap(102));

	book.addOrder(order104);
	ASSERT_EQ(book.getHighestBuy(), 104);
	ASSERT_TRUE(book.isLimitInMap(104));
	ASSERT_TRUE(book.isOrderInMap(12));

	remaining = book.removeUnits(20, true, 100);
	ASSERT_EQ(remaining, 0);
	ASSERT_EQ(book.getHighestBuy(), 100);
	ASSERT_FALSE(book.isOrderInMap(12));
	ASSERT_FALSE(book.isLimitInMap(104));
	ASSERT_FALSE(book.isOrderInMap(9));
	ASSERT_FALSE(book.isLimitInMap(102));
	
}

}

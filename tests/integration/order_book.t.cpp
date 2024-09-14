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
	int remaining, unitsFilled, priceFilled;
	book.removeUnits(7, true, 0, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 0);
	EXPECT_EQ(unitsFilled, 7);
	EXPECT_EQ(priceFilled, 7*105);
	EXPECT_FALSE(book.isOrderInMap(7));
	EXPECT_FALSE(book.isLimitInMap(105));
	EXPECT_EQ(book.getHighestBuy(), 102);

	book.removeUnits(20, true, 102, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 2);
	EXPECT_EQ(unitsFilled, 18);
	EXPECT_EQ(priceFilled, 18*102);
	EXPECT_EQ(book.getHighestBuy(), 100);
	EXPECT_FALSE(book.isOrderInMap(8));
	EXPECT_FALSE(book.isOrderInMap(9));
	EXPECT_FALSE(book.isLimitInMap(102));

	book.removeUnits(15, true, 0, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 0);
	EXPECT_EQ(unitsFilled, 15);
	EXPECT_EQ(priceFilled, 100*13 + 98*2);
	EXPECT_EQ(book.getHighestBuy(), 98);
	EXPECT_FALSE(book.isOrderInMap(1));
	EXPECT_FALSE(book.isOrderInMap(2));
	EXPECT_FALSE(book.isOrderInMap(3));
	EXPECT_FALSE(book.isLimitInMap(100));
	EXPECT_TRUE(book.isLimitInMap(98));

	book.removeUnits(8, true, 0, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 0);
	EXPECT_EQ(unitsFilled, 8);
	EXPECT_EQ(priceFilled, 98*8);
	EXPECT_EQ(book.getHighestBuy(), 97);
	EXPECT_FALSE(book.isOrderInMap(10));
	EXPECT_FALSE(book.isLimitInMap(98));

	book.removeUnits(20, true, 96, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 4);
	EXPECT_EQ(unitsFilled, 16);
	EXPECT_EQ(priceFilled, 97*3 + 96*13);
	EXPECT_EQ(book.getHighestBuy(), 95);
	EXPECT_FALSE(book.isOrderInMap(6));
	EXPECT_FALSE(book.isOrderInMap(11));
	EXPECT_FALSE(book.isLimitInMap(97));
	EXPECT_FALSE(book.isLimitInMap(96));

	book.removeUnits(40, true, 0, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 9);
	EXPECT_EQ(unitsFilled, 31);
	EXPECT_EQ(priceFilled, 31*95);
	EXPECT_FALSE(book.isOrderInMap(4));
	EXPECT_FALSE(book.isOrderInMap(5));
	EXPECT_FALSE(book.isLimitInMap(95));
}

TEST_F(OrderBookTest, testRemoveAndAddUnits)
{
	int remaining, unitsFilled, priceFilled;
	book.removeUnits(7, true, 0, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 0);
	EXPECT_EQ(unitsFilled, 7);
	EXPECT_EQ(priceFilled, 7*105);
	EXPECT_FALSE(book.isOrderInMap(7));
	EXPECT_FALSE(book.isLimitInMap(105));
	EXPECT_EQ(book.getHighestBuy(), 102);

	book.removeUnits(5, true, 102, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 0);
	EXPECT_EQ(unitsFilled, 5);
	EXPECT_EQ(priceFilled, 102*5);
	EXPECT_EQ(book.getHighestBuy(), 102);
	EXPECT_TRUE(book.isOrderInMap(8));
	EXPECT_TRUE(book.isOrderInMap(9));
	EXPECT_TRUE(book.isLimitInMap(102));

	book.removeUnits(1, true, 0, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 0);
	EXPECT_EQ(unitsFilled, 1);
	EXPECT_EQ(priceFilled, 102);
	EXPECT_EQ(book.getHighestBuy(), 102);
	EXPECT_FALSE(book.isOrderInMap(8));
	EXPECT_TRUE(book.isOrderInMap(9));
	EXPECT_TRUE(book.isLimitInMap(102));

	std::shared_ptr<Order> order104 = std::make_shared<Order>(12, true, 8, 104, 12);
	book.addOrder(order104);
	EXPECT_EQ(book.getHighestBuy(), 104);
	EXPECT_TRUE(book.isLimitInMap(104));
	EXPECT_TRUE(book.isOrderInMap(12));

	book.removeUnits(20, true, 103, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 12);
	EXPECT_EQ(unitsFilled, 8);
	EXPECT_EQ(priceFilled, 104*8);
	EXPECT_EQ(book.getHighestBuy(), 102);
	EXPECT_FALSE(book.isOrderInMap(12));
	EXPECT_FALSE(book.isLimitInMap(104));
	EXPECT_TRUE(book.isOrderInMap(9));
	EXPECT_TRUE(book.isLimitInMap(102));

	book.addOrder(order104);
	EXPECT_EQ(book.getHighestBuy(), 104);
	EXPECT_TRUE(book.isLimitInMap(104));
	EXPECT_TRUE(book.isOrderInMap(12));

	book.removeUnits(20, true, 100, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 0);
	EXPECT_EQ(unitsFilled, 20);
	EXPECT_EQ(priceFilled, 104*8 + 102*12);
	EXPECT_EQ(book.getHighestBuy(), 100);
	EXPECT_FALSE(book.isOrderInMap(12));
	EXPECT_FALSE(book.isLimitInMap(104));
	EXPECT_FALSE(book.isOrderInMap(9));
	EXPECT_FALSE(book.isLimitInMap(102));

	EXPECT_EQ(book.getBuyTreeRoot(), 100);
	book.removeUnits(15, true, 0, remaining, unitsFilled, priceFilled);
	EXPECT_EQ(remaining, 0);
	EXPECT_EQ(unitsFilled, 15);
	EXPECT_EQ(priceFilled, 100*13 + 98*2);
	EXPECT_EQ(book.getHighestBuy(), 98);
	EXPECT_FALSE(book.isOrderInMap(3));
	EXPECT_FALSE(book.isLimitInMap(100));
	EXPECT_EQ(book.getBuyTreeRoot(), 95);

	std::shared_ptr<Order> order99 = std::make_shared<Order>(13, true, 4, 99, 13);
	book.addOrder(order99);
	EXPECT_EQ(book.getBuyTreeRoot(), 95);
	EXPECT_EQ(book.getHighestBuy(), 99);
	EXPECT_TRUE(book.isLimitInMap(99));
	EXPECT_TRUE(book.isOrderInMap(13));
	
}
}

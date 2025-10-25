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
	Order a = Order(1, true, 3, 100, 1);
	Order b = Order(2, true, 5, 100, 2);
	Order c = Order(3, true, 5, 100, 3);
	Order d = Order(4, true, 8, 95, 4);
	Order e = Order(5, true, 23, 95, 5);
	Order f = Order(6, true, 3, 97, 6);
	Order g = Order(7, true, 7, 105, 7);
	Order h = Order(8, true, 6, 102, 8);
	Order i = Order(9, true, 12, 102, 9);
	Order j = Order(10, true, 10, 98, 10);
	Order k = Order(11, true, 13, 96, 11);
};

TEST_F(OrderBookTest, testRemoveUnits){
	Order order100 = Order(100, false, 7, 0, 100);
	OrderStatus status= book.matchOrder(7, false, 0);
	EXPECT_EQ(status.unitsUnfilled, 0);
	EXPECT_EQ(status.unitsFilled, 7);
	EXPECT_EQ(status.priceFilled, 7*105);
	EXPECT_FALSE(book.isOrderInMap(7));
	EXPECT_FALSE(book.isLimitInMap(105));
	EXPECT_EQ(book.getHighestBuy(), 102);

	Order order101 = Order(101, false, 20, 102, 101);
	status= book.matchOrder(20, false, 102);
	EXPECT_EQ(status.unitsUnfilled, 2);
	EXPECT_EQ(status.unitsFilled, 18);
	EXPECT_EQ(status.priceFilled, 18*102);
	EXPECT_EQ(book.getHighestBuy(), 100);
	EXPECT_FALSE(book.isOrderInMap(8));
	EXPECT_FALSE(book.isOrderInMap(9));
	EXPECT_FALSE(book.isLimitInMap(102));

	Order order102 = Order(102, false, 15, 0, 102);
	status= book.matchOrder(15, false, 0);
	EXPECT_EQ(status.unitsUnfilled, 0);
	EXPECT_EQ(status.unitsFilled, 15);
	EXPECT_EQ(status.priceFilled, 100*13 + 98*2);
	EXPECT_EQ(book.getHighestBuy(), 98);
	EXPECT_FALSE(book.isOrderInMap(1));
	EXPECT_FALSE(book.isOrderInMap(2));
	EXPECT_FALSE(book.isOrderInMap(3));
	EXPECT_FALSE(book.isLimitInMap(100));
	EXPECT_TRUE(book.isLimitInMap(98));

	Order order103 = Order(103, false, 8, 0, 103);
	status= book.matchOrder(8, false, 0);
	EXPECT_EQ(status.unitsUnfilled, 0);
	EXPECT_EQ(status.unitsFilled, 8);
	EXPECT_EQ(status.priceFilled, 98*8);
	EXPECT_EQ(book.getHighestBuy(), 97);
	EXPECT_FALSE(book.isOrderInMap(10));
	EXPECT_FALSE(book.isLimitInMap(98));

	Order order104 = Order(104, false, 20, 96, 104);
	status= book.matchOrder(20, false, 96);
	EXPECT_EQ(status.unitsUnfilled, 4);
	EXPECT_EQ(status.unitsFilled, 16);
	EXPECT_EQ(status.priceFilled, 97*3 + 96*13);
	EXPECT_EQ(book.getHighestBuy(), 95);
	EXPECT_FALSE(book.isOrderInMap(6));
	EXPECT_FALSE(book.isOrderInMap(11));
	EXPECT_FALSE(book.isLimitInMap(97));
	EXPECT_FALSE(book.isLimitInMap(96));

	Order order105 = Order(105, false, 40, 0, 105);
	status= book.matchOrder(40, false, 0);
	EXPECT_EQ(status.unitsUnfilled, 9);
	EXPECT_EQ(status.unitsFilled, 31);
	EXPECT_EQ(status.priceFilled, 31*95);
	EXPECT_FALSE(book.isOrderInMap(4));
	EXPECT_FALSE(book.isOrderInMap(5));
	EXPECT_FALSE(book.isLimitInMap(95));
}

TEST_F(OrderBookTest, testRemoveAndAddUnits)
{
	Order order100 = Order(100, false, 7, 0, 100);
	OrderStatus status = book.matchOrder(7, false, 0);
	EXPECT_EQ(status.unitsUnfilled, 0);
	EXPECT_EQ(status.unitsFilled, 7);
	EXPECT_EQ(status.priceFilled, 7*105);
	EXPECT_FALSE(book.isOrderInMap(7));
	EXPECT_FALSE(book.isLimitInMap(105));
	EXPECT_EQ(book.getHighestBuy(), 102);

	Order order101 = Order(101, false, 5, 102, 101);
	status= book.matchOrder(5, false, 102);
	EXPECT_EQ(status.unitsUnfilled, 0);
	EXPECT_EQ(status.unitsFilled, 5);
	EXPECT_EQ(status.priceFilled, 102*5);
	EXPECT_EQ(book.getHighestBuy(), 102);
	EXPECT_TRUE(book.isOrderInMap(8));
	EXPECT_TRUE(book.isOrderInMap(9));
	EXPECT_TRUE(book.isLimitInMap(102));

	Order order102 = Order(102, false, 1, 0, 102);
	status= book.matchOrder(1, false, 0);
	EXPECT_EQ(status.unitsUnfilled, 0);
	EXPECT_EQ(status.unitsFilled, 1);
	EXPECT_EQ(status.priceFilled, 102);
	EXPECT_EQ(book.getHighestBuy(), 102);
	EXPECT_FALSE(book.isOrderInMap(8));
	EXPECT_TRUE(book.isOrderInMap(9));
	EXPECT_TRUE(book.isLimitInMap(102));

	Order order103 = Order(103, true, 8, 104, 103);
	book.addOrder(order103);
	EXPECT_EQ(book.getHighestBuy(), 104);
	EXPECT_TRUE(book.isLimitInMap(104));
	EXPECT_TRUE(book.isOrderInMap(103));

	Order order104 = Order(104, false, 20, 103, 104);
	status = book.matchOrder(20, false, 103);
	EXPECT_EQ(status.unitsUnfilled, 12);
	EXPECT_EQ(status.unitsFilled, 8);
	EXPECT_EQ(status.priceFilled, 104*8);
	EXPECT_EQ(book.getHighestBuy(), 102);
	EXPECT_FALSE(book.isOrderInMap(12));
	EXPECT_FALSE(book.isLimitInMap(104));
	EXPECT_TRUE(book.isOrderInMap(9));
	EXPECT_TRUE(book.isLimitInMap(102));

	book.addOrder(order103);
	EXPECT_EQ(book.getHighestBuy(), 104);
	EXPECT_TRUE(book.isLimitInMap(104));
	EXPECT_TRUE(book.isOrderInMap(103));

	Order order105 = Order(105, false, 20, 100, 105);
	status = book.matchOrder(20, false, 100);
	EXPECT_EQ(status.unitsUnfilled, 0);
	EXPECT_EQ(status.unitsFilled, 20);
	EXPECT_EQ(status.priceFilled, 104*8 + 102*12);
	EXPECT_EQ(book.getHighestBuy(), 100);
	EXPECT_FALSE(book.isOrderInMap(12));
	EXPECT_FALSE(book.isLimitInMap(104));
	EXPECT_FALSE(book.isOrderInMap(9));
	EXPECT_FALSE(book.isLimitInMap(102));

	Order order106 = Order(106, false, 15, 0, 106);
	status = book.matchOrder(15, false, 0);
	EXPECT_EQ(status.unitsUnfilled, 0);
	EXPECT_EQ(status.unitsFilled, 15);
	EXPECT_EQ(status.priceFilled, 100*13 + 98*2);
	EXPECT_EQ(book.getHighestBuy(), 98);
	EXPECT_FALSE(book.isOrderInMap(3));
	EXPECT_FALSE(book.isLimitInMap(100));

	Order order99 = Order(13, true, 4, 99, 13);
	book.addOrder(order99);
	EXPECT_EQ(book.getHighestBuy(), 99);
	EXPECT_TRUE(book.isLimitInMap(99));
	EXPECT_TRUE(book.isOrderInMap(13));
	
}
}

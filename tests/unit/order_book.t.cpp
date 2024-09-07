#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <order_book.h>

namespace{

class OrderBookTest: public testing::Test{
	protected:
		OrderBookTest(){
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
	Order a{1, true, 3, 100, 1};
	Order b{2, true, 5, 100, 2};
	Order c{3, true, 5, 100, 3};
	Order d{4, true, 8, 95, 4};
	Order e{5, true, 23, 95, 5};
	Order f{6, true, 3, 97, 6};
	Order g{7, true, 7, 105, 7};
	Order h{8, true, 6, 102, 8};
	Order i{9, true, 12, 102, 9};
	Order j{10, true, 10, 98, 10};
};

TEST_F(OrderBookTest, testRemoveUnits){
}

}

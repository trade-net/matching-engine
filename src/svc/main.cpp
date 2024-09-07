#include <order_book.h>
#include <order.h>

int main(){
	OrderBook aapl;
	Order a(1, true, 3, 100, 1);
	Order b(2, true, 5, 100, 2);
	Order c(3, true, 5, 100, 3);
	Order d(4, true, 8, 95, 4);
	Order e(5, true, 2, 95, 5);
	Order f(6, true, 3, 97, 6);
	Order g(7, true, 7, 105, 7);
	Order h(8, true, 6, 102, 8);
	Order i(9, true, 2, 105, 9);

	aapl.addOrder(a);
	aapl.addOrder(b);
	aapl.addOrder(c);
	aapl.addOrder(d);
	aapl.addOrder(e);
	aapl.addOrder(f);
	aapl.addOrder(g);
	aapl.addOrder(h);
	aapl.addOrder(i);
}

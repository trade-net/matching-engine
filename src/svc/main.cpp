#include <order_book.h>
#include <order_request.h>
#include <order.h>
#include <threadpool.h>
#include <memory>
#include <iostream>

int main(){
	BookManager manager(4);

	std::vector<OrderRequest> requests;
	requests.emplace_back(1, true, 10, 100, 1, "AAPL");
	requests.emplace_back(2, false, 12, 100, 1, "AAPL");
	requests.emplace_back(3, true, 10, 100, 1, "AAPL");
	requests.emplace_back(4, true, 10, 100, 1, "AAPL");
	requests.emplace_back(5, true, 10, 100, 1, "AAPL");
	requests.emplace_back(6, true, 10, 100, 1, "AAPL");
	requests.emplace_back(7, true, 10, 100, 1, "AAPL");
	requests.emplace_back(8, true, 10, 100, 1, "AAPL");
	requests.emplace_back(9, true, 10, 100, 1, "AAPL");
	requests.emplace_back(10, true, 10, 100, 1, "AAPL");
	requests.emplace_back(11, true, 10, 100, 1, "AAPL");
	requests.emplace_back(12, true, 10, 100, 1, "AAPL");
	requests.emplace_back(13, true, 10, 100, 1, "AAPL");

	ThreadPool pool(4);
	for(size_t i = 0; i < 100; ++i){
		pool.enqueue(
			i%4,
			[i]()
			{
				std::cout << "Task " << i << " is running on thread " << std::this_thread::get_id() << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		);
	}

	return 0;
}

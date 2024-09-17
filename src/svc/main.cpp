#include <order_book.h>
#include <order.h>
#include <threadpool.h>
#include <memory>
#include <iostream>

int main(){
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

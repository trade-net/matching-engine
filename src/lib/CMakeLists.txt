add_library(sxchangelib
	matcher_request_processor.cpp
	order_book.cpp
	limit.cpp
	partitioned_threadpool.cpp
)

find_package(networklib REQUIRED)

target_link_libraries(sxchangelib PUBLIC
	pthread
	networklib
	protolib
)

target_include_directories(sxchangelib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	/usr/local/include/networklib
)

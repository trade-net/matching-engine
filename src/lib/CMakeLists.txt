add_library(matcherlib
	matcher_request_processor.cpp
	order_book.cpp
	limit.cpp
	partitioned_threadpool.cpp
)

find_package(networklib REQUIRED)

target_link_libraries(matcherlib PUBLIC
	pthread
	networklib
	protolib
)

message(STATUS, ${networklib_INCLUDE_DIR})

target_include_directories(matcherlib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

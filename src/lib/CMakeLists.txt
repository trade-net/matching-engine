add_library(sxchangelib
	book_manager.cpp
	order_book.cpp
	limit.cpp
	order.cpp
	threadpool.cpp
)

target_link_libraries(sxchangelib PRIVATE
	pthread
)

target_include_directories(sxchangelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

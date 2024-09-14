add_library(sxchangelib
	book_manager.cpp
	order_book.cpp
	limit.cpp
	order.cpp
)

target_include_directories(sxchangelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

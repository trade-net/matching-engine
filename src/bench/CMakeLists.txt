add_executable(order_book_bench
	order_book_bench.cpp
)

target_link_libraries(order_book_bench PRIVATE
	matcherlib
)

install(TARGETS order_book_bench RUNTIME COMPONENT bench)

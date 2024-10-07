add_executable(matcher.t
	order_book.t.cpp
)

target_link_libraries(matcher.t PRIVATE
	gtest
	gtest_main
	rt
	pthread
	gmock
	matcherlib
)

add_test(
	NAME matcher.t
	COMMAND matcher.t
)

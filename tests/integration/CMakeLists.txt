find_package(GTest REQUIRED)

add_executable(matcher.t
	order_book.t.cpp
)

target_link_libraries(matcher.t PRIVATE
	GTest::gtest
	GTest::gtest_main
	GTest::gmock
	pthread
	matcherlib
)

add_test(
	NAME matcher.t
	COMMAND matcher.t
)

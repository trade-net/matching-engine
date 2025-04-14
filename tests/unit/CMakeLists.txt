set(UNIT_TEST_SOURCES
    limit.t.cpp
    order_book.t.cpp
    partitioned_threadpool.t.cpp
)

# Find Google Test
find_package(GTest REQUIRED)

# Create an executable for unit tests
add_executable(unit_tests ${UNIT_TEST_SOURCES})

# Link the unit tests with necessary libraries
target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    matcherlib
)

# Include Google Test directories
target_include_directories(unit_tests PRIVATE ${GTEST_INCLUDE_DIRS})

# Add unit tests to CTest
add_test(NAME UnitTests COMMAND unit_tests)

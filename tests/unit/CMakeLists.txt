add_executable(sxchange.t
	remove_units.t.cpp
)

target_link_libraries(sxchange.t PRIVATE
	gtest
	gtest_main
	rt
	pthread
	gmock
	sxchangelib
)

add_test(
	NAME sxchange.t
	COMMAND sxchange.t
)

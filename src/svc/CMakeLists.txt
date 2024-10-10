add_executable(matcher
	main.cpp
)

target_link_libraries(matcher PRIVATE
	matcherlib
)

install(TARGETS matcher RUNTIME COMPONENT matcher)

add_executable(client
	client.cpp
)

find_package(networklib REQUIRED)

target_link_libraries(client PRIVATE
	protolib	
	pthread
	networklib
)

install(TARGETS client RUNTIME COMPONENT client)


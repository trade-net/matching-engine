add_executable(matcher
	main.cpp
)

target_link_libraries(matcher PRIVATE
	matcherlib
)

install(TARGETS matcher RUNTIME COMPONENT matcher)

add_executable(client
	tcp_client.cpp
)

target_link_libraries(client PRIVATE
	protolib	
	pthread
)

install(TARGETS client RUNTIME COMPONENT client)


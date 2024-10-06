add_executable(sxchange
	main.cpp
)

target_link_libraries(sxchange PRIVATE
	sxchangelib
)

install(TARGETS sxchange RUNTIME COMPONENT sxchange)

add_executable(client
	tcp_client.cpp
)

target_link_libraries(client PRIVATE
	protolib	
	pthread
)

install(TARGETS client RUNTIME COMPONENT client)


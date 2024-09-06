add_executable(sxchange
	main.cpp
)

target_link_libraries(sxchange PRIVATE
	sxchangelib
)

install(TARGETS sxchange RUNTIME COMPONENT sxchange)


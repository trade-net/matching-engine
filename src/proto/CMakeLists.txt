find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})

set(PROTO_FILES
	order_request.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_library(protolib 
	${PROTO_SRCS}
)

target_link_libraries(protolib PRIVATE
	protobuf
)

target_include_directories(protolib PUBLIC 
	${CMAKE_CURRENT_BINARY_DIR}
)


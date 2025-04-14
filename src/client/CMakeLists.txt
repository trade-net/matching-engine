cmake_minimum_required(VERSION 3.10)

project(automated_client)

# Add executable
add_executable(automated_client automated_client.cpp)

# Link required libraries
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(automated_client PRIVATE nlohmann_json::nlohmann_json)

# Include directories for protobuf and tcp_client
target_include_directories(automated_client PRIVATE ../svc)
target_link_libraries(automated_client PRIVATE protolib networklib)

# Copy orders.json to the build directory
add_custom_command(
    TARGET automated_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/client/orders.json
        $<TARGET_FILE_DIR:automated_client>
)

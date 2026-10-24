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

# Copy orders.json next to the binary. configure_file re-runs CMake whenever the
# source changes, so the copy never goes stale (a POST_BUILD copy only ran on relink).
configure_file(
    ${CMAKE_SOURCE_DIR}/src/client/orders.json
    ${CMAKE_CURRENT_BINARY_DIR}/orders.json
    COPYONLY
)

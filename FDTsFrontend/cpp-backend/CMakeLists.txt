cmake_minimum_required(VERSION 3.8)
project(cpp-backend)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

add_subdirectory(Simple-WebSocket-Server)
add_subdirectory(json)

include_directories(/usr/local/include)


add_executable(main ${SOURCE_FILES})
target_link_libraries(main boost_system)
target_link_libraries(main ssl)
target_link_libraries(main crypto)
target_link_libraries(main pthread)

target_link_libraries(main nlohmann_json::nlohmann_json)
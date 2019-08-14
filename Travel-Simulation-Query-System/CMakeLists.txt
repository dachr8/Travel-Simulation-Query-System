project(Travel-Simulation-Query-System)

set(CMAKE_LEGACY_CYGWIN_WIN32 0)
add_definitions(-D_WIN32)

add_subdirectory(boost-cmake)
add_subdirectory(openssl-cmake)
add_subdirectory(Simple-WebSocket-Server)
add_subdirectory(json)

add_executable(main main.cpp PassengerTable.cpp Permutations.cpp TimeTable.cpp logger.cpp ws_server.cpp transmission_protocol.cpp)
target_link_libraries(main pthread)

target_link_libraries(main boost_system)
target_link_libraries(main ssl)
target_link_libraries(main crypto)

target_link_libraries(main nlohmann_json::nlohmann_json)

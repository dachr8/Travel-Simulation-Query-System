project(Travel-Simulation-Query-System)

add_subdirectory(Simple-WebSocket-Server)
add_subdirectory(json)

add_executable(main main.cpp PassengerTable.cpp Permutations.cpp TimeTable.cpp logger.cpp ws_server.cpp transmission_protocol.cpp)
target_link_libraries(main pthread)

target_link_libraries(main boost_system)
target_link_libraries(main ssl)
target_link_libraries(main crypto)

target_link_libraries(main nlohmann_json::nlohmann_json)

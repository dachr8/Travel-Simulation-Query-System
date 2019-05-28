project(Travel-Simulation-Query-System)

add_executable(main main.cpp PassengerTable.cpp Permutations.cpp TimeTable.cpp logger.cpp)
target_link_libraries(main pthread)
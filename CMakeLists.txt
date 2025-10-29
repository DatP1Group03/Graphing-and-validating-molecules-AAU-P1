cmake_minimum_required(VERSION 4.0)
project(Projekt_kode C)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c)

add_subdirectory(Databehandling)

enable_testing()
target_link_libraries(main bfs_matrix m)
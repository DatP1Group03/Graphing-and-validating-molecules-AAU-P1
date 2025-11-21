cmake_minimum_required(VERSION 4.0)
project(Projekt_kode C)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c)

add_subdirectory(Databehandling)
add_subdirectory(Input)
include(cmake/mtest.cmake)

enable_testing()
target_link_libraries(main bfs_matrix dfs_matrix validation valence_check m)
add_subdirectory(test)
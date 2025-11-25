cmake_minimum_required(VERSION 3.20)
project(P1 C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(Input)
add_subdirectory("SMILES NODE")
add_subdirectory(Databehandling)

add_executable(main
        main.c
)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/Input)
target_link_libraries(main PRIVATE SMILES_NODE validation bfs_matrix m)

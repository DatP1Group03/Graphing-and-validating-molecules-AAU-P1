cmake_minimum_required(VERSION 3.20)
project(P1 C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(Input)
add_subdirectory(SMILES_NODE)
add_subdirectory(Databehandling)
add_subdirectory(TEST_SMILES_NODE)

add_executable(main SMILES_NODE/main.c)

target_link_libraries(main SMILES_NODE validation bfs_matrix)




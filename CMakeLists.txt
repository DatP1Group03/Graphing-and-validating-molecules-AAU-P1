cmake_minimum_required(VERSION 4.0)
project(Projekt_kode C)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c
        "SMILES NODE/SMILESNODE.c"
        "test for SMILES NODE/test_SMILESNODE.c"
        "SMILES NODE/SMILESNODE.h"
        "SMILES NODE/main.c")

add_subdirectory(Databehandling)
add_subdirectory(Input)
include(cmake/mtest.cmake)

add_subdirectory(adjacency_matrix)
target_link_libraries(main Adjacency_matrix)

enable_testing()
add_subdirectory(test)

set(RAYLIB_SRC_DIR "${CMAKE_SOURCE_DIR}/external/raylib")

add_subdirectory(${RAYLIB_SRC_DIR})
target_include_directories(main PUBLIC
        ${RAYLIB_SRC_DIR}/src
)
target_link_libraries(main
        raylib
        opengl32
        gdi32
        winmm
)
target_link_libraries(main bfs_matrix dfs_matrix validation valence_check m)
add_subdirectory(test)

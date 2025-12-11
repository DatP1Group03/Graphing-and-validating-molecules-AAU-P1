cmake_minimum_required(VERSION 3.10)
project(Projekt_kode C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(external/raylib)

add_subdirectory(Databehandling)       # bfs_matrix, dfs_matrix, valence_check
add_subdirectory(Input)                # validation
add_subdirectory(adjacency_matrix)     # Adjacency_matrix
add_subdirectory(GUI)
add_subdirectory(toxicphore)
#har været nødt til at kommenterer det ud fordi jeg på linux skal bruge find_package
#set(RAYLIB_SRC_DIR "${CMAKE_SOURCE_DIR}/external/raylib")

#add_subdirectory(${RAYLIB_SRC_DIR})



add_executable(main
        main.c
)

target_include_directories(main PUBLIC
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/Input
  ${CMAKE_SOURCE_DIR}/Databehandling
  ${CMAKE_SOURCE_DIR}/adjacency_matrix
)

target_link_libraries(main PRIVATE
  bfs_matrix
  dfs_matrix
  validation
  valenceCheck
  Adjacency_matrix
  toxicphore
  gui
  raylib
)

# Test
include(cmake/mtest.cmake)
enable_testing()
add_subdirectory(test)

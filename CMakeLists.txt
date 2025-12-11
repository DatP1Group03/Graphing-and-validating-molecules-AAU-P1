cmake_minimum_required(VERSION 3.10)
project(Projekt_kode C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(Databehandling)       # bfs_matrix, dfs_matrix, valence_check
add_subdirectory(Input)                # validation
add_subdirectory(adjacency_matrix)     # Adjacency_matrix
add_subdirectory(GUI)
add_subdirectory(toxicphore)

#har været nødt til at kommenterer det ud fordi jeg på linux skal bruge find_package
#set(RAYLIB_SRC_DIR "${CMAKE_SOURCE_DIR}/external/raylib")

#add_subdirectory(${RAYLIB_SRC_DIR})
find_package(raylib REQUIRED)   # bruger  raylib, der er installeret med pacman



add_executable(main
        main.c
)


target_include_directories(main PUBLIC
        ${CMAKE_SOURCE_DIR}/external
        ${RAYLIB_SRC_DIR}/src
        ${CMAKE_SOURCE_DIR}/Input
        ${CMAKE_SOURCE_DIR}/Databehandling
        ${CMAKE_SOURCE_DIR}/adjacency_matrix
)
# KOble Libraries
target_link_libraries(main
        bfs_matrix
        dfs_matrix
        validation
        valenceCheck
        drawTextBoxed
        Adjacency_matrix
        gui
	toxicphore
        raylib
        #opengl32 er windows-libs
        #gdi32
        #winmm
)

# Test
include(cmake/mtest.cmake)
enable_testing()
add_subdirectory(test)

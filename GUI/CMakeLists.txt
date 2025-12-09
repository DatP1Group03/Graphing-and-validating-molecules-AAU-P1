add_library(gui gui.c)

find_package(raylib REQUIRED)   # bruger  raylib, der er installeret med pacman
add_subdirectory(Graph_representation)

target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gui
        PUBLIC
        bfs_matrix
        dfs_matrix
        valenceCheck
        Adjacency_matrix
        validation
	toxicphore
		Graph_representation
        raylib
        m
)

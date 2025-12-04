add_library(gui gui.c)

find_package(raylib REQUIRED)   # bruger  raylib, der er installeret med pacman

target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gui
        PUBLIC
        bfs_matrix
        dfs_matrix
        valenceCheck
        Adjacency_matrix
        validation
	toxicphore
        raylib
        m
)

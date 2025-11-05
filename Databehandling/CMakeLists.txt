add_library(bfs_matrix bfs_matrix.c)
add_library(valence_check valence_check.c)
target_link_libraries(bfs_matrix m)
target_include_directories(bfs_matrix PUBLIC .)


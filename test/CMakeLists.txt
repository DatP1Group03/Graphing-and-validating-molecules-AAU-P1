
add_executable(test_bfs test_BFS_matrix.c)
target_link_libraries(test_bfs bfs_matrix mtest)

discover_tests(test_bfs)

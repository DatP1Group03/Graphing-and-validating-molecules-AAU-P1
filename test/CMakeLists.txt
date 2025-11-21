

add_executable(test_adj test_adjacency_matrix.c test_DFS test_DFS_matrix.c)
target_link_libraries(test_adj Adjacency_matrix test_DFS dfs_matrix mtest)

discover_tests(test_bfs test_DFS)

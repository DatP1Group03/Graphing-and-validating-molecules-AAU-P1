


add_executable(test_DFS test_DFS_matrix.c)
target_link_libraries(test_DFS dfs_matrix mtest)

discover_tests(test_DFS)

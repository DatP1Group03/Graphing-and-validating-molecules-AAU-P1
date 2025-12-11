
add_executable(test_bfs test_BFS_matrix.c)
target_link_libraries(test_bfs bfs_matrix mtest raylib SMILES_NODE)
discover_tests(test_bfs)


add_executable(test_DFS test_DFS_matrix.c)
target_link_libraries(test_DFS dfs_matrix mtest raylib)
discover_tests(test_DFS)

add_executable(test_toxicphere test_toxicphere.c) 
target_link_libraries(test_toxicphere dfs_matrix mtest Adjacency_matrix toxicphore)
discover_tests(test_toxicphere)

add_executable(test_adj test_adjacency_matrix.c)
target_link_libraries(test_adj Adjacency_matrix mtest)
discover_tests(test_adj)

add_executable(test_val valence_test.c )
target_link_libraries(test_val mtest valenceCheck Adjacency_matrix validation)
discover_tests(test_val)

add_executable(test_input input_validation_test.c )
target_link_libraries(test_input mtest validation)
discover_tests(test_input)

add_executable(test_smilesnode test_smilesnode.c)
target_link_libraries(test_smilesnode mtest SMILES_NODE validation)
discover_tests(test_smilesnode)


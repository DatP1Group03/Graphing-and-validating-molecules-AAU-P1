#include "mtest.h"
#include "dfs_matrix.h"

/*
 * 1) Connected graph.
 * Verifies DFS traversal on a connected graph starting from node 0.
 * The test checks that all reachable nodes are visited exactly once
 * and that the traversal order follows depth-first exploration with backtracking.
 */
TEST_CASE(dfs_connected_graph, {
	int cycles[6][2]; 
	int cycle_count = 0; 
	int n = 6;
	const int adj[6][6] = {
		{0,1,0,0,0,0},
		{1,0,1,0,1,0},
		{0,1,0,1,0,1},
		{0,0,1,0,0,0},
		{0,1,0,0,0,0},
		{0,0,1,0,0,0}
	};
	int dfs[6];
	int visited[6]= {0};
	int parent[6];
	int count = dfs_matrix(0, n, adj, dfs, visited, parent, cycles, &cycle_count, 0);
	const int expected[] = {0, 1, 2, 3, 5, 4};
	for (int i = 0; i < n; i++ ) {
		CHECK_EQ_INT(expected[i], dfs[i]);
	}
})

/*
 * 1a) Connected graph with double bonds (edge weight = 2).
 * Ensures that DFS traversal is unaffected by bond multiplicity and
 * that traversal order matches the unweighted version of the graph.
 */
TEST_CASE(dfs_connected_graph_dobbeltbindinger, {
	int n = 6;
	const int adj[6][6] = {
		{0,2,0,0,0,0},
		{2,0,2,0,2,0},
		{0,2,0,2,0,2},
		{0,0,2,0,0,0},
		{0,2,0,0,0,0},
		{0,0,2,0,0,0}
	};
	int cycles[6][2]; 
	int cycle_count = 0; 
	int dfs[6];
	int visited[6]= {0};
	int parent[6];
	int count = dfs_matrix(0, n, adj, dfs, visited, parent, cycles, &cycle_count, 0);
	const int expected[] = {0, 1, 2, 3, 5, 4};
	for (int i = 0; i < n; i++ ) {
		CHECK_EQ_INT(expected[i], dfs[i]);
	}
})

/*
 * 2) Disconnected graph.
 * The graph consists of two separate components.
 * Starting DFS from node 0 should only visit nodes in the same component (0,1,2)
 * and must not include nodes from the disconnected component.
 */
TEST_CASE(dfs_disconnected_graph_from_0, {
	int n = 5;
	const int adj[5][5] = {
		{0,1,0,0,0},
		{1,0,1,0,0},
		{0,1,0,0,0},
		{0,0,0,0,1},
		{0,0,0,1,0}
	};
	int cycles[6][2]; 
	int cycle_count = 0; 
	int dfs[6];
	int visited[6] = {0};
	int parent[6];
	int count = dfs_matrix(0, n, adj, dfs, visited, parent,cycles, &cycle_count, 0);
	const int expected[] = {0, 1, 2};
	for (int i = 0; i < 3; i++ ) {
		CHECK_EQ_INT(expected[i], dfs[i]);
	}
})

/*
 * 3) Single-node graph.
 * Verifies that DFS correctly handles the minimal case where the graph
 * consists of only one node.
 */
TEST_CASE(dfs_single_node, {
	int n = 1;
	const int adj[1][1] = {
		{0}
	};
	int cycles[6][2]; 
	int cycle_count = 0; 
	int dfs[6];
	int visited[6] = {0};
	int parent[6];
	int count = dfs_matrix(0, n, adj, dfs, visited, parent, cycles, &cycle_count, 0);
	const int expected[] = {0};
	CHECK_EQ_INT(expected[0], dfs[0]);
})

/*
 * 4) Graph with no edges.
 * All nodes are isolated. Starting DFS from node 2 should only visit node 2.
 * This test ensures DFS does not traverse non-existent edges.
 */
TEST_CASE(dfs_no_edges_from_2, {
	int n = 4;
	const int adj[4][4] = {
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	};
	int cycles[6][2]; 
	int cycle_count = 0; 
	int dfs[6];
	int visited[6] = {0};
	int parent[6];
	int count = dfs_matrix(2, n, adj, dfs, visited, parent,cycles, &cycle_count, 0);
	const int expected[] = {2};
	CHECK_EQ_INT(expected[0], dfs[0]);
})

/*
 * 5) Linear graph (0-1-2-3-4), starting from the middle.
 * Verifies correct DFS traversal order when starting from a non-endpoint node,
 * including proper backtracking behavior.
 */
TEST_CASE(dfs_line_graph_start_middle, {
	int n = 5;
	const int adj[5][5] = {
		{0,1,0,0,0},
		{1,0,1,0,0},
		{0,1,0,1,0},
		{0,0,1,0,1},
		{0,0,0,1,0}
	};
	int cycles[6][2]; 
	int cycle_count = 0; 
	int dfs[5];
	int visited[5] = {0};
	int parent[5];
	int count = dfs_matrix(2, n, adj, dfs, visited, parent, cycles, &cycle_count, 0);
	const int expected[] = {2, 1, 0, 3, 4};
	for (int i = 0; i < n; i++ ) {
		CHECK_EQ_INT(expected[i], dfs[i]);
	}
})

/*
 * 6) Cyclic graph (0-1-2-3-0).
 * Ensures that DFS correctly handles cycles by marking visited nodes
 * and does not enter infinite recursion.
 */
TEST_CASE(dfs_cycle_graph, {
	int n = 4;
	const int adj[4][4] = {
		{0,1,0,1},
		{1,0,1,0},
		{0,1,0,1},
		{1,0,1,0}
	};
	int cycles[6][2]; 
	int cycle_count = 0; 
	int dfs[4];
	int visited[4] = {0};
	int parent[4];
	int count = dfs_matrix(0, n, adj, dfs, visited, parent, cycles, &cycle_count, 0);
	const int expected[] = {0, 1, 2, 3};
	for (int i = 0; i < n; i++ ) {
		CHECK_EQ_INT(expected[i], dfs[i]);
	}
})

/*
 * 6a) Cyclic graph with double bonds.
 * Verifies that DFS handles cycles correctly even when edges represent
 * double bonds, and that each node is visited only once.
 */
TEST_CASE(dfs_cycle_graph_dobbeltbindinger, {
	int n = 4;
	const int adj[4][4] = {
		{0,2,0,2},
		{2,0,2,0},
		{0,2,0,2},
		{2,0,2,0}
	};

	int cycles[6][2]; 
	int cycle_count = 0; 
	int dfs[4];
	int visited[4] = {0};
	int parent[4];
	int count = dfs_matrix(0, n, adj, dfs, visited, parent, cycles, &cycle_count, 0);

	const int expected[] = {0, 1, 2, 3};
	for (int i = 0; i < n; i++ ) {
		CHECK_EQ_INT(expected[i], dfs[i]);
	}
})

MAIN_RUN_TESTS(
	dfs_connected_graph,
	dfs_disconnected_graph_from_0,
	dfs_single_node,
	dfs_no_edges_from_2,
	dfs_line_graph_start_middle,
	dfs_cycle_graph,
	dfs_connected_graph_dobbeltbindinger,
	dfs_cycle_graph_dobbeltbindinger
) 

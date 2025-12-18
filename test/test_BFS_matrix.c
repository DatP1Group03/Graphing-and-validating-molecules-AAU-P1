#include "mtest.h"
#include "bfs_matrix.h"

/*
 * 1) Testing for connected graphs
 * Expected BFS traversal from node 0: 0 1 2 4 3 5
 */
TEST_CASE(bfs_connected_graph, {
    int n = 6;
    const int adj[6][6] = {
        {0,1,0,0,0,0},
        {1,0,1,0,1,0},
        {0,1,0,1,0,1},
        {0,0,1,0,0,0},
        {0,1,0,0,0,0},
        {0,0,1,0,0,0}
    };

    int bfs[6];
    int count = bfs_matrix(n, adj, 0, bfs);

    const int expected[] = {0, 1, 2, 4, 3, 5};
    CHECK_EQ_INT(expected[0], bfs[0]);
    CHECK_EQ_INT(expected[1], bfs[1]);
    CHECK_EQ_INT(expected[2], bfs[2]);
    CHECK_EQ_INT(expected[3], bfs[3]);
    CHECK_EQ_INT(expected[4], bfs[4]);
    CHECK_EQ_INT(expected[5], bfs[5]);
})

// Tests BFS traversal on a connected graph with double bonds.
// Verifies that BFS correctly traverses all reachable nodes,
// ignoring bond order values and using only connectivity.
TEST_CASE(bfs_connected_graph_dobbeltbindinger, {
    int n = 6;
    const int adj[6][6] = {
        {0,2,0,0,0,0},
        {2,0,2,0,2,0},
        {0,2,0,2,0,2},
        {0,0,2,0,0,0},
        {0,2,0,0,0,0},
        {0,0,2,0,0,0}
    };

    int bfs[6];
    int count = bfs_matrix(n, adj, 0, bfs);

    const int expected[] = {0, 1, 2, 4, 3, 5};
    CHECK_EQ_INT(expected[0], bfs[0]);
    CHECK_EQ_INT(expected[1], bfs[1]);
    CHECK_EQ_INT(expected[2], bfs[2]);
    CHECK_EQ_INT(expected[3], bfs[3]);
    CHECK_EQ_INT(expected[4], bfs[4]);
    CHECK_EQ_INT(expected[5], bfs[5]);
})

/*
 * Not connected graph (it is divided into two components 
	* 0-1-2 are connected, 3-4 is a separate component
	* From src=0 we must see only 0,1,2 in our traversal. We do not expect 4 & 5 to show. 
 */
TEST_CASE(bfs_disconnected_graph_from_0, {
    int n = 5;
    const int adj[5][5] = {
        {0,1,0,0,0},
        {1,0,1,0,0},
        {0,1,0,0,0},
        {0,0,0,0,1},
        {0,0,0,1,0}
    };

    int bfs[5];
    int count = bfs_matrix(n, adj, 0, bfs);

    const int expected[] = {0, 1, 2};
    CHECK_EQ_INT(expected[0], bfs[0]);
    CHECK_EQ_INT(expected[1], bfs[1]);
    CHECK_EQ_INT(expected[2], bfs[2]);
})

/*
 * Single-node graph (n = 1).
 * Start i node 0 → only connected node is 0.
 */
TEST_CASE(bfs_single_node, {
    int n = 1;
    const int adj[1][1] = {
        {0}
    };

    int bfs[1];
    int count = bfs_matrix(n, adj, 0, bfs);

    const int expected[] = {0};
    CHECK_EQ_INT(expected[0], bfs[0]);
})

/*
 * 4) Graph without edges
	* All nodes are "isolated". If we start from node, we only visits node 2. 
 */
TEST_CASE(bfs_no_edges_from_2, {
    int n = 4;
    const int adj[4][4] = {
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
    };

    int bfs[4];
    int count = bfs_matrix(n, adj, 2, bfs);

    const int expected[] = {2};
    CHECK_EQ_INT(expected[0], bfs[0]);
})

/*
	* Linear graph 0-1-2-3-4, if we start from node 2 what do we get?
	* We expect the BFS traversal from node 2 to be: 2-1-3-0-4 
 */
TEST_CASE(bfs_line_graph_start_middle, {
    int n = 5;
    const int adj[5][5] = {
        {0,1,0,0,0},
        {1,0,1,0,0},
        {0,1,0,1,0},
        {0,0,1,0,1},
        {0,0,0,1,0}
    };

    int bfs[5];
    int count = bfs_matrix(n, adj, 2, bfs);

    const int expected[] = {2, 1, 3, 0, 4};
    CHECK_EQ_INT(expected[0], bfs[0]);
    CHECK_EQ_INT(expected[1], bfs[1]);
    CHECK_EQ_INT(expected[2], bfs[2]);
    CHECK_EQ_INT(expected[3], bfs[3]);
    CHECK_EQ_INT(expected[4], bfs[4]);
})

/*
 * Graph with a cycle (cycle is 0-1-2-3-0)
	* We test that we only still visits one node once.
	* We expect BFS traversal from 0 to be: 0,1,3,2
 */
TEST_CASE(bfs_cycle_graph, {
    int n = 4;
    const int adj[4][4] = {
        {0,1,0,1},
        {1,0,1,0},
        {0,1,0,1},
        {1,0,1,0}
    };

    int bfs[4];
    int count = bfs_matrix(n, adj, 0, bfs);

    const int expected[] = {0, 1, 3, 2};
    CHECK_EQ_INT(expected[0], bfs[0]);
    CHECK_EQ_INT(expected[1], bfs[1]);
    CHECK_EQ_INT(expected[2], bfs[2]);
    CHECK_EQ_INT(expected[3], bfs[3]);
})

// Tests BFS traversal on a cyclic graph with double bonds (represented by weight 2).
// The test ensures that BFS correctly visits each node exactly once and handles cycles
// without getting stuck or revisiting nodes.
TEST_CASE(bfs_cycle_graph_dobbeltbindinger, {
    int n = 4;
    const int adj[4][4] = {
        {0,2,0,2},
        {2,0,2,0},
        {0,2,0,2},
        {2,0,2,0}
    };

    int bfs[4];
    int count = bfs_matrix(n, adj, 0, bfs);

    const int expected[] = {0, 1, 3, 2};
    CHECK_EQ_INT(expected[0], bfs[0]);
    CHECK_EQ_INT(expected[1], bfs[1]);
    CHECK_EQ_INT(expected[2], bfs[2]);
    CHECK_EQ_INT(expected[3], bfs[3]);
})

MAIN_RUN_TESTS(
    bfs_connected_graph,
    bfs_disconnected_graph_from_0,
    bfs_single_node,
    bfs_no_edges_from_2,
    bfs_line_graph_start_middle,
    bfs_cycle_graph,
    bfs_connected_graph_dobbeltbindinger,
    bfs_cycle_graph_dobbeltbindinger
)

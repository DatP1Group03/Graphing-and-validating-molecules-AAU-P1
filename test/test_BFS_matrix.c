#include "mtest.h"
#include "bfs_matrix.h"

/*
 * 1) Sammenhængende graf
 * Forventet BFS fra node 0: 0 1 2 4 3 5
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
 * 2) Ikke-sammenhængende graf: to komponenter
 * 0-1-2 er forbundet, 3-4 er en separat komponent.
 * Fra src=0 må vi kun se 0,1,2., vi forventer altså ikke at 4 og 5 er med!
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
 * 3) Single-node graf (n = 1).
 * Start i node 0 → eneste node er 0.
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
 * 4) Graf uden kanter.
 * Alle noder er isolerede. Starter vi i node 2, skal vi kun besøge 2.
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
 * 5) Lineær graf 0-1-2-3-4, start i midten (2).
 * BFS fra 2: 2,1,3,0,4 (fordi vi kigger naboer i rækkefølgen v = 0..4).
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
 * 6) Graf med cyklus 0-1-2-3-0
 * Vi tester at vi stadig kun besøger hver node én gang.
 * Med nabo-iterationen v=0..3 bliver BFS fra 0: 0,1,3,2
 * (0 ser 1 og 3 → enqueue [1,3]; derefter 1 ser 2; 3 ser også 2, men den er besøgt).
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
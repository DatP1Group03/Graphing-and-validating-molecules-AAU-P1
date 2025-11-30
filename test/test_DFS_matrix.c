#include "mtest.h"
#include "dfs_matrix.h"

/*
 * 1) Sammenhængende graf
 * Forventet DFS fra node 0: 0 -> 1 -> 2 -> 3 -> 5 -> backtrack -> 2 -> backtrack -> 1 -> 4
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
 * 2) Ikke-sammenhængende graf: to komponenter
 * 0-1-2 er forbundet, 3-4 er en separat komponent.
 * Fra src=0 må vi kun se 0,1,2., vi forventer altså ikke at 4 og 5 er med!
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
 * 3) Single-node graf (n = 1).
 * Start i node 0 → eneste node er 0.
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
 * 4) Graf uden kanter.
 * Alle noder er isolerede. Starter vi i node 2, skal vi kun besøge 2.
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
 * 5) Lineær graf 0-1-2-3-4, start i midten (2).
 * BFS fra 2: 2,1,3,0,4 (fordi vi kigger naboer i rækkefølgen v = 0..4).
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
 * 6) Graf med cyklus 0-1-2-3-0
 * Vi tester at vi stadig kun besøger hver node én gang.
 * Med nabo-iterationen v=0..3 bliver BFS fra 0: 0,1,3,2
 * (0 ser 1 og 3 → enqueue [1,3]; derefter 1 ser 2; 3 ser også 2, men den er besøgt).
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

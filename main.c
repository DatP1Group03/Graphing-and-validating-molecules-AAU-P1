#include <stdio.h>
#include "bfs_matrix.h"
#include "dfs_matrix.h"

int main(void) {
    int n = 6;
    int adj[6][6] = {
        {0,1,0,0,0,0},  // 0
        {1,0,1,1,1,0},  // 1: naboer -> 0,2,3,4
        {0,1,0,1,0,1},  // 2: naboer -> 1,3,5
        {0,1,1,0,0,0},  // 3: naboer -> 1,2
        {0,1,0,0,0,0},  // 4: nabo -> 1
        {0,0,1,0,0,0}   // 5: nabo -> 2
    };

    int bfs[6];
    int visited_count = bfs_matrix(n, adj, 0, bfs);

    printf("BFS rækkefølge: ");
    for (int i = 0; i < visited_count; i++)
        printf("%d ", bfs[i]);
    printf("\n");

    if (visited_count == n)
        printf("Grafen er sammenhængende fra node 0\n");
    else
        printf("Grafen er IKKE sammenhængende fra node 0\n");




    int dfsmatrixarr[6];
    int visited[6];
    int parent[6];

    int dfs_count = dfs_matrix(0, n, adj, dfsmatrixarr, visited,parent, 0);
    printf("%d \n", dfs_count);

    printf("DFS matrixen:");
    for (int i = 0; i < dfs_count; i++) {
        printf("%d", dfsmatrixarr[i]);
    }

    printf("\n og parent matrixen er:");
    for (int i = 0; i < dfs_count; i++) {
        printf("%d", parent[i]);
    }
    return 0;
}

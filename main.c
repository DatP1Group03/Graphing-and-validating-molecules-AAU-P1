#include <stdio.h>
#include "bfs_matrix.h"
#include "Input/validation.h"
#include "valence_check.h"

#define MaxInput 256


int main(void) {
    int n = 5;
    int adj[5][5] = {
        {0,1,0,0,0},
         {1,0,1,0,0},
         {0,1,0,0,0},
         {0,0,0,0,1},
         {0,0,0,1,0}
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

    return 0;
}
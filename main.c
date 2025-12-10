#include <stdio.h>
#include "bfs_matrix.h"
#include "dfs_matrix.h"
#include "Input/validation.h"
#include "Adjacency_matrix.h"
#include "valence_check.h"




#include "gui.h"








int main(void)
{
    int run = 1;
    while (run == 1) {
        run = runGUI();
    }

/* hvis man vil kører fra terminal	
    int n = 6;
    int adj[6][6] = {
        {0,1,1,0,0,0},  // 0: Naboer -> 1,2
        {1,0,1,0,0,0},  // 1: naboer -> 0,2,
        {1,1,0,1,1,1},  // 2: naboer -> 0,1,3,4 5
        {0,0,1,0,1,0},  // 3: naboer -> 2,4
        {0,0,1,1,0,0},  // 4: nabo -> 2,3
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
    int visited[6] = {0}; //Important that we initial our arrays, for visited to 0, so we know the values of it if there isnt a connection.
    int parent[6] = {-1};  //Important that we initial our arrays, for parent to -1, so we know the values of parent[0] also will be -1.
    // print_DFS_explanation();
	
    int cycle_count = 0; 
    int cycles[6][2]; 
    int dfs_count = dfs_matrix(0, n, adj, dfsmatrixarr, visited,parent,cycles, &cycle_count, 0);
    printf(" >>> DFS finished, returned a list with %d and our number of atom in molecules is %d \n", dfs_count, n);
    */
	return 0;
}




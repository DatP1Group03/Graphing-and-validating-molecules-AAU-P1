#include <stdio.h>
#include "bfs_matrix.h"
#include "dfs_matrix.h"
#include "Input/validation.h"
#include "valence_check.h"

#define MaxInput 256


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

    print_definition_of_BFS();
    printf("Now lets run the algorithm \n");
    int bfs[6];
    int visited_count = bfs_matrix(n, adj, 0, bfs);

    //following is important for print the BFS matrix correctly
    int no_comma_on_first = 1;
    printf("} Final BFS list is: {");

    for (int i = 0; i < visited_count; i++) {
        if (!no_comma_on_first) {
            printf(", ");
        }
        printf("%d", bfs[i]);
        no_comma_on_first = 0;
    }
    printf("}\n");
    // ends here

    if (visited_count == n)
        printf("Grafen er sammenhængende fra node 0\n");
    else
        printf("Grafen er IKKE sammenhængende fra node 0\n");




    int dfsmatrixarr[6];
    int visited[6] = {0}; //Important that we initial our arrays, for visited to 0, so we know the values of it if there isnt a connection.
    int parent[6] = {-1};  //Important that we initial our arrays, for parent to -1, so we know the values of parent[0] also will be -1.
    print_DFS_explanation();
    int dfs_count = dfs_matrix(0, n, adj, dfsmatrixarr, visited,parent, 0);
    printf(" >>> DFS finished, returned a list with %d and our number of atom in molecules is %d \n", dfs_count, n);

    return 0;
}

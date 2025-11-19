#include <stdio.h>
#include "bfs_matrix.h"

int main(void) {
    int n = 6;
    int adj[6][6] = {
        {0,1,0,0,0,0}, // 0
        {1,0,1,0,1,0}, // 1
        {0,1,0,1,0,1}, // 2
        {0,0,1,0,0,0}, // 3
        {0,1,0,0,0,0}, // 4
        {0,0,1,0,0,0}  // 5
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

    return 0;
}
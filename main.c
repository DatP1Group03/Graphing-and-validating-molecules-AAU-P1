#include <stdio.h>
#include "bfs_matrix.h"
#include "Input/validation.h"
#include "valence_check.h"

#define MaxInput 256


int main(void) {
    char input[MaxInput];

    printf("Input smiles: ");
    scanf("%s", input);
    if (validate_smiles(input)) {
        //Kør smiles til adjacency
        //Kør smiles til node
        //if ()//valence_check med (smiles_size, input,matrix(fra adj))
            //Gør noget mere
    }

    /* int matrix[3][3] = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };
    run_valence_check(smiles_size,"CCO",matrix );
     //TEST KODE*/ //
    int test[6][6] = {
        {0,1,0,0,0,0},
        {1,0,1,0,0,0},
        {0,1,0,1,0,0},
        {0,0,1,0,1,0},
        {0,0,0,1,0,1},
        {0,0,0,0,5,0}
    };

    run_valence_check(atom_count,"C-C-C-C-C-C",test);
    int n = 6;
    int adj[6][6] = {
        {0,1,0,0,0,0},
        {1,0,1,0,1,0},
        {0,1,0,1,0,1},
        {0,0,1,0,0,0},
        {0,1,0,0,0,0},
        {0,0,1,0,0,0}
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
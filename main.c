#include <stdio.h>
#include "bfs_matrix.h"
#include "Adjacency_matrix.h"

int main(void) {
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



    char* SMILES = "CN1C=NC2=C1C(=O)N(C)C(=O)N2C";
    int atom_count = get_atom_count(SMILES);
    int Adj[atom_count][atom_count];

    for (int i = 0; i<atom_count; i++ ) {
        for (int j = 0; j<atom_count; j++) {
            Adj[j][i]= 0;
        }
    }

    create_adjacency_matrix(SMILES, atom_count, Adj);

    print_ADJ(Adj,atom_count);


    return 0;
}
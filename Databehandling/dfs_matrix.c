//
// Created by magnus on 14/11/2025.
//

#include "dfs_matrix.h"
#include <stdio.h>
/* startnode - the node you want to start the traversal from
 * adj[][] is the adjacency matrix we are traversing
 * n is the total number of nodes
 * node_t node is the struct that contains each node, what they are connected to and how we got to the node (parent), so we can detect cycle
 * bfsmatrix is a list of the total traversal each node.
 * visited[] is simply where subscript is the node, and 1 represnt that we have visited.
 * count - how many times dfs_matrix has run.
 */
int dfs_matrix(int startnode, int n, const int adj[n][n], int dfsmatrix[], int visited[], int count) {
    // we mark node as visited, by using our matrix visited, we also insert ved node into the bfs matrix for the "list" of traversal.
    visited[startnode] = 1;
    dfsmatrix[count++] = startnode;

    // We are going to run thorugh the row of the adj corresponding to the startnode, and see what the node is connected to.
    for (int i = 0; i < n; i++) {
        if (adj[startnode][i] == 1) {
            printf("%d er en nabo til %d \n", i, startnode );
        }
    }

    // now we are going to explore all adjacent vertices as deep as possible before, and we just do it in nummerical order (as they come in the neighbours array).
    for (int i = 0; i < n; i++) {
        if (adj[startnode][i] == 1 && visited[i] != 1) {
            count = dfs_matrix(i, n, adj, dfsmatrix, visited, count);
        }
    }
    return count;
}
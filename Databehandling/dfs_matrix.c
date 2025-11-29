//
// Created by magnus on 14/11/2025.
//

#include "dfs_matrix.h"
#include <stdio.h>
#include <raylib.h>
#include <string.h>

/* startnode - the node you want to start the traversal from
 * adj[][] is the adjacency matrix we are traversing
 * n is the total number of nodes
 * node_t node is the struct that contains each node, what they are connected to and how we got to the node (parent), so we can detect cycle
 * bfsmatrix is a list of the total traversal each node.
 * visited[] is simply where subscript is the node, and 1 represnt that we have visited.
 * count - how many times dfs_matrix has run.
 */
int dfs_matrix(int startnode, int n, const int adj[n][n], int dfsmatrix[], int visited[], int parent[], int count) {
    // we mark node as visited, by using our matrix visited, we also insert ved node into the bfs matrix for the "list" of traversal.
    printf("We mark: %d as visited and add it to our list. and its neighbours are { ", startnode);
    visited[startnode] = 1;
    dfsmatrix[count++] = startnode;



    // We are going to run thorugh the row of the adj corresponding to the startnode, and see what the node is connected to. simply for learning purposes.
    int no_comma_on_first = 1; // this value is only used to secure that we dont get comma on first. Her we print comma first and then number so we are sured that we dont get comma on the last one.
    for (int i = 0; i < n; i++) {
        if (adj[startnode][i] >= 1) {
            if (!no_comma_on_first) {
                printf(", ");
            }
            printf("%d", i );
            no_comma_on_first = 0;
        }
    }
    printf("}\n");

    // now we are going to explore all adjacent vertices as deep as possible before, and we just do it in nummerical order (as they come in the neighbours array).
    for (int i = 0; i < n; i++) {
        if (adj[startnode][i] >= 1 && visited[i] != 1) {
            printf("we are now going to visit %d. Our DFS list consist of: { ", i);

            int no_comma_on_first = 1;
            for (int j = 0; j < count; j++) {
                if (!no_comma_on_first) {
                    printf(", ");
                }
                printf("%d", dfsmatrix[j]);
                no_comma_on_first = 0;
            }
            printf("}\n");

            parent[i] = startnode;
            count = dfs_matrix(i, n, adj, dfsmatrix, visited, parent, count);
        }
        /* we can check here if there is a cycle! We can do this because we know that if we traverse the neighbors of a node,
         * and we find a neighbor who has already been visited, and that this is not our "parents", i.e. where we came from, then we have a
         * cycle in which we can then go back. We can set this up as the following conditions
         * however, I have added an extra condition, namely that startnode < i, because then it will only be detected once :) otherwise you will
         * make it detect first around to the right, and then also where you go left, i.e. detect 1 --> 3 and then 3 --> 1.
         */
        else if (adj[startnode][i] >= 1 && visited[i] == 1 && i != parent[startnode] && startnode < i) {
            printf("Cycle detected: Node %d (parent %d) has an edge to already visited node %d (parent %d)\n",
            startnode, parent[startnode], i, parent[i]);

        }

        }

    if (parent[startnode] != -1) {
        printf("Since all neighbours of %d are now visited, we backtrack to node %d.\n",
               startnode, parent[startnode]);
    } else {
        printf("Since all neighbours of %d are now visited, DFS from the root is complete here.\n",
               startnode);
    }
    return count;
}

/* hvis man ved at gennemløbe naboerne til en node, finder en nabo som er visited,
 * og som ikke er parent[startnode] (altså den forrige node).
int DFS_cycledetektion(int n, const int adj[][], const int dfsmatrix[], const int visited[], const int parent[]) {
    int node, naboer;

    for (node = 0; node < n; node++) {

    }
}
*/

void print_DFS_explanation() {
    printf("DFS (Stands for Depth-First Search) goes along a path as deeply as possible along each branch before backtracking. \n");
    printf("DFS starts at a chosen node and visits it. \n");
    printf("It then looks at all the neighbours (all nodes it has connection to) of that node and follows the first neighbour it finds. \n");
    printf("From that neighbour, it again looks at its neighbours and continues going forward as long as there is an unvisited neighbour to follow. \n");
    printf("When it reaches a node where all neighbours have already been visited, it backtracks — meaning it goes back to the node it came from, and continues with the next neighbour there.\n");
    printf("DFS continues this pattern of: \n");
    printf("1. go foward to an unvisited neighbour \n");
    printf("2. backtrack when stuck \n");
    printf("3. continue from the previous node \n");
    printf("Until all reachable nodes have been visited \n");
    printf("We are now going to perform DFS on the molecule \n");
}

int dfs_matrix_onlyforgui(int startnode, int n, const int adj[n][n], int dfsmatrix[], int visited[], int parent[], int count, int *pLineHeight) {
    // we mark node as visited, by using our matrix visited, we also insert ved node into the bfs matrix for the "list" of traversal.
    const int baseY = 220;


    visited[startnode] = 1;
    dfsmatrix[count++] = startnode;

    // Tekst: "We mark: X as visited and add it to our list. and its neighbours are { ... }"
    char line[256];
    int offset = snprintf(line, sizeof(line),
                          "We mark: %d as visited and add it to our list. and its neighbours are { ",
                          startnode);

    int no_comma_on_first = 1;
    for (int i = 0; i < n; i++) {
        if (adj[startnode][i] >= 1) {
            offset += snprintf(line + offset,
                               sizeof(line) - offset,
                               "%s%d",
                               (no_comma_on_first ? "" : ", "),
                               i);
            no_comma_on_first = 0;
        }
    }
    snprintf(line + offset, sizeof(line) - offset, " }");

    DrawText(line, 30, baseY + *pLineHeight, 20, BLACK);
    *pLineHeight += 20;

    // 2) Gå igennem naboer – DFS step + forklarende tekst
    for (int i = 0; i < n; i++) {
        if (adj[startnode][i] >= 1 && visited[i] != 1) {
            // Tekst: "we are now going to visit i. Our DFS list consist of: { ... }"
            char dfsListLine[256];
            int off = snprintf(dfsListLine, sizeof(dfsListLine),
                               "We are now going to visit %d. Our DFS list consist of: { ",
                               i);

            int first = 1;
            for (int j = 0; j < count; j++) {
                off += snprintf(dfsListLine + off,
                                sizeof(dfsListLine) - off,
                                "%s%d",
                                (first ? "" : ", "),
                                dfsmatrix[j]);
                first = 0;
            }
            snprintf(dfsListLine + off, sizeof(dfsListLine) - off, " }");

            DrawText(dfsListLine, 30, baseY + *pLineHeight, 20, BLACK);
            *pLineHeight += 20;

            parent[i] = startnode;
            count = dfs_matrix_onlyforgui(i, n, adj, dfsmatrix, visited, parent, count, pLineHeight);
        }
        /* we can check here if there is a cycle! We can do this because we know that if we traverse the neighbors of a node,
         * and we find a neighbor who has already been visited, and that this is not our "parents", i.e. where we came from, then we have a
         * cycle in which we can then go back. We can set this up as the following conditions
         * however, I have added an extra condition, namely that startnode < i, because then it will only be detected once :) otherwise you will
         * make it detect first around to the right, and then also where you go left, i.e. detect 1 --> 3 and then 3 --> 1.
         */
        else if (adj[startnode][i] >= 1 && visited[i] == 1 && i != parent[startnode] && startnode < i) {
            char cycleLine[256];
            snprintf(cycleLine, sizeof(cycleLine),
                     "Cycle detected: Node %d (parent %d) has an edge to already visited node %d (parent %d)",
                     startnode, parent[startnode], i, parent[i]);

            DrawText(cycleLine, 30, baseY + *pLineHeight, 20, RED);
            *pLineHeight += 20;
        }
    }

    // 3) Backtracking-tekst
    if (parent[startnode] != -1) {
        char backLine[256];
        snprintf(backLine, sizeof(backLine),
                 "Since all neighbours of %d are now visited, we backtrack to node %d.",
                 startnode, parent[startnode]);
        DrawText(backLine, 30, baseY + *pLineHeight, 20, DARKGRAY);
        *pLineHeight += 20;
    } else {
        char doneLine[256];
        snprintf(doneLine, sizeof(doneLine),
                 "Since all neighbours of %d are now visited, DFS from the root is complete here.",
                 startnode);
        DrawText(doneLine, 30, baseY + *pLineHeight, 20, DARKGRAY);
        *pLineHeight += 20;
    }
    return count;
}
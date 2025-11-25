//
// Created by magnus on 29.10.2025.
//

#ifndef PROJEKT_KODE_BFS_MATRIX_H
#define PROJEKT_KODE_BFS_MATRIX_H

#include <stddef.h>   // for size_t

// Standard BFS (din oprindelige)
int bfs_matrix(int n, const int adj[n][n], int src, int bfs[]);

// Tekstlig forklaring i terminal (din oprindelige)
void print_definition_of_BFS(void);

//GUI bfs matrix istedet for printf
int bfs_matrix_drawtext_ONLYFORUSEINRAYGUI(int n, const int adj[n][n], int src, int bfs[], int lineheight);

#endif // PROJEKT_KODE_BFS_MATRIX_H

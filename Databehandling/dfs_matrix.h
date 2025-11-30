//
// Created by magnus on 14/11/2025.
//

#ifndef PROJEKT_KODE_DFS_MATRIX_H
#define PROJEKT_KODE_DFS_MATRIX_H

#endif //PROJEKT_KODE_DFS_MATRIX_H


int dfs_matrix(int startnode, int n, const int adj[n][n], int dfsmatrix[], int visited[], int parent[], int cycles[][2], int *cycle_count, int count);
void print_DFS_explanation();

int dfs_matrix_onlyforgui(int startnode, int n, const int adj[n][n], int dfsmatrix[], int visited[], int parent[], int count, int *pLineHeight);

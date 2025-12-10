//
// Created by magnus on 29/11/2025.
//

#ifndef PROJEKT_KODE_GUI_H
#define PROJEKT_KODE_GUI_H

// Tab functions
void DrawTab_InputValidation();
void DrawTab_AdjacencyMatrix();
void DrawTab_StabilityCheck();

//GUI bfs matrix istedet for printf
int bfs_matrix_drawtext_ONLYFORUSEINRAYGUI(int n, const int adj[n][n], int src, int bfs[]);


int dfs_matrix_onlyforgui(int startnode, int n, const int adj[n][n], int dfsmatrix[], int visited[], int parent[], int cycles[][2], int *cycle_count, int count, int *pLineHeight, int baseX, int baseY);

void DrawTab_AlgorithmVisualization();
void DrawTab_GraphView();
void Clear();
int runGUI();

#endif //PROJEKT_KODE_GUI_H

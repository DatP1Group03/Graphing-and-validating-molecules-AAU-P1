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

    return 0;
}




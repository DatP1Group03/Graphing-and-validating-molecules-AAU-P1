#include <stdio.h>
#include "bfs_matrix.h"
#include "dfs_matrix.h"
#include "Input/validation.h"
#include "Adjacency_matrix.h"
#include "valence_check.h"
#include "toxicphore/toxicphore.h"
#include "smiles_nodefeature/SMILESNODE.h"
#include "valence_check.h"



#include "gui.h"







int main(void)
{

    int run = 1;
    while (run == 1) {
        run = runGUI();
    }


/*
char smile[100];  

printf("Please input SMILE string \n"); 
scanf("%s", smile); 

printf("Du inputtede smile: %s \n", smile);

int is_it_valid = validate_smiles(smile); 

if (is_it_valid == 1){
		printf("The smile is valid \n"); 
	} else {
		printf("Please try again \n"); 
		return -1; 
	}
int atom_count = get_atom_count(smile); 

int adj[atom_count][atom_count]; 
create_adjacency_matrix(smile, atom_count, adj);

int valence = run_valence_check(atom_count,smile,  adj); 

// node feature matrix
 char atoms[MAX_ATOMS][3];
 double node_matrix[MAX_ATOMS][MAX_FEATURES];
 int n_atoms = parse_SMILES(smile, atoms);
build_node_matrix(atoms, n_atoms, node_matrix);
if (!validate_features(node_matrix, n_atoms)) {
fprintf(stderr, "Node feature validation failed. See errors above. \n");
    print_errors();
    return 1;
}


    // 7. Hvis programmet er kørt uden fejl, så foretager den en "handling action", som så afslutter med en output til vores validation af SMILES ADJ.
print_node_matrix(atoms, node_matrix, n_atoms);

char toxicphore[100]; 
printf("Nu skal du venligst indtaste smile string på det toxicphore du ønsker at find \n"); 
scanf("%s", toxicphore); 

printf("Du inputtede toxicphore: %s \n", toxicphore); 

int is_it_valid_toxicphore = validate_smiles(toxicphore); 

if (is_it_valid_toxicphore == 1){
		printf("The toxicphore is valid \n"); 
	} else {
		printf("Please try again \n"); 
		return -1; 
	}
char atom_symbol[100]; 

int result = fill_atom_symbols_from_smile(smile, atom_symbol, atom_count); 

printf("%d \n", result); 

for (int i = 0; i < atom_count; i++){
		printf("%c \n", atom_symbol[i]); 
	}
int toxicphore_test = toxicphore_function(smile, toxicphore); 

printf("toxicphore_test: %d \n", toxicphore_test); 

*/

/* hvis man vil kører fra terminal	
    int n = 6;
    int adj[6][6] = {
        {0,1,1,0,0,0},  // 0: Naboer -> 1,2
        {1,0,1,0,0,0},  // 1: naboer -> 0,2,
        {1,1,0,1,1,1},  // 2: naboer -> 0,1,3,4 5
        {0,0,1,0,1,0},  // 3: naboer -> 2,4
        {0,0,1,1,0,0},  // 4: nabo -> 2,3
        {0,0,1,0,0,0}   // 5: nabo -> 2
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




    int dfsmatrixarr[6];
    int visited[6] = {0}; //Important that we initial our arrays, for visited to 0, so we know the values of it if there isnt a connection.
    int parent[6] = {-1};  //Important that we initial our arrays, for parent to -1, so we know the values of parent[0] also will be -1.
    // print_DFS_explanation();
	
    int cycle_count = 0; 
    int cycles[6][2]; 
    int dfs_count = dfs_matrix(0, n, adj, dfsmatrixarr, visited,parent,cycles, &cycle_count, 0);
    printf(" >>> DFS finished, returned a list with %d and our number of atom in molecules is %d \n", dfs_count, n);
    */
	return 0;
}




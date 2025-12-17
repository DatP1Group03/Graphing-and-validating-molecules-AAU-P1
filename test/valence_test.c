#include <assert.h>
#include "valence_check.h"
#include "adjacency_matrix/Adjacency_matrix.h"
#include "validation.h"

static int error_count_test = 0; 
static Error errors_test[MAX_ERRORS]; 


static void reset_state(void) {
	error_count_test = 0;
	for (int i = 0; i < MAX_ERRORS; i++) {
		errors_test[i].message[0] = '\0';
		errors_test[i].position = 0;
	}
}

void test_overbonded_oxygen(void)
{
	reset_state();
    char *SMILES = "C=O=C";
    validate_smiles(SMILES, &error_count_test, errors_test);
    int atom_count = get_atom_count(SMILES);

	int smile_size = count_smiles(SMILES);
	Symbol *molecule = NULL; 
    int adj[atom_count][atom_count];
    create_adjacency_matrix(SMILES, atom_count, adj);
    int result = run_valence_check(&molecule, smile_size, atom_count, SMILES, adj);

    assert(result == 0);
}
void test_overbonded_nitrogen(void)
{
	reset_state();
    char *SMILES = "N=N#N";
    validate_smiles(SMILES, &error_count_test, errors_test);
    int atom_count = get_atom_count(SMILES);
	int smile_size = count_smiles(SMILES);
	Symbol *molecule = NULL; 
    int adj[atom_count][atom_count];
    create_adjacency_matrix(SMILES, atom_count, adj);

    int result = run_valence_check(&molecule, smile_size, atom_count, SMILES, adj);

    assert(result == 0);
}
void test_bonds(void)
{
	reset_state();
    char *SMILES = "C=C=C=C=C";
    validate_smiles(SMILES, &error_count_test, errors_test);
    int atom_count = get_atom_count(SMILES);
    int bonds[5]  = {2,4,4,4,2};

    int adj[atom_count][atom_count];
    create_adjacency_matrix(SMILES, atom_count, adj);

    assert(error_count_test == 0);
}
void test_implicit_h(void) {
    reset_state();
    char *SMILES = "O=C=O";
    validate_smiles(SMILES, &error_count_test, errors_test);
    int atom_count = get_atom_count(SMILES);
    int implicitH[3]  = {0,2,0,};
    int adj[atom_count][atom_count];
    create_adjacency_matrix(SMILES, atom_count, adj);

    assert(error_count_test == 0);
}

int main(void)
{
    test_overbonded_oxygen();
    test_overbonded_nitrogen();
    test_bonds();
    test_implicit_h();
    return 0;
}

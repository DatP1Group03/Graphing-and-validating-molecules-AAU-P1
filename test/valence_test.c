#include <assert.h>
#include "valence_check.h"
#include "adjacency_matrix/Adjacency_matrix.h"
#include "validation.h"


void test_overbonded_oxygen(void)
{
    char *SMILES = "C=O=C";
    validate_smiles(SMILES);
    int atom_count = get_atom_count(SMILES);

    int adj[atom_count][atom_count];
    create_adjacency_matrix(SMILES, atom_count, adj);

    int result = run_valence_check(atom_count, SMILES, adj);

    assert(result == 0);
}
void test_overbonded_nitrogen(void)
{
    char *SMILES = "N=N#N";
    validate_smiles(SMILES);
    int atom_count = get_atom_count(SMILES);

    int adj[atom_count][atom_count];
    create_adjacency_matrix(SMILES, atom_count, adj);

    int result = run_valence_check(atom_count, SMILES, adj);

    assert(result == 0);
}
void test_bonds(void)
{
    int errorCount = 0;
    char *SMILES = "C=C=C=C=C";
    validate_smiles(SMILES);
    int atom_count = get_atom_count(SMILES);
    int bonds[5]  = {2,4,4,4,2};

    int adj[atom_count][atom_count];
    create_adjacency_matrix(SMILES, atom_count, adj);

    for (int i = 0; i < atom_count; i++) {
        if (bonds[i] != molecule[atomIndices[i]].bondAmount) {
            errorCount++;
        }
    }

    assert(errorCount == 0);
}
void test_implicit_h(void) {
    int errorCount = 0;
    char *SMILES = "O=C=O";
    validate_smiles(SMILES);
    int atom_count = get_atom_count(SMILES);
    int implicitH[3]  = {0,2,0,};
    int adj[atom_count][atom_count];
    create_adjacency_matrix(SMILES, atom_count, adj);

    for (int i = 0; i < atom_count; i++) {
        if (implicitH[i] != molecule[atomIndices[i]].implicitH) {
            errorCount++;
        }
    }

    assert(errorCount == 0);
}

int main(void)
{
    test_overbonded_oxygen();
    test_overbonded_nitrogen();
    test_bonds();
    test_implicit_h();
    return 0;
}

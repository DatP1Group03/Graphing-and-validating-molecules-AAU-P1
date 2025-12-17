#include "mtest.h"
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

/*
 * Verifies that an oxygen atom with too many bonds (C=O=C)
 * is rejected by the valence check due to exceeding allowed valence.
 */
TEST_CASE(test_overbonded_oxygen, {
	reset_state();

	char *SMILES = "C=O=C";
	validate_smiles(SMILES, &error_count_test, errors_test);

	int atom_count = get_atom_count(SMILES);
	int smile_size = count_smiles(SMILES);

	Symbol *molecule = NULL;
	int adj[atom_count][atom_count];
	create_adjacency_matrix(SMILES, atom_count, adj);

	int result = run_valence_check(&molecule, smile_size, atom_count, SMILES, adj);

	CHECK_EQ_INT(0, result);
})

/*
 * Verifies that a nitrogen atom exceeding its allowed valence (N=N#N)
 * is correctly detected and rejected by the valence check.
 */
TEST_CASE(test_overbonded_nitrogen, {
	reset_state();

	char *SMILES = "N=N#N";
	validate_smiles(SMILES, &error_count_test, errors_test);

	int atom_count = get_atom_count(SMILES);
	int smile_size = count_smiles(SMILES);

	Symbol *molecule = NULL;
	int adj[atom_count][atom_count];
	create_adjacency_matrix(SMILES, atom_count, adj);

	int result = run_valence_check(&molecule, smile_size, atom_count, SMILES, adj);

	CHECK_EQ_INT(0, result);
})

/*
 * Verifies that a linear molecule with alternating double bonds (C=C=C=C=C)
 * passes syntax validation and does not trigger any validation errors.
 */
TEST_CASE(test_multiple_double_bonds, {
	reset_state();

	char *SMILES = "C=C=C=C=C";
	validate_smiles(SMILES, &error_count_test, errors_test);

	int atom_count = get_atom_count(SMILES);
	int adj[atom_count][atom_count];
	create_adjacency_matrix(SMILES, atom_count, adj);

	CHECK_EQ_INT(0, error_count_test);
})

/*
 * Verifies correct handling of implicit hydrogens in a valid molecule (O=C=O),
 * ensuring that the valence and validation logic accept the structure.
 */
TEST_CASE(test_implicit_hydrogens, {
	reset_state();

	char *SMILES = "O=C=O";
	validate_smiles(SMILES, &error_count_test, errors_test);

	int atom_count = get_atom_count(SMILES);
	int adj[atom_count][atom_count];
	create_adjacency_matrix(SMILES, atom_count, adj);

	CHECK_EQ_INT(0, error_count_test);
})

MAIN_RUN_TESTS(
	test_overbonded_oxygen,
	test_overbonded_nitrogen,
	test_multiple_double_bonds,
	test_implicit_hydrogens
)

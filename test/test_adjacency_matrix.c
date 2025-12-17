//
// Created by jonas on 19-11-2025.
//

#include "Adjacency_matrix.h"
#include "mtest.h"

/*
 * Toluene (ring + substituent):
 * Verifies that create_adjacency_matrix() correctly parses a ring closure ("1...1")
 * and connects the methyl substituent to the ring, producing the expected 7x7 adjacency matrix.
 */
TEST_CASE(Toluene,
	char* SMILES = "CC1CCCCC1";
	int atom_count = get_atom_count(SMILES);
	int adj[atom_count][atom_count];
	for (int i = 0; i<atom_count; i++ ) {
		for (int j = 0; j<atom_count; j++) {
		adj[j][i]= 0;
		}
	}
	create_adjacency_matrix(SMILES, atom_count, adj);
	int correctAdj[7][7] = {0, 1, 0, 0, 0, 0, 0,
                        	1, 0, 1, 0, 0, 0, 1,
                        	0, 1, 0, 1, 0, 0, 0,
                        	0, 0, 1, 0, 1, 0, 0,
                        	0, 0, 0, 1, 0, 1, 0,
                        	0, 0, 0, 0, 1, 0, 1,
                        	0, 1, 0, 0, 0, 1, 0};

	int errorCount = 0;
	for (int i = 0; i<atom_count; i++ ) {
		for (int j = 0; j<atom_count; j++) {
			if (adj[j][i]!=correctAdj[j][i]) errorCount++;
		}
	}
	CHECK(errorCount==0, "toluene");
)

/*
 * Menthol (ring + multiple branches):
 * Verifies that create_adjacency_matrix() correctly handles a cyclohexane ring ("1...1"),
 * multiple sidechains using parentheses, and attaches substituents to the correct ring atoms.
 * The produced adjacency matrix must match the known correct structure.
 */
TEST_CASE(Menthol,
	char* SMILES = "CC1CCC(C(C)C)C(O)C1";
	int atom_count = get_atom_count(SMILES);
	int adj[atom_count][atom_count];

	for (int i = 0; i<atom_count; i++ ) {
		for (int j = 0; j<atom_count; j++) {
			adj[j][i]= 0;
		}
	}

	create_adjacency_matrix(SMILES, atom_count, adj);

	int correctAdj[11][11] = {
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0
	};

	int errorCount = 0;
	for (int i = 0; i<atom_count; i++ ) {
		for (int j = 0; j<atom_count; j++) {
			if (adj[j][i]!=correctAdj[j][i]) errorCount++;
		}
	}

	CHECK(errorCount==0, "Menthol");
)

/*
 * Caffeine (hetero atoms + ring fusion + double bonds):
 * Verifies that create_adjacency_matrix() correctly parses a more complex SMILES with:
 * - multiple ring indices (fused/connected ring system),
 * - hetero atoms (N, O),
 * - explicit double bonds ('=') and carbonyls ("=O"),
 * and encodes correct bond orders (1 vs 2) in the adjacency matrix (reference matrix from RDKit).
 */
TEST_CASE(Caffeine,
	char* SMILES = "CN1C=NC2=C1C(=O)N(C)C(=O)N2C";
	int atom_count = get_atom_count(SMILES);
	int adj[atom_count][atom_count];

	for (int i = 0; i<atom_count; i++ ) {
		for (int j = 0; j<atom_count; j++) {
			adj[j][i]= 0;
		}
	}

	create_adjacency_matrix(SMILES, atom_count, adj);

	int correctAdj[14][14] = { // rdkit
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0,
	0, 1, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 2, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	};

	int errorCount = 0;
	for (int i = 0; i<atom_count; i++ ) {
		for (int j = 0; j<atom_count; j++) {
			if (adj[j][i]!=correctAdj[j][i]) errorCount++;
		}
	}

	CHECK(errorCount==0, "caffeine");
)

/*
 * Nested multiple sidechains (deeply nested parentheses + multiple double bonds):
 * Verifies that create_adjacency_matrix() correctly handles nested branches (parentheses inside parentheses)
 * and multiple '=’ bonds, ensuring each branch is connected to the correct parent atom and that
 * double-bond edges are stored with weight 2 (reference matrix from RDKit).
 */
TEST_CASE(nested_multiple_sidechains,
	  char* SMILES = "C(=C(=C)=C)(=C)=C";
	int atom_count = get_atom_count(SMILES);
	int adj[atom_count][atom_count];

	for (int i = 0; i<atom_count; i++ ) {
		for (int j = 0; j<atom_count; j++) {
			adj[j][i]= 0;
		}
	}

	create_adjacency_matrix(SMILES, atom_count, adj);

	int correctAdj[6][6] = { // rdkit
		0,2,0,0,2,2,
		2,0,2,2,0,0,
		0,2,0,0,0,0,
		0,2,0,0,0,0,
		2,0,0,0,0,0,
		2,0,0,0,0,0
	};

	int errorCount = 0;
	for (int i = 0; i<atom_count; i++ ) {
		for (int j = 0; j<atom_count; j++) {
			if (adj[j][i]!=correctAdj[j][i]) errorCount++;
		}
	}

	CHECK(errorCount==0, "caffeine");
)


MAIN_RUN_TESTS(Toluene, Menthol, Caffeine,nested_multiple_sidechains )

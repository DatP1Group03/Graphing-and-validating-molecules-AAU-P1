#include "valence_check.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

// defined valency for each atom
#define  MAX_ALLOWED_C 4
#define  MAX_ALLOWED_N 3
#define  MAX_ALLOWED_O 2

/*
 * create_molecule()
 *
 * This function parses a SMILES input string and initializes the internal
 * molecule representation used for valence checking.
 *
 * It dynamically allocates two arrays:
 *  - molecule: an array of Symbol structs with one entry per character
 *    in the SMILES string
 *  - atomIndices: a mapping from atom order (0..atomCount-1) to their
 *    corresponding index in the SMILES string
 *
 * During the parsing, each character in the SMILES string is classified
 * as either an atom, bond, or branch symbol. For atom symbols, the function:
 *  - stores the SMILES index in atomIndices
 *  - increments atomCount
 *  - assigns the maximum allowed valence based on the atom type
 *
 * All fields in the Symbol struct are initialized to safe default values
 * to avoid accessing uninitialized memory.
 *
 * The function updates pointer variables molecule, atomIndices, and
 * atomCount, which are later used by the valence checking routines.
 *
 * Parameters:
 *  - smiles_size: length of the SMILES input string
 *  - input: the SMILES string to be parsed
 */

void create_molecule(int smiles_size, const char input[], Symbol **molecule, int **atom_indices, int *atom_count)  {
	int current_id = 0;
	*atom_count =0; 
	*molecule = malloc(sizeof(Symbol) * smiles_size);
	*atom_indices = malloc(sizeof(int) * smiles_size);
	
	for (int i = 0; i < smiles_size; i++) {
		(*molecule)[i].bondAmount = 0;
		(*molecule)[i].implicitH = 0;
		(*molecule)[i].illegalValence = 0;
		(*molecule)[i].bondType = 0;
		(*molecule)[i].maxBonds = 0;
		(*molecule)[i].atomChar = toupper(input[i]);

		if (isalpha(input[i])) {
			(*molecule)[i].type = SYMBOL_ATOM;
			(*atom_indices)[current_id] = i;
			current_id++;
			(*atom_count)++;

			switch (input[i]) {
				case 'C': (*molecule)[i].maxBonds = 4; break;
				case 'O': (*molecule)[i].maxBonds = 2; break;
				case 'N': (*molecule)[i].maxBonds = 3; break;
			}
		}
		else if (input[i] == '-' || input[i] == '=' || input[i] == '#') {
			(*molecule)[i].type = SYMBOL_BOND;

			switch (input[i]) {
				case '-': (*molecule)[i].bondType = 1; break;
				case '=': (*molecule)[i].bondType = 2; break;
				case '#': (*molecule)[i].bondType = 3; break;
			}
		}
		else if (input[i] == '(' ) {
			(*molecule)[i].type = SYMBOL_BRANCH_OPEN;
		}
		else if (input[i] == ')') {
			(*molecule)[i].type = SYMBOL_BRANCH_CLOSE;
		}
	}
}



/*
 * fillAtoms()
 *
 * This function computes the number of bonds for each atom based on the
 * adjacency matrix representation of the molecule.
 *
 * The adjacency matrix is indexed by atom order (0..atomCount-1), where
 * each non-zero entry represents a bond between two atoms. For each atom,
 * the function sums the values in the corresponding row of the matrix to
 * determine the total bond order connected to that atom.
 *
 * Using the atomIndices mapping, the calculated bond count is written back
 * to the correct position in the molecule array, which is indexed by the
 * original SMILES string positions.
 *
 * The function relies on the file local variables molecule, which must have been initialized by create_molecule() prior
 * to calling this function.
 *
 * Parameters:
 *  - atom_size: number of atoms in the molecule (used for matrix dimensions)
 *  - matrix: adjacency matrix describing bonds between atoms
 */

void fillAtoms(int atom_size, int matrix[atom_size][atom_size], int *atom_indices, Symbol *molecule) {
	for (int i = 0; i < atom_size; i++) {
		int add_bonds = 0;
		
		for (int j = 0; j < atom_size; j++) {
			if (matrix[i][j] != 0) {
				add_bonds += matrix[i][j];
			}
		}
		int idx = atom_indices[i];
		molecule[idx].bondAmount = add_bonds;
	}
}


/*
 * fill_implicit_hydrogen()
 *
 * This function calculates the number of implicit hydrogen atoms for each
 * atom in the molecule.
 *
 * For each atom, the function uses the atom_indices array to map from the
 * atom order (0..atom_size-1) to the corresponding position in the SMILES
 * string. The number of implicit hydrogens is then computed as the difference
 * between the maximum allowed number of bonds for the atom and the number of
 * bonds already assigned to it.
 *
 * The result is stored in the implicitH field of the corresponding Symbol
 * structure.
 *
 * Parameters:
 *  - atom_size: number of atoms in the molecule
 *  - atom_indices: mapping from atom index to SMILES string position
 *  - molecule: array of Symbol structs representing the parsed SMILES input
 */

void fill_implicit_hydrogen(int atom_size, int *atom_indices, Symbol *molecule) {
	for (int i = 0; i < atom_size; i++) {
		int idx = atom_indices[i];
		molecule[idx].implicitH = molecule[idx].maxBonds - molecule[idx].bondAmount;
	}
}


/*
 * valence_check_struct()
 *
 * This function verifies that the valence of each atom in the molecule does
 * not exceed the maximum allowed value for its atom type.
 *
 * For each atom, the function uses the atom_indices mapping to access the
 * corresponding Symbol entry in the molecule array. The number of assigned
 * bonds is compared against predefined maximum valence limits for carbon,
 * oxygen, and nitrogen.
 *
 * If an atom exceeds its allowed valence, the illegalValence flag is set for
 * that atom and the molecule is marked as invalid. The function continues
 * checking all atoms to ensure that all valence violations are detected.
 *
 * Parameters:
 *  - atom_size: number of atoms in the molecule
 *  - atom_indices: mapping from atom order to SMILES string positions
 *  - molecule: array of Symbol structs representing the parsed molecule
 *
 * Returns:
 *  - 1 if all atoms satisfy their valence constraints
 *  - 0 if one or more atoms violate the valence rules
 */

int valence_check_struct(int atom_size, int *atom_indices, Symbol *molecule) {
	int valid = 1;

	for (int i = 0; i < atom_size; i++) {
		int idx = atom_indices[i];
		if (molecule[idx].atomChar == 'C' && molecule[idx].bondAmount > MAX_ALLOWED_C) {
			molecule[idx].illegalValence = 1;
			valid = 0;
		}

		if (molecule[idx].atomChar == 'O' && molecule[idx].bondAmount > MAX_ALLOWED_O) {
			molecule[idx].illegalValence = 1;
			valid = 0;
		}
		if (molecule[idx].atomChar == 'N' && molecule[idx].bondAmount > MAX_ALLOWED_N) {
			molecule[idx].illegalValence = 1;
			valid = 0;
		}
	}
	return valid;
}

/*
 * free_valency_memory()
 *
 * This function releases all dynamically allocated memory used by the
 * valence checking module.
 *
 * It frees the molecule and atomIndices arrays and resets their pointers
 * to NULL to prevent dangling references and accidental reuse.
 *
 * The function should be called once the valence check has completed and
 * the allocated data is no longer needed.
 */
void free_valency_memory(Symbol **molecule) {
	free(*molecule); 
	*molecule = NULL;
}

int run_valence_check(Symbol **molecule,int smile_size, int atom_size, const char input[], int matrix[atom_size][atom_size]) {
	// Internalizing of variables 
	int *atom_indices = NULL; // a mapping from atom order to their corresponding index in the SMILES string 
	int atom_count = 0; 
	create_molecule(smile_size, input, molecule, &atom_indices, &atom_count);
	fillAtoms(atom_count, matrix, atom_indices, *molecule);
	fill_implicit_hydrogen(atom_count, atom_indices, *molecule);

	printf("\nTest :\n");
	for (int i = 0; i < atom_count; i++) {
        	int idx = atom_indices[i];
        	printf("Atom %d (%c): bonds=%d, implicitH=%d\n",
			i, (*molecule)[idx].atomChar, (*molecule)[idx].bondAmount, (*molecule)[idx].implicitH);
    	}

	int valid = valence_check_struct(atom_count, atom_indices, *molecule);

	if (valid)
		printf("\nValence check passed\n");
	else
		printf("\nMolecule invalid: Valency check failed\n"); 

	free(atom_indices);
	atom_indices = NULL; 
	return valid;
}


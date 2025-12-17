// Kode skrevet af momar25aau
//
#include "SMILESNODE.h"
#include "validation.h"


/*
 * read_atom_symbol()
 *
 * This helper function attempts to read a valid atom symbol from a SMILES
 * string starting at position s.
 *
 * The function supports both one-character and two-character atom symbols.
 * If the first character is an uppercase letter, it may optionally be followed
 * by a lowercase letter (e.g. "C", "O", "Cl", "Br"). If the first character is
 * lowercase, it is treated as a single-character aromatic atom (e.g. "c", "n").
 *
 * The detected atom symbol is written into the output buffer 'out', which is
 * expected to have space for at least three characters (including the null
 * terminator).
 *
 * Return value:
 *  - 2 if a two-character atom symbol was read
 *  - 1 if a one-character atom symbol was read
 *  - 0 if no valid atom symbol was found
 *
 * This return value allows the caller to advance the input index by the number
 * of characters consumed.
 */
static int read_atom_symbol(const char *s, char out[3]) {
	if (s[0] == '\0') return 0;
	if (isupper((unsigned char)s[0])) {
		out[0] = s[0];
		out[1] = '\0';
		out[2] = '\0';
		if (islower((unsigned char)s[1])) {
			out[1] = s[1];
			out[2] = '\0';
			return 2;
		}
		return 1;
	} else if (islower((unsigned)s[0])) {
		out[0] = s[0];
		out[1] = '\0';
		out[2] = '\0';
		return 1;
	}
	return 0;
}

/*
 * parse_SMILES()
 *
 * This function parses a SMILES string and extracts all atom symbols in
 * sequential order. Each detected atom is stored in the 'atoms' array,
 * where each entry can hold up to two characters plus a null terminator.
 *
 * The function scans the SMILES string character by character and attempts
 * to read valid atom symbols using read_atom_symbol(). Non-atom characters
 * (such as bond symbols or parentheses) are skipped.
 *
 * Parsing stops when either the end of the SMILES string is reached or the
 * maximum number of atoms (MAX_ATOMS) has been collected.
 *
 * If no atoms are found, an error is reported using add_error() (using the 
 * same error system as validation) , and the function returns 0.
 *
 * Parameters:
 *  - smiles: input SMILES string
 *  - atoms: output array storing parsed atom symbols
 *  - error_count: pointer to the global error counter
 *  - errors: array storing accumulated error messages
 *
 * Returns:
 *  - the number of atoms successfully parsed
 *  - 0 if no atoms were found
 */
int parse_SMILES(const char *smiles, char atoms[MAX_ATOMS][3], int *error_count, Error errors[]) {
	int count = 0;
	int i = 0;
	while (smiles[i] != '\0' && count < MAX_ATOMS) {
		char a[3] = {'\0','\0','\0'};
		int consumed = 0;
		if (isupper((unsigned char)smiles[i])) {
			consumed = read_atom_symbol(&smiles[i], a);
		} else if (islower((unsigned char)smiles[i])) {
			consumed = read_atom_symbol(&smiles[i], a);
		} else {
			i++;
			continue;
		}
		if (consumed > 0) {
			atoms [count][0] = a[0];
			atoms[count][1] = a[1];
			atoms[count][2] = '\0';
			count++;
			i += consumed;
			continue;
		}
		i++;
	}
	if (count == 0) {
		add_error("No atoms found in SMILES", 0, error_count, errors);
		return 0;
	}
	return count;
}

/*
 * build_node_matrix()
 *
 * This function constructs the node feature matrix for a molecule based on
 * the list of parsed atom symbols.
 *
 * Each row in the matrix corresponds to a single atom and stores a fixed set
 * of features:
 *  - column 0: atomic number
 *  - column 1: typical valence
 *  - column 2: aromaticity flag (1 if aromatic, 0 otherwise)
 *
 * The function iterates through the atom list and assigns feature values
 * according to the atom type. Both uppercase (aliphatic) and lowercase
 * (aromatic) SMILES symbols are supported.
 *
 * If an unknown or unsupported atom symbol is encountered, the function
 * reports an error using add_error(). This allows invalid node features
 * to be detected early and handled consistently by the validation system.
 *
 * Parameters:
 *  - atoms: array of atom symbols extracted from the SMILES string
 *  - n_atoms: number of atoms in the molecule
 *  - matrix: output node feature matrix
 *  - error_count: pointer to the global error counter
 *  - error: array storing accumulated error messages
 */
void build_node_matrix(char atoms[MAX_ATOMS][3], int n_atoms, double matrix[MAX_ATOMS][MAX_FEATURES], int *error_count, Error error[]) {
	for (int i = 0; i < n_atoms; ++i) {
		matrix[i][0] = 0.0;
		matrix[i][1] = 0.0;
		matrix[i][2] = 0.0;
		if (strcmp(atoms[i], "C") == 0) {
			matrix[i][0] = 6;
			matrix[i][1] = 4;
			matrix[i][2] = 0;
		} else if (strcmp(atoms[i], "c") == 0) {
			matrix[i][0] = 6;
			matrix[i][1] = 3;
			matrix[i][2] = 1;
		} else if (strcmp(atoms[i], "O") == 0 || strcmp(atoms[i], "o") == 0) {
			matrix[i][0] = 8;
			matrix[i][1] = 2;
			matrix[i][2] = (atoms [i][0] == 'o') ? 1 : 0;
		} else if (strcmp(atoms[i], "N") == 0 || strcmp(atoms[i], "n") == 0) {
			matrix[i][0] = 7;
			matrix[i][1] = 3;
			matrix[i][2] =  (atoms [i][0] == 'n') ? 1 : 0;
		} else if (strcmp(atoms[i], "S") == 0 || strcmp(atoms[i], "s") == 0) {
			matrix[i][0] = 16;
			matrix[i][1] = 2;
			matrix[i][2] =  (atoms [i][0] == 'n') ? 1 : 0;
		} else if (strcmp(atoms[i], "H") == 0) {
			matrix[i][0] = 1;
			matrix[i][1] = 1;
			matrix[i][2] = 0;
		} else if (strcmp(atoms[i], "Cl") == 0) {
			matrix[i][0] = 17;
			matrix[i][1] = 1;
			matrix[i][2] = 0;
		} else if (strcmp(atoms[i], "Br") == 0) {
			matrix[i][0] = 35;
			matrix[i][1] = 1;
			matrix[i][2] = 0;
		} else {
			add_error("Unkown atom symbol in node feature extraction", i+1, error_count, error); 
		}
	}
}

/*
 * validate_features()
 *
 * This function validates the node feature matrix generated for a molecule.
 *
 * For each atom, the function checks whether the essential features have been
 * correctly assigned. In particular, it verifies that:
 *  - the atomic number (column 0) is non-zero
 *  - the expected valence (column 1) is non-zero
 *
 * A zero value in any of these fields indicates that the atom feature extraction
 * failed or that an unsupported atom symbol was encountered earlier.
 *
 * If an invalid feature is detected, an error is reported using add_error(),
 * and the function continues checking the remaining atoms in order to collect
 * all possible errors.
 *
 * Parameters:
 *  - matrix: node feature matrix to validate
 *  - n_atoms: number of atoms in the molecule
 *  - error_count: pointer to the global error counter
 *  - errors: array storing accumulated error messages
 *
 * Returns:
 *  - 1 if all node features are valid
 *  - 0 if one or more invalid features were found
 */
int validate_features(double matrix[MAX_ATOMS][MAX_FEATURES], int n_atoms, int *error_count, Error errors[]) {
	int ok = 1;
	for (int i = 0; i < n_atoms; i++) {
		if (matrix[i][0] == 0 || matrix[i][1] == 0) {
			add_error("Error: Invalid feature found", i+1, error_count, errors);
			ok = 0;
		}
	}
	return ok;
}

/*
 * print_node_matrix()
 *
 * This function prints the node feature matrix in a human-readable tabular
 * format for debugging and verification purposes.
 *
 * Each row corresponds to a single atom and displays:
 *  - the atom index
 *  - the atom symbol
 *  - atomic number
 *  - typical valence
 *  - aromaticity flag
 *
 * This output is primarily intended for development and debugging, and is
 * not used as part of the core algorithmic processing.
 *
 * Parameters:
 *  - atoms: array of atom symbols
 *  - matrix: node feature matrix
 *  - n_atoms: number of atoms in the molecule
 */
void print_node_matrix(char atoms[MAX_ATOMS][3], double matrix[MAX_ATOMS][MAX_FEATURES], int n_atoms) {
	printf("\nNode Feature Matrix: index | atom | atomic# | valence | aromatic):\n") ;
	printf("-----------------------------------------------------------\n");
	for (int i = 0; i < n_atoms; ++i) {
		printf("%-5d %-5s %-8.0f %-8.0f %8.0f\n",
	 		i, atoms[i],matrix[i][0],matrix[i][1],matrix[i][2]);
		printf("-----------------------------------------------------------\n");
	}
}

#include "Adjacency_matrix.h"

/*
 * create_adjacency_matrix()
 * High-level wrapper that builds the adjacency matrix for a SMILES string.
 * Internally it generates a 1D adjacency representation (atom_count x atom_count),
 * converts it into the provided 2D matrix, and then frees the temporary allocation.
 */
void create_adjacency_matrix(char* SMILES, int atom_count, int adjacency_matrix[atom_count][atom_count]) { //makes the adjacency matrix and transfers it into the two dimensional array it is given
	int *Adjacency = find_adjacency(SMILES, atom_count); //makes an adjacency matrix with the find matrix function which is one dimensional
	convert_matrix(Adjacency, atom_count, adjacency_matrix); //converts the one dimensional array into a two dimensional array

	free(Adjacency); // frees memory
}

/*
 * find_adjacency()
 * Core SMILES-to-graph parser that constructs an adjacency matrix (stored as a 1D array).
 *
 * Supported features (in this simplified parser):
 *  - Atoms are detected via isalpha() and numbered in the order they appear.
 *  - Bonds between consecutive atoms are added (single/double/triple via -, =, #).
 *  - Branches (parentheses) are handled by scanning backwards to find the correct
 *    attachment atom outside the branch context.
 *  - Ring closures are handled using digit tokens (e.g., "C1...C1") by remembering
 *    the atom index where a ring digit first appears and connecting when it appears again.
 *
 * The algorithm first fills directed/partial bonds during the forward scan, then mirrors
 * the matrix at the end to ensure symmetry (undirected graph).
 *
 * Returns: dynamically allocated int array of size atom_count * atom_count.
 * Caller is responsible for freeing the returned pointer.
 */
int *find_adjacency(char S[], int atom_count) { //finds the adjacency matrix
	int length = (int) strlen(S); // used to run through the entire SMILES string in a forloop
	int rings[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; // is used to check if there has

	int *ADJ = malloc(sizeof(int) * atom_count * atom_count); //allocates space to make a full two dimensional array in a one dimensional array

	for (int i = 0; i < atom_count * atom_count; i++) { // sets all values to zero in the array
		ADJ[i] = 0;
	}

	int prev = -1; // variable to store the previous atom

	for (int i = 0; i < length; i++) { // loop which runs through the entire SMILES string
		if (isalpha(S[i])) { // checks if it is an atom
			if (prev == -1) { // if it is the first atom it does nothing
			} else if ( (S[i - 1] == ')') || // if it is an atom in any of the following cases ()(C), ()(=C), ()C, ()=C
			( (S[i-1]=='=' || S[i-1]=='#'||S[i-1]=='-')
			&& (S[i-2] == '(' && S[i-3]==')')|| S[i-2]==')' )) {
				int j; //an integer which is used to count where you are in the array since sidechains can require reading backwards
				if (S[i - 2] == ')') { // for the case where the atom is two spaces away from a sidechain but not because of another atom
					j = i - 2;
				} else { // all other cases which are in the for loop
					j = i - 1;
				}
				int k = 0; // integer used to count the parentheses where positive means that it is currently inside óne or multiple sidechains
				if (S[i-2]=='(' && (S[i-1]=='=' || S[i-1]=='#'||S[i-1]=='-')) { //if it is seperated by a bond order and inside a sidechain it adds 1 to k
					k++;
				}
				do { // do while loop which runs backwards in the SMILES string until it finds the first atom outside a sidechain
					if (S[j] == ')') { // counts up when entering a sidechain and down when exiting a sidechain
						k++;
					} else if (S[j] == '(') {
						k--;
						if (S[j - 1] == ')') { //checks if another sidechain starts after it has exited the previous sidechain
							k++;
							j--;
						}
					}
					j--;
				} while (k != 0); //Runs until k is zero
				int count = 0; // integer to count how many atoms are passed to find the atom which the curent atom should bond to
				for (int l = j; l > 0; l--) { // loop which runs from the atom which is getting bonded to, to the first character in the array
					if (isalpha(S[l])) {
						count++;
					}
				}
				ADJ[(prev +1) * atom_count + count] = get_bond_value(S, i - 1); //adds a bond between the current character and the last character before the sidechain
			} else {
			ADJ[(prev +1) * atom_count + prev] = get_bond_value(S, i - 1); // adds a bond between the current and the previous atom
			}
			prev++; // sets the previous atom to be te current one
		} else if (isdigit(S[i]) != 0) { // if it is a number
			int number = (int) S[i] - ASCII_ZERO;
			if (rings[number] == -1) { // if the number has not been seen before it stores the value of the last seen atom in the array at the space corresponding to the number which was found
				rings[number] = prev;
			} else {
				ADJ[rings[number] + prev * atom_count] = 1; // if the number has been seen before adds a bond to the atom which is stored in the spot in the array corresponding to the number
			}
		}	
	}
	for (int i = atom_count-1; i>=0;i--) { // mirrors the array since it has only counted bonds backwards
		for (int j = i; j>=0;j--) {
			ADJ[i+j*atom_count] = ADJ[i*atom_count+j];
		}
	}
	return ADJ;
}

/*
 * bond_order()
 * Maps SMILES bond symbols to an integer weight:
 * '-' -> 1, '=' -> 2, '#' -> 3, default -> 1.
 * This weight is stored in the adjacency matrix to represent bond multiplicity.
 */
int bond_order(char c) { // switch case which finds the order of a bond
	switch (c) {
		case '-':
			return 1;
		case '=':
			return 2;
		case '#':
			return 3;
		default:
			return 1;
	}
}


/*
 * get_bond_value()
 * Determines the bond value to use between two atoms based on the character
 * immediately preceding the current atom position in the SMILES string.
 * If the previous character is an atom (no explicit bond symbol), the bond is single (1).
 * Otherwise it uses bond_order() on the symbol character.
 */
int get_bond_value(char S[], int index) { // function that returns the bond order when given a place in the SMILES string
	if (isalpha(S[index])) {
		return 1;
	}
	return bond_order(S[index]);
}

/*
 * get_atom_count()
 * Counts the number of atoms in a SMILES string using isalpha() as the atom detector.
 * Note: This assumes single-letter atoms and does not handle multi-character atoms
 * like 'Cl', 'Br' in this simplified implementation.
 */
int get_atom_count(char S[]) { // counts the number of atoms by
	int counter = 0;
	for (int i = 0; i < strlen(S); i++) {
		if (isalpha(S[i])) {
			counter++;
		}
	}
	return counter;
}

/*
 * print_ADJ()
 * Debug helper that prints a 1D adjacency matrix (atom_count x atom_count)
 * in row-major order, one row per line.
 */
void print_ADJ(const int *adj, int atom_count) { // prints an adjacency matrix
	for (int i = 0; i < atom_count; i++) {
		for (int j = 0; j < atom_count; j++) {
			printf("%d ", adj[i * atom_count + j]);
		}
		printf("\n");
	}
}

/*
 * convert_matrix()
 * Converts the adjacency matrix from a 1D representation (row-major) into the
 * provided 2D array format. This is mainly for convenience when later algorithms
 * expect a 2D adjacency matrix.
 */
void convert_matrix(int* adj, int atom_count, int adjacency_matrix[atom_count][atom_count]) { // converts a one dimensional array to a two dimensional array
	for (int i = 0; i<atom_count;i++) {
		for (int j = 0; j<atom_count; j++) {
			adjacency_matrix[j][i] = adj[i*atom_count+j];
		}
	}
}

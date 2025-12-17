// Kode skrevet af momar25aau
//
#include "SMILESNODE.h"
#include "validation.h"


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

void build_node_matrix(char atoms[MAX_ATOMS][3], int n_atoms, double matrix[MAX_ATOMS][MAX_FEATURES]) {

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
            char buf[128];

            printf("Error: Unidentified atom '%s'. \n", atoms[i],i);
            matrix[i][0] = 0;
            matrix[i][1] = 0;
            matrix[i][2] = 0;
        }


    }

}
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
void print_node_matrix(char atoms[MAX_ATOMS][3], double matrix[MAX_ATOMS][MAX_FEATURES], int n_atoms) {
    printf("\nNode Feature Matrix: index | atom | atomic# | valence | aromatic):\n") ;
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < n_atoms; ++i) {
        printf("%-5d %-5s %-8.0f %-8.0f %8.0f\n",
        i, atoms[i],matrix[i][0],matrix[i][1],matrix[i][2]);

        printf("-----------------------------------------------------------\n");

    }
}

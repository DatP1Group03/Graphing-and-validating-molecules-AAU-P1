
#ifndef PROJEKT_KODE_SMILESNODE_H
#define PROJEKT_KODE_SMILESNODE_H

#include <stdio.h>
#include <string.h>

#define MAX_ATOMS 100
#define MAX_FEATURES 3 // 3 propeties: valence, atom number and aromatic



typedef struct {
    char symbol[3];
    int atomic_number;
    int valence;
    int aromatic;
}AtomFeature;

// Here it defines the functions to be used in the node feature matrix part in our program. Ready for use in SMILES NODE.c

int validate_input (const char *smiles) {
    for (int i = 0; i < strlen(smiles); i++) {
        char c = smiles [i];
        if (!((c >= 'A' && c <= 'Z') || c == '(' || c == ')' || c == '=' || c == '#')) {
            printf("Fejl: Ugyldigt tegn '%c' fundet. \n", c);
            return 0;
        }
    }
    return 1;
}

int parse_SMILES (const char *smiles, char atoms[MAX_ATOMS][3]) {
int count = 0;
    for (int i = 0; i< strlen(smiles); i++) {

if (smiles[i] >= 'A' && smiles [i] <= 'Z') {
    atoms[count][0] = smiles[i];
    atoms[count][1] = '\0';
    count++;
}
    }
if (count == 0) {
printf("Error!: No atoms found in SMILES! \n");
    return 0;
}
return count;
}

void build_node_matrix(char atoms[MAX_ATOMS][3], int n_atoms, double matrix[MAX_ATOMS][MAX_FEATURES]) {
for (int i = 0; i < n_atoms; i++) {
    if (strcmp(atoms[i], "C") == 0) {
        matrix[i][0] = 6;
        matrix[i][1] = 4;
        matrix[i][2] = 0;
    } else if (strcmp(atoms[i], "O") == 0) {
        matrix[i][0] = 8;
        matrix[i][1] = 2;
        matrix[i][2] = 0;
    } else if (strcmp(atoms[i], "N") == 0) {
        matrix[i][0] = 7;
        matrix[i][1] = 3;
        matrix[i][2] = 0;
    } else {
        printf("Error: Unidentified atom '%s'. \n", atoms[i]);
        matrix[i][0] = 0;
        matrix[i][1] = 0;
        matrix[i][2] = 0;
            }
        }
    }

int validate_features(double matrix[MAX_ATOMS][MAX_FEATURES], int n_atoms) {
    for (int i = 0; i < n_atoms; i++) {
        if (matrix[i][0] == 0 || matrix[i][1] == 0) {
            printf("Error: Invalid feature found in colummn %d.\n", i);
            return 0;
        }
    }

  return 1;
}

int print_node_matrix(char atoms[MAX_ATOMS][3], double matrix[MAX_ATOMS][MAX_FEATURES], int n_atoms) {
    printf("\nNode Feature Matrix: \n") ;


    // Header
    printf("%-6s %-12s %-8s %-8s\n", "Atom", "AtomicNumber", "Valence", "Aromatic");

    printf("-----------------------------------------------------------\n");

    // Rows
    for (int i = 0; i < n_atoms; i++) {
        printf("%-6s %-12.0f %-8.0f %-8.0f\n",
        atoms[i],
        matrix[i][0],
        matrix[i][1],
        matrix[i][2]);
        }
    }

#endif //PROJEKT_KODE_SMILESNODE_H
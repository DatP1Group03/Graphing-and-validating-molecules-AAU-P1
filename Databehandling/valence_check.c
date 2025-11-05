#include "valence_check.h"
#include "../Input/validation.h"
#include <stdlib.h>
#include <stdio.h>

// Har bare defineret max valency her indtil videre.
#define  MAX_ALLOWED_C 4
#define  MAX_ALLOWED_N 3
#define  MAX_ALLOWED_O 2
Atom *atoms = NULL;

// Fyld atom arrayet med ingenting (med mindre jeg får noget fra et andet sted, så skal jeg nok assign det til det)
void create_atoms(int smiles_size, const char input[]) {
    atoms = malloc(sizeof( Atom) * smiles_size);
    for (int i = 0; i < smiles_size; i++) {
        atoms[i].bondAmount = 0;
        atoms[i].implicitH = 0;
        atoms[i].atomChar = input[i];
    }

    for (int i = 0; i < smiles_size; i++) {

        printf("%c", atoms[i].atomChar);
    }

}
//Hvis rækkens værdi !=0 tilføjes værdien altså til addBonds, som i slutning bliver sat til atom[0].bondAmount
void fillAtoms(int smiles_size, int matrix[smiles_size][smiles_size]) {
    int addBonds = 0;

    for (int i = 0; i < smiles_size;i++) {

        addBonds = 0;
        for (int j = 0; j< smiles_size; j++) {

            if (matrix[i][j] != 0) {
                addBonds += (matrix[i][j]);
            }
        }
        atoms[i].bondAmount = addBonds;

    }

}
// Meget ligetil at beregne implicit atomer, hvis der er flere cases end disse 3 atomer, skal jeg nok tilføje dem
void fill_implicit_hydrogen(int size) {
    for (int i = 0; i < size; i++) {
        switch (atoms[i].atomChar) {
            case 'C':
                atoms[i].implicitH = MAX_ALLOWED_C - atoms[i].bondAmount;
                break;
            case 'O':
                atoms[i].implicitH = MAX_ALLOWED_O - atoms[i].bondAmount;
                break;
            case 'N':
                atoms[i].implicitH = MAX_ALLOWED_N- atoms[i].bondAmount;
                break;
            default:
                atoms[i].implicitH = 0;
        }
    }

}

int valence_check_struct(int size){

    for (int i = 0; i < size; i++) {
        if (atoms[i].atomChar == 'C' && atoms[i].bondAmount > MAX_ALLOWED_C ) {
            printf("Valence error, carbon at position %d violates valency rules\n", i );
            return 0;
        }
        if (atoms[i].atomChar == 'O' && atoms[i].bondAmount > MAX_ALLOWED_O) {
            printf("Valence error, oxygen at position %d violates valency rules\n", i );
            return 0;
        }
        if (atoms[i].atomChar == 'N' && atoms[i].bondAmount > MAX_ALLOWED_N) {
            printf("Valence error, nitrogen at position %d violates valency rules \n", i );
            return 0;
        }

    }
     return 1;

}

int run_valence_check(int smiles_size, const char input[],int matrix[smiles_size][smiles_size]) {
    create_atoms(smiles_size, input);
    fillAtoms(smiles_size, matrix);
    fill_implicit_hydrogen(smiles_size);

    int valid = valence_check_struct(smiles_size);

    if (valid)
        printf("\nValence check passed\n");
    else
        printf("\n Molecule invalid: Valency check failed\n");

    free(atoms);

    atoms = NULL; // Skal nok lade være med dette hvis der er brug for arrayet udover dette modul
    return valid;
}




#include "valence_check.h"
#include "../Input/validation.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
/**
Funktion 1: create_atoms() allokere dynamisk lager til atoms arrayet, som er et array af structet Atoms, dermed sætter den karakterene til arrayet, og incrementer atomCount hvis input[i] er en karakter
Funktion 2: fill_atoms() tager summen af en række på adjacency matrixen, og tilføjer det til vores array med index idx, som holder styr på hvor der faktisk er atomer i structet
Funktion 3: fill_implicit_hydrogen tiføjer implicit hydrogen ved at lave en simpel berening af maksimum tiladt - nuværende bonds, og gør det kun hvis input[i] er med i de cases vi beskriver i vores switch
Funktion 4: Tjekker om vi overskrider valency ved at lave en expression til at se om vi overstiger max tilladte for det pågældende atom

*/

// Har bare defineret max valency her indtil videre. Jeg ved ikke om det kan forbedres med noget JSON.
#define  MAX_ALLOWED_C 4
#define  MAX_ALLOWED_N 3
#define  MAX_ALLOWED_O 2

Atom *atoms = NULL;
int *atomIndices = NULL;
int atomCount = 0; // Holder styr på antal faktiske atomer

// Fyld atom arrayet med ingenting (med mindre jeg får noget fra et andet sted, så skal jeg nok assign det til det)
void create_atoms(int smiles_size, const char input[]) {
    int currentID = 0;
    atomCount = 0;

    atoms = malloc(sizeof(Atom) * smiles_size);
    atomIndices = malloc(sizeof(int) * smiles_size);

    for (int i = 0; i < smiles_size; i++) {
        atoms[i].bondAmount = 0;
        atoms[i].implicitH = 0;
        atoms[i].atomChar = input[i];
        if (isalpha(input[i])) { //gem indeks til hvert atom, dvs ignorere bond symvoler osv.
            atomIndices[currentID] = i;
            currentID++;
            atomCount++;
        }
    }
}

//Hvis rækkens værdi !=0 tilføjes værdien altså til addBonds, som i slutning bliver sat til atom[0].bondAmount
void fillAtoms(int atom_size, int matrix[atom_size][atom_size]) {
    for (int i = 0; i < atomCount; i++) {
        int addBonds = 0;
        for (int j = 0; j < atomCount; j++) {
            if (matrix[i][j] != 0) {
                addBonds += matrix[i][j];
            }
        }
        int idx = atomIndices[i];
        atoms[idx].bondAmount = addBonds;
    }
}

// Meget ligetil at beregne implicit atomer, hvis der er flere cases end disse 3 atomer, skal jeg nok tilføje dem
void fill_implicit_hydrogen(int size) {
    for (int i = 0; i < size; i++) {
        int idx = atomIndices[i];// bruger mapping til korrekt atom i SMILES
        switch (atoms[idx].atomChar) {
            case 'C':
                atoms[idx].implicitH = MAX_ALLOWED_C - atoms[idx].bondAmount;
                break;
            case 'O':
                atoms[idx].implicitH = MAX_ALLOWED_O - atoms[idx].bondAmount;
                break;
            case 'N':
                atoms[idx].implicitH = MAX_ALLOWED_N - atoms[idx].bondAmount;
                break;
            default:
                atoms[idx].implicitH = 0;
        }
    }
}

int valence_check_struct(int size) {
    for (int i = 0; i < size; i++) {
        int idx = atomIndices[i];// tjekker kun faktiske atomer
        if (atoms[idx].atomChar == 'C' && atoms[idx].bondAmount > MAX_ALLOWED_C) {
            printf("Valence error, carbon at position %d violates valency rules\n", i+1);
            return 0;
        }
        if (atoms[idx].atomChar == 'O' && atoms[idx].bondAmount > MAX_ALLOWED_O) {
            printf("Valence error, oxygen at position %d violates valency rules\n", i+1);
            return 0;
        }
        if (atoms[idx].atomChar == 'N' && atoms[idx].bondAmount > MAX_ALLOWED_N) {
            printf("Valence error, nitrogen at position %d violates valency rules \n", i+1);
            return 0;
        }
    }
    return 1;
}

int run_valence_check(int atom_size, const char input[], int matrix[atom_size][atom_size]) {
    // Brug globale værdier fra validation.c (smiles_input_size, atom_count osv.)
    create_atoms(smiles_input_size, input);
    fillAtoms(atomCount, matrix);
    fill_implicit_hydrogen(atomCount);

    printf("\nTest :\n");
    for (int i = 0; i < atomCount; i++) {
        int idx = atomIndices[i];
        printf("Atom %d (%c): bonds=%d, implicitH=%d\n",
               i, atoms[idx].atomChar, atoms[idx].bondAmount, atoms[idx].implicitH);
    }

    int valid = valence_check_struct(atomCount);

    if (valid)
        printf("\nValence check passed\n");
    else
        printf("\nMolecule invalid: Valency check failed\n");

    free(atoms); // Skal det bruges efter dette modul? hvis ja så fjern koden
    free(atomIndices);
    atoms = NULL;
    atomIndices = NULL;

    return valid;
}
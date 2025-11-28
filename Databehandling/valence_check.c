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

Symbol *molecule = NULL;
int *atomIndices = NULL;
int atomCount = 0; // Holder styr på antal faktiske atomer

// Fyld atom arrayet med ingenting (med mindre jeg får noget fra et andet sted, så skal jeg nok assign det til det)
void create_molecule(int smiles_size, const char input[]) {

    int currentID = 0;
    atomCount = 0;

    molecule = malloc(sizeof(Symbol) * smiles_size);
    atomIndices = malloc(sizeof(int) * smiles_size);

    for (int i = 0; i < smiles_size; i++) {

        //Default Værdier her, Type bliver sat i nederste if block, default værdier er så vi ikke tilgår tomme fields
        molecule[i].bondAmount = 0;
        molecule[i].implicitH = 0;
        molecule[i].illegalValence = 0;
        molecule[i].bondType = 0;
        molecule[i].maxBonds = 0;
        molecule[i].atomChar = toupper(input[i]);


        if (isalpha(input[i])) { //gem indeks til hvert atom, dvs ignorere bond symvoler osv.
            molecule[i].type = SYMBOL_ATOM;
            atomIndices[currentID] = i;
            currentID++;
            atomCount++;
            switch (input[i]) {
                case 'C': molecule[i].maxBonds = 4; break;
                case 'O': molecule[i].maxBonds = 2; break;
                case 'N': molecule[i].maxBonds = 3; break;
            }
        }
        else if (input[i] == '-' || input[i] == '=' || input[i] == '#') {
            molecule[i].type = SYMBOL_BOND;

            switch (input[i]) {
                case '-': molecule[i].bondType = 1; break;
                case '=': molecule[i].bondType = 2; break;
                case '#': molecule[i].bondType = 3; break;
            }
        }
        else if (input[i] == '(' ) {
            molecule[i].type = SYMBOL_BRANCH_OPEN;
        }
        else if (input[i] == ')') {
            molecule[i].type = SYMBOL_BRANCH_CLOSE;
        }

    }
}

//Hvis rækkens værdi !=0 tilføjes værdien altså til addBonds, som i slutning bliver sat til atom[0].bondAmount
void fillAtoms(int atom_size, int matrix[atom_size][atom_size]) {
    printf("Atom Count %d \n Smiles Count %d",atomCount,smiles_size);
    for (int i = 0; i < atomCount; i++) {
        int addBonds = 0;
        for (int j = 0; j < atomCount; j++) {
            if (matrix[i][j] != 0) {
                addBonds += matrix[i][j];
            }
        }
        int idx = atomIndices[i];
        molecule[idx].bondAmount = addBonds;
    }
}

// Meget ligetil at beregne implicit atomer
void fill_implicit_hydrogen(int size) {
    for (int i = 0; i < size; i++) {
        int idx = atomIndices[i];// bruger mapping til korrekt atom i SMILES
        molecule[idx].implicitH = molecule[idx].maxBonds - molecule[idx].bondAmount;
    }
}

int valence_check_struct(int size) {
    int valid = 1;

    for (int i = 0; i < size; i++) {
        int idx = atomIndices[i];

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

int run_valence_check(int atom_size, const char input[], int matrix[atom_size][atom_size]) {
    // Brug globale værdier fra validation.c (smiles_input_size, atom_count osv.)
    create_molecule(smiles_input_size, input);
    fillAtoms(atomCount, matrix);
    fill_implicit_hydrogen(atomCount);

    printf("\nTest :\n");
    for (int i = 0; i < atomCount; i++) {
        int idx = atomIndices[i];
        printf("Atom %d (%c): bonds=%d, implicitH=%d\n",
               i, molecule[idx].atomChar, molecule[idx].bondAmount, molecule[idx].implicitH);
    }

    int valid = valence_check_struct(atomCount);

    if (valid)
        printf("\nValence check passed\n");
    else
        printf("\nMolecule invalid: Valency check failed\n");




    return valid;
}
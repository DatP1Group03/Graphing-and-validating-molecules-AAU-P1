#include <stdio.h>
#include "../SMILES_NODE/SMILESNODE.h"


void TEST_COC() {


    char* SMILES = "COC"; // Here are the 3 atoms
    char atoms[MAX_ATOMS][3];
    double matrix[MAX_ATOMS][MAX_FEATURES];

    int count = parse_SMILES(SMILES, atoms);

    build_node_matrix(atoms, count, matrix);

    double correct[3][3] = {
    {6, 4, 0}, // C atom
    {8, 2, 0}, // O atom
    {6, 4, 0}
    };

    int errors = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < MAX_FEATURES; j++) {
            if (matrix[i][j] != correct [i][j])
                errors++;
        }
    }

    if (errors == 0)
        printf("TEST_COC: Passed \n");

            else
                printf("TEST_COC: Failed (%d errors)\n", errors);

        }


void TEST_CCC() {


    char* SMILES = "CCC"; // Here are the 3 atoms
    char atoms[MAX_ATOMS][3];
    double matrix[MAX_ATOMS][MAX_FEATURES];

    int count = parse_SMILES(SMILES, atoms);

    build_node_matrix(atoms, count, matrix);

    double correct[3][3] = {
        {6, 4, 0}, // C atom
        {6, 4, 0}, // C
        {6, 4, 0} // C
    };

    int errors = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < MAX_FEATURES; j++) {
            if (matrix[i][j] != correct [i][j])
                errors++;
        }
    }

            if (errors == 0)
                printf("TEST_CCC: Passed \n");

            else
                printf("TEST_CCC: Failed (%d errors)\n", errors);

        }




void TEST_O() {


    char* SMILES = "O"; // Here are the 3 atoms
    char atoms[MAX_ATOMS][3];
    double matrix[MAX_ATOMS][MAX_FEATURES];

    int count = parse_SMILES(SMILES, atoms);

    build_node_matrix(atoms, count, matrix);

    double correct[1][3] = {
        {8, 2, 0}, // C atom
    };

    int errors = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < MAX_FEATURES; j++) {
            if (matrix[i][j] != correct [i][j])
                errors++;
        }
    }

            if (errors == 0)
                printf("TEST_O: Passed \n");

            else
                printf("TEST_O: Failed (%d errors)\n", errors);

        }

void TEST_CNO() {


    char* SMILES = "CNO"; // Here are the 3 atoms
    char atoms[MAX_ATOMS][3];
    double matrix[MAX_ATOMS][MAX_FEATURES];

    int count = parse_SMILES(SMILES, atoms);

    build_node_matrix(atoms, count, matrix);

    double correct[3][3] = {
        {6, 4, 0}, // C atom
        {7, 3, 0}, // O atom
        {8, 2, 0}
    };

    int errors = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < MAX_FEATURES; j++) {
            if (matrix[i][j] != correct [i][j])
                errors++;
        }
    }

            if (errors == 0)
                printf("TEST_CNO: Passed \n");

            else
                printf("TEST_CNO: Failed (%d errors)\n", errors);

        }


int main(void) {
    TEST_COC();
    TEST_CCC();
    TEST_O();
    TEST_CNO();


}

#include "mtest.h"
#include "SMILESNODE.h"
#include "validation.h"

static int error_count_test = 0; 
static Error errors_test[MAX_ERRORS]; 

static void reset_state(void){
	error_count_test = 0; 
	for (int i = 0; i < MAX_ERRORS; i++) {
		errors_test[i].message[0] = '\0';
		errors_test[i].position = 0;
	}
}

TEST_CASE(smilesnode_COC, {
	reset_state(); 
    const char *smiles = "COC";
    char atoms[MAX_ATOMS][3];
    double matrix[MAX_ATOMS][MAX_FEATURES];

    int count = parse_SMILES(smiles, atoms, &error_count_test, errors_test);
    CHECK_EQ_INT(3, count);

    build_node_matrix(atoms, count, matrix, &error_count_test, errors_test);

    const int expected[3][MAX_FEATURES] = {
        {6, 4, 0},
        {8, 2, 0},
        {6, 4, 0}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < MAX_FEATURES; j++) {
            CHECK_EQ_INT(expected[i][j], (int)matrix[i][j]);
        }
    }
})

TEST_CASE(smilesnode_CCC, {
	reset_state();
    const char *smiles = "CCC";
    char atoms[MAX_ATOMS][3];
    double matrix[MAX_ATOMS][MAX_FEATURES];

    int count = parse_SMILES(smiles, atoms, &error_count_test, errors_test);
    CHECK_EQ_INT(3, count);

    build_node_matrix(atoms, count, matrix, &error_count_test, errors_test);

    const int expected[3][MAX_FEATURES] = {
        {6, 4, 0},
        {6, 4, 0},
        {6, 4, 0}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < MAX_FEATURES; j++) {
            CHECK_EQ_INT(expected[i][j], (int)matrix[i][j]);
        }
    }
})

TEST_CASE(smilesnode_O, {
	reset_state();
    const char *smiles = "O";
    char atoms[MAX_ATOMS][3];
    double matrix[MAX_ATOMS][MAX_FEATURES];

    int count = parse_SMILES(smiles, atoms, &error_count_test, errors_test);
    CHECK_EQ_INT(1, count);

    build_node_matrix(atoms, count, matrix, &error_count_test, errors_test);

    const int expected[1][MAX_FEATURES] = {
        {8, 2, 0}
    };

    for (int j = 0; j < MAX_FEATURES; j++) {
        CHECK_EQ_INT(expected[0][j], (int)matrix[0][j]);
    }
})

TEST_CASE(smilesnode_CNO, {
	reset_state();
    const char *smiles = "CNO";
    char atoms[MAX_ATOMS][3];
    double matrix[MAX_ATOMS][MAX_FEATURES];

    int count = parse_SMILES(smiles, atoms, &error_count_test, errors_test);
    CHECK_EQ_INT(3, count);

    build_node_matrix(atoms, count, matrix, &error_count_test, errors_test);

    const int expected[3][MAX_FEATURES] = {
        {6, 4, 0},
        {7, 3, 0},
        {8, 2, 0}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < MAX_FEATURES; j++) {
            CHECK_EQ_INT(expected[i][j], (int)matrix[i][j]);
        }
    }
})

MAIN_RUN_TESTS(
    smilesnode_COC,
    smilesnode_CCC,
    smilesnode_O,
    smilesnode_CNO
)

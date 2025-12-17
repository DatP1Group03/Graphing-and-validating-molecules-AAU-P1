#include <assert.h>
#include <string.h>
#include "validation.h"

static int error_count_test = 0; 
static Error errors_test[MAX_ERRORS]; 


static void reset_state(void) {
	error_count_test = 0;
	for (int i = 0; i < MAX_ERRORS; i++) {
		errors_test[i].message[0] = '\0';
		errors_test[i].position = 0;
	}
}

void test_illegal_character(void) {
	reset_state();
	int result = validate_smiles("C$C", &error_count_test, errors_test);
	assert(result == 0);
	assert(error_count_test == 1);
	assert(strcmp(get_error_message(0, &error_count_test, errors_test), "Illegal character\n") == 0);
}

void test_unmatched_parentheses(void) { 
	reset_state();
	int result = validate_smiles("C(C", &error_count_test, errors_test);
	assert(result == 0);
	assert(error_count_test >= 1);
}

void test_unmatched_brackets(void) {
	reset_state();
	int result = validate_smiles("C[NH3", &error_count_test, errors_test);
	assert(result == 0);
	assert(error_count_test >= 1);
}

void test_two_bond_symbols(void) {
	reset_state();
	int result = validate_smiles("C==C", &error_count_test, errors_test);
	assert(result == 0);
	assert(error_count_test >= 1);
}

void test_ring_not_closed(void) {
	reset_state();
	int result = validate_smiles("C1CC", &error_count_test, errors_test);
	assert(result == 0);
	assert(error_count_test >= 1);
}

void test_valid_smiles(void) {
	reset_state();
	int result = validate_smiles("C=C(O)C", &error_count_test, errors_test);
	assert(result == 1);
	assert(error_count_test == 0);
}

int main(void)
{
    test_illegal_character();
    test_unmatched_parentheses();
    test_unmatched_brackets();
    test_two_bond_symbols();
    test_ring_not_closed();
    test_valid_smiles();

    return 0;
}

#include "mtest.h"
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

 // Verifies that the validator rejects SMILES containing an unsupported character ('$'),
// returns failure (0), and reports exactly one error with the expected message.
TEST_CASE(test_illegal_character,{
	reset_state();
	int result = validate_smiles("C$C", &error_count_test, errors_test);
	CHECK_EQ_INT(0, result);
	CHECK_EQ_INT(1, error_count_test);
	CHECK_EQ_INT(0, strcmp(get_error_message(0, &error_count_test, errors_test), "Illegal character\n"));
})

// Ensures that an opening parenthesis without a matching closing parenthesis is detected as an error
// (validation fails and at least one error is recorded).
TEST_CASE(test_unmatched_parentheses,{ 
	reset_state();
	int result = validate_smiles("C(C", &error_count_test, errors_test);
	CHECK_EQ_INT(0, result);
	int is_error_over_one = 0; 
	if (error_count_test >= 1){ is_error_over_one =1;}
	CHECK_EQ_INT(1, is_error_over_one);
})

// Ensures that an opening bracket without a matching closing bracket is detected as an error
// (validation fails and at least one error is recorded).
TEST_CASE(test_unmatched_brackets,{
	reset_state();
	int result = validate_smiles("C[NH3", &error_count_test, errors_test);
	CHECK_EQ_INT(0, result);
	int is_error_over_one = 0; 
	if (error_count_test >= 1){ is_error_over_one =1;}
	CHECK_EQ_INT(1, is_error_over_one);
})

// Checks that consecutive bond symbols ("==") are treated as invalid syntax
// (validation fails and at least one error is recorded).
TEST_CASE(test_two_bond_symbols,{
	reset_state();
	int result = validate_smiles("C==C", &error_count_test, errors_test);
	CHECK_EQ_INT(0, result);
	int is_error_over_one = 0; 
	if (error_count_test >= 1){ is_error_over_one =1;}
	CHECK_EQ_INT(1, is_error_over_one);
})

// Checks that a ring digit without a matching closing digit (e.g., "C1...") is detected
// (validation fails and at least one error is recorded).
TEST_CASE(test_ring_not_closed,{
	reset_state();
	int result = validate_smiles("C1CC", &error_count_test, errors_test);
	CHECK_EQ_INT(0, result);
	int is_error_over_one = 0; 
	if (error_count_test >= 1){ is_error_over_one =1;}
	CHECK_EQ_INT(1, is_error_over_one);
})

// Confirms that a syntactically valid SMILES string is accepted:
// validation succeeds (1) and no errors are recorded.
TEST_CASE(test_valid_smiles,{
	reset_state();
	int result = validate_smiles("C=C(O)C", &error_count_test, errors_test);
	CHECK_EQ_INT(1, result);
	CHECK_EQ_INT(0, error_count_test);
})

MAIN_RUN_TESTS(
    test_illegal_character,
    test_unmatched_parentheses,
    test_unmatched_brackets,
    test_two_bond_symbols,
    test_ring_not_closed,
    test_valid_smiles,
)

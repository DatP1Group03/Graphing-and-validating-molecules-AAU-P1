#include <assert.h>
#include <string.h>
#include "validation.h"


static void reset_state(void) {
    validate_smiles("");
}

void test_illegal_character(void) {
    reset_state();
    int result = validate_smiles("C$C");
    assert(result == 0);
    assert(get_error_count() == 1);
    assert(strcmp(get_error_message(0), "Illegal character\n") == 0);
}

void test_unmatched_parentheses(void) {
    reset_state();
    int result = validate_smiles("C(C");
    assert(result == 0);
    assert(get_error_count() >= 1);
}

void test_unmatched_brackets(void) {
    reset_state();
    int result = validate_smiles("C[NH3");
    assert(result == 0);
    assert(get_error_count() >= 1);
}

void test_two_bond_symbols(void) {
    reset_state();
    int result = validate_smiles("C==C");
    assert(result == 0);
    assert(get_error_count() >= 1);
}

void test_ring_not_closed(void) {
    reset_state();
    int result = validate_smiles("C1CC");
    assert(result == 0);
    assert(get_error_count() >= 1);
}

void test_valid_smiles(void) {
    reset_state();
    int result = validate_smiles("C=C(O)C");
    assert(result == 1);
    assert(get_error_count() == 0);
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

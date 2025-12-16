// validation.c
#include <stdio.h>
#include <ctype.h>
#include "validation.h"
#include <string.h>

// The following is the allowed smiles symboles currently implemented. This is according to report 2.1.5
static const char allowed_smiles_symbols[] = { // GYLDIGE SMILES KARAKTER
    'C', 'N', 'O', 'S','H',
    'c', 'n', 'o', 's', 'p',
    '-', '=', '#', ':', '/', '\\',
    '(', ')','[',']',
    '1','2','3','4','5','6','7','8','9',
    '@', '.', '*'
};
// The maximum number of allowed characters
const int allowed_smiles_symbol_count = sizeof(allowed_smiles_symbols) / sizeof(allowed_smiles_symbols[0]);

/* 
	* add_error()
* Helper function used by the validation routines to register an error
* 
* The function appends a new error message and its position to the provided 
* errors array and increments the error counter via a pointer. It does not
* perform any validation logic itself and does not reset or manage the error
* counter beyond incrementing it. 
*
* The function assumes that error_count and errors are owned and managed by the calling function
* (typically validate_smiles), ensuring that all error state is handled explictly and without hidden
* global variables.
*
* Parameters:
* - msg: textual description of the error
* - pos: posision in the input string where the error occurred
* - error_count: pointer to the current number of registered errors
* - errors: array of Error structs used to store validation errors 
*/
void add_error(const char *msg, int pos, int *error_count, Error errors[]) {
    if (*error_count < MAX_ERRORS) { 
        strncpy(errors[*error_count].message, msg, MAX_MSG_LEN - 1); // copy errormessage to struct 
        errors[*error_count].position = pos; // copy position to struct 
        (*error_count)++; 
    }
}

/* 
	* is permitted()
* Checks whether all characters in the input string are part of the allowed SMILES symbol set
*
* The function iterates through the input string and compares each character
* against the list of permitted SMILES symbols. If a character is not found in the 
* allowed symbol table, an error is registered using add_error().
*
* The function does not terminate on the first error; instead, it continues scanning
* the entire input to ensure that all illegial characters are detected and reported.
*
* Paramters: 
	* input: the SMILES string to be validated 
	* error_count: pointer to the current number of registrerd erorrs 
	* errors: array used to store validation error messages 
	*
* Returns:
	* - 1 if alle characters in the input are permitted
	* - 0 if one or mor illegal characters are found
*/ 

int is_permitted(const char *input,int *error_count, Error errors[]) {
	int before = *error_count; 
	for (int i = 0; input[i] != '\0'; i++) { 
		int match_found = 0; 

		for (int m = 0; m < allowed_smiles_symbol_count; m++) {
			if (input[i] == allowed_smiles_symbols[m]) {
				match_found = 1;
				break;
			}
		}

		if (!match_found) { 
			add_error("Illegal character\n", i+1, error_count, errors);
		}
	}
	return (*error_count == before); // checks if no new errors has been added 
}

/*
 * count_smiles()
 *
 * Counts the number of characters in a SMILES input string.
 *
 * The function iterates through the input until the null terminator is
 * reached and returns the total number of characters. The count includes
 * all characters in the SMILES string (atoms, bonds, branches, digits, etc.)
 * and excludes the terminating null character.
 *
 * Parameters:
 *  - input: the SMILES string to be counted
 *
 * Returns:
 *  - the length of the SMILES string
 */
int count_smiles(const char *input) {
	int smile_size = 0;
	for (int i = 0; input[i] != '\0';i++) {
		smile_size++;
	}
	return smile_size;
}

/*
 * count_atoms()
 *
 * Counts the number of atom symbols in a SMILES input string.
 *
 * The function iterates through the input and increments the counter for
 * each alphabetic character, which in this simplified model represents
 * an atom symbol. The function does not distinguish between different atom
 * types or multi-character element symbols.
 *
 * Parameters:
 *  - input: the SMILES string to be analyzed
 *
 * Returns:
 *  - the total number of atom symbols found in the input string
 */
int count_atoms(const char *input) {
	int count_of_atoms = 0;
	for (int i = 0; input[i] != '\0';i++) {
		if (isalpha(input[i])) {
			count_of_atoms++;
		}
	}
	return count_of_atoms;
}


/*
 * closed_brackets()
 *
 * Validates that parentheses '()' and square brackets '[]' are properly balanced.
 *
 * The function scans the input string and uses two simple stacks (implemented
 * as fixed-size arrays) to track opening parentheses and opening brackets.
 * When a closing symbol is encountered, the corresponding stack is checked to
 * ensure there is a matching opening symbol. Missing or extra closing symbols,
 * as well as unmatched opening symbols remaining at the end, are reported via
 * add_error().
 *
 * The function does not stop at the first error; it continues to detect and
 * report all bracket/parenthesis issues in the input.
 *
 * Parameters:
 *  - input: the SMILES string to validate
 *  - error_count: pointer to the global error counter (owned by the caller)
 *  - errors: array used to store validation errors
 *
 * Returns:
 *  - 1 if this function added no new errors
 *  - 0 if one or more bracket/parenthesis errors were detected
 */

int closed_brackets(const char *input, int *error_count, Error errors[]) {
	int before =*error_count;
	char stack_parantheses[MAX_INPUT]; 
	char stack_brackets[MAX_INPUT];
	int top = -1;
	int top_brackets =-1;

	for (int i = 0; input[i] != '\0'; i++) {
		if (input[i] == '(') {
			stack_parantheses[++top] = '('; // add ( to the top of the stack
		}
		if (input[i] == ')' && stack_parantheses[top] != '(') {
			add_error("Closing parantheses has no matching opening parantheses \n", i+1, error_count, errors);
		}
		if (input[i] == ')' && stack_parantheses[top] == '(' ) {
			top--; 
		}
		if (input[i] == '[') {
			stack_brackets[++top_brackets] = '[';
		}
		if (input[i] == ']' && stack_brackets[top_brackets] != '[') {
			add_error("Closing bracket has no matching opening bracket \n", i+1, error_count, errors);
		}
		if (input[i] == ']' && stack_brackets[top_brackets] == '[' ) {
			top_brackets--;
		}
		if (input[i] == ']' && input[i+1] == ']' ) {
			add_error("Two consecutive brackets \n", i+1, error_count, errors);
		}
	}
	if (top != -1) {
		add_error("Unmatched '(' remain", -1, error_count, errors);
	}
	if (top_brackets != -1) {
		add_error("Unmatched '[' remain", -1, error_count, errors);
	}

	return (*error_count == before); // checks if no new errors has been added
}

/*
 * ring_closed()
 *
 * Validates that all ring closures in the SMILES string are properly matched.
 *
 * The function scans the input string for ring digits that immediately follow
 * atom symbols (e.g. C1, N2). Each ring digit represents a ring opening or
 * closing and must therefore appear exactly twice in the SMILES string.
 *
 * A small balance array is used to track ring digits. When a ring digit is
 * encountered, the corresponding entry is toggled using XOR, effectively
 * marking the ring as opened or closed. After processing the entire input,
 * any ring digit that remains unbalanced indicates an unclosed ring.
 *
 * The loop index is manually incremented when a ring digit is found in order
 * to skip over the digit and continue processing at the next atom symbol.
 *
 * Parameters:
 *  - input: the SMILES string to validate
 *  - error_count: pointer to the global error counter (owned by the caller)
 *  - errors: array used to store validation errors
 *
 * Returns:
 *  - 1 if this function added no new errors
 *  - 0 if one or more ring closure errors were detected
 */
int ring_closed(const char *input, int *error_count, Error errors[]) {
	int before = *error_count; 
	int balance[10] = {0}; // max count of rings are 9. This arrays keep track of each ring 
	for (int i = 0; input[i] != '\0'; i++) {
		if (isalpha(input[i]) && input[i+1] != '\0' && isdigit(input[i+1])) {
			int d = input[i+1] - '0'; // d is the digit after the atom 
			if (d < 0 || d > 9) { 
				add_error("Invalid ring digit",i+1, error_count, errors);
			}
			balance[d] ^= 1; // XOR toggle
			i++; // i gets incremented twice because ring C1CC.. we need to get to the next atom
		}
	}
	for (int d = 0; d < 10; d++) {
		if (balance[d] != 0) {
			add_error("Ring not closed properly", -1, error_count, errors);
		}
	}
	return (*error_count == before); // checks if no new errors has been added

    }

/*
 * misc_check()
 *
 * Performs a set of miscellaneous validation rules that are not covered by the
 * other dedicated checks.
 *
 * The function verifies that the SMILES string does not begin or end with a
 * bond symbol, detects empty branches "()", and checks for consecutive bond
 * symbols (e.g. "==", "--", "##").
 *
 * Errors are reported through add_error(), and the function continues scanning
 * to collect all issues rather than stopping at the first violation.
 *
 * Parameters:
 *  - input: the SMILES string to validate
 *  - error_count: pointer to the global error counter (owned by the caller)
 *  - errors: array used to store validation errors
 *
 * Returns:
 *  - 1 if this function added no new errors
 *  - 0 if one or more validation issues were detected
 */
int misc_check(const char *input, int *error_count, Error errors[]) {
	int before = *error_count;
	int smiles_input_size = count_smiles(input);
	if (smiles_input_size == 0) {
		add_error("SMILES string is empty\n", -1, error_count, errors);
		return 0;
	}
	// check if SMILES string starts with binding symbol 
	if (input[0] == '=' || input[0] == '-' || input[0] == '#') { 
		add_error("Smiles string cannot contain a bond symbol as the first character\n", 1, error_count, errors);
	}
	// checks if SMILES string ends with binding symbol 
	if (input[smiles_input_size-1] == '=' || input[smiles_input_size-1] == '-' || input[smiles_input_size-1] == '#') {
		add_error("Smiles string cannot contain a bond symbol as the last character\n", smiles_input_size, error_count, errors);
	}
	for (int i = 0; i < smiles_input_size-1;i++) {
		if (input[i] == '(' && input[i+1] == ')') {
			add_error("Empty branch in SMILES string",-1, error_count, errors);
		}
		if (input[i] == '=' || input[i] == '-'|| input[i] == '#') {
			if (input[i+1] == '=' || input[i+1] == '-'|| input[i+1] == '#') {
				add_error("Two consecutive bond symbols beginning at position", i+1, error_count, errors);
			}
		}
	}
	return (*error_count == before); 
}
/* print_errors
* prints all the errors in the terminal 
*/ 
void print_errors(int error_count, Error errors[]) {
    for (int i = 0; i < error_count; i++) {
        printf("Error %d: %s (position %d)\n",i+1, errors[i].message, errors[i].position);
    }
}


/*
 * validate_smiles()
 *
 * Runs the complete SMILES validation pipeline and collects all validation
 * errors in the provided errors array.
 *
 * The function resets the error counter, executes each validation check
 * (character whitelist, bracket/parenthesis matching, ring closure matching,
 * and miscellaneous SMILES rules), and stores any detected issues via add_error().
 *
 * If one or more errors are found, the function prints all collected errors
 * and returns 0. Otherwise, it returns 1 to indicate that the input passed
 * all validation checks.
 *
 * Parameters:
 *  - input: the SMILES string to validate
 *  - error_count: output parameter; will contain the number of detected errors
 *  - errors: array used to store up to MAX_ERRORS validation errors
 *
 * Returns:
 *  - 1 if the SMILES string is valid according to the implemented checks
 *  - 0 if one or more validation errors were detected
 */
int validate_smiles(const char *input, int *error_count, Error errors[MAX_ERRORS]) {
	if (!input || !error_count || !errors) return 0;
	*error_count = 0;
	is_permitted(input, error_count, errors);
	closed_brackets(input, error_count, errors);
	ring_closed(input, error_count, errors);
	misc_check(input, error_count, errors);
	if (*error_count > 0) {
		print_errors(*error_count, errors);
		return 0;
	}
	return 1;
}

int get_error_count(int *error_count) {
    return *error_count;
}

const char* get_error_message(int index, int *error_count, Error errors[]) {
    if (index >= 0 && index < *error_count)
        return errors[index].message;
    return "";
}

int get_error_position(int index, int *error_count, Error errors[]) {
    if (index >= 0 && index < *error_count)
        return errors[index].position;
    return -1;
}


